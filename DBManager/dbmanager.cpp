#include "dbmanager.h"

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QCryptographicHash>
#include <QFile>

#include <QDebug>

QMutex mutex;
static int progress = 0;


namespace DBTypes {

InterruptControl* InterruptControl::mPtr = 0;

InterruptControl* InterruptControl::instance()
{
    if (!mPtr) mPtr = new DBTypes::InterruptControl;
    return mPtr;
}

void InterruptControl::setInterruptState(bool state)
{
    interrupt.fetchAndStoreRelaxed(state);
}

bool InterruptControl::state() const noexcept
{
    return interrupt.loadRelaxed();
}
}

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    init();
}

DBManager::~DBManager() {
    if (m_db.isOpen())
        m_db.close();
    qDebug() << "~BDManager";
}

void DBManager::init()
{
    m_db = QSqlDatabase::addDatabase(DBTypes::dbDriver);
    m_db.setDatabaseName(DBTypes::dbName);

    if (!m_db.open()) {
        qDebug() << m_db.lastError().text();
    }

    m_db.exec("PRAGMA synchronous = OFF");
    m_db.exec("PRAGMA journal_mode = MEMORY");
    m_db.exec("PRAGMA temp_store = MEMORY");

    const QString& left_item_table = "CREATE TABLE IF NOT EXISTS left_item ("
                                     "id INTEGER PRIMARY KEY, "
                                     "file_url TEXT NOT NULL, "
                                     "check_summ TEXT NOT NULL "
                                     ");";

    const QString& right_item_table = "CREATE TABLE IF NOT EXISTS right_item ("
                                      "id INTEGER PRIMARY KEY, "
                                      "file_url TEXT NOT NULL, "
                                      "check_summ TEXT NOT NULL"
                                      ");";

    const QString& checked_item = "CREATE TABLE IF NOT EXISTS checked_item ("
                                  "id INTEGER PRIMARY KEY, "
                                  "file_left TEXT NOT NULL, "
                                  "file_right TEXT NOT NULL, "
                                  "check_summ TEXT NOT NULL"
                                  ");";

    QSqlQuery query(m_db);

    if (!query.exec(left_item_table)) {
        qDebug() << Q_FUNC_INFO << "Error: " << query.lastError().text();
    }

    if (!query.exec(right_item_table)) {
        qDebug() << Q_FUNC_INFO << "Error: " << m_db.lastError().text();
    }

    if (!query.exec(checked_item)) {
        qDebug() << Q_FUNC_INFO << "Error: " << m_db.lastError().text();
    }
}

void DBManager::close()
{
    mutex.lock();
    clearTables();
    mutex.unlock();

    emit canClose();
}

void DBManager::insertFileList(const QFileInfoList &list,
                               const DBTypes::ItemSides& table_type)
{
    QSqlQuery query(m_db);

    int id = 0;
    foreach(const QFileInfo& file, list) {
        mutex.lock();
        if (dbInterrupt()->state()) {
            mutex.unlock();
            break;
        }

        switch (table_type) {
        case DBTypes::ItemSides::Left:
            query.prepare(QString("INSERT OR IGNORE INTO %1_item (id, file_url, check_summ)"
                                  "VALUES (:id, :file_url, :check_summ)").arg("left"));
            break;
        case DBTypes::ItemSides::Right:
            query.prepare(QString("INSERT OR IGNORE INTO %1_item (id, file_url, check_summ)"
                                  "VALUES (:id, :file_url, :check_summ)").arg("right"));
            break;
        default:
            qDebug() << "Error: incorrect side item";
            return;
        }

        const QString& filepath = file.filePath();
        const QString& check_summ = fileCheckSumm(filepath);

        query.bindValue("id", id);
        query.bindValue(":file_url", filepath);
        query.bindValue(":check_summ", check_summ);

        if (!query.exec()) {
            qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
        }

        emit setRow(++progress);

        id++;
        mutex.unlock();
    }
}

void DBManager::findTheSame()
{
    QSqlQuery query(m_db);

    const QString request = "SELECT left.file_url, left.check_summ, right.file_url "
                            "FROM left_item AS left "
                            "INNER JOIN right_item AS right "
                            "ON left.check_summ=right.check_summ";


    if (!query.exec(request)) {
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
    }

    QList<DBTypes::ITEM> list_items;

    int i = 1;
    while(query.next()) {
        mutex.lock();
        if (dbInterrupt()->state()) {
            mutex.unlock();
            break;
        }
        const QString& file_left = query.value(LeftFile).toString();
        const QString& check_summ = query.value(CheckSumm).toString();
        const QString& file_right = query.value(RightFile).toString();
        DBTypes::ITEM item;

        item.filename_left = file_left;
        item.filename_right = file_right;
        item.check_summ = check_summ;
        item.id = i;

        list_items.append(item);

        i++;
        mutex.unlock();
    }

    query.clear();

    insertToCheckedItem(list_items);
}

void DBManager::clearTables()
{
    progress = 0;

    if (!m_db.isOpen()) return;

    QSqlQuery query(m_db);

    if (!query.exec("DELETE FROM left_item")) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
    }

    query.clear();

    if (!query.exec("DELETE FROM right_item")) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
    }

    query.clear();

    if (!query.exec("DELETE FROM checked_item")) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
    }

    emit clearComplete();
}

QString DBManager::fileCheckSumm(const QString &filename) const
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        if (hash.addData(&file)) {
            return hash.result().toHex();
        }
    }
    return QString();
}

void DBManager::insertToCheckedItem(const QList<DBTypes::ITEM> list_items)
{
    QSqlQuery query(m_db);

    foreach(const DBTypes::ITEM& item, list_items) {
        mutex.lock();
        if (dbInterrupt()->state()) {
            mutex.unlock();
            break;
        }

        query.prepare("INSERT INTO checked_item (id, file_left, file_right, check_summ) "
                      "VALUES(:id, :file_left, :file_right, :check_summ)");

        query.bindValue(":id", item.id);
        query.bindValue(":file_left", item.filename_left);
        query.bindValue(":file_right", item.filename_right);
        query.bindValue(":check_summ", item.check_summ);

        if (!query.exec()) {
            qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
        }

        mutex.unlock();
    }

    emit checkComplete();
}
