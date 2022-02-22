#include "filesviewmodel.h"

#include <QSqlQuery>
#include <QApplication>

#include "DBManager_global.h"


FilesViewModel::FilesViewModel(QSqlTableModel* parent)
    : QSqlTableModel(parent)
{
    m_thread = new QThread;

    m_db = new DBManager;
    m_db->moveToThread(m_thread);

    connect(this, &FilesViewModel::setFileListDb, m_db, &DBManager::insertFileList, Qt::QueuedConnection);
    connect(this, &FilesViewModel::started, m_db, &DBManager::findTheSame, Qt::QueuedConnection);

    connect(m_db, SIGNAL(setRow(int)), this, SIGNAL(progressChanged(int)), Qt::QueuedConnection);

    connect(this, &FilesViewModel::clearTableDb, m_db, &DBManager::clearTables, Qt::QueuedConnection);
    connect(this, &FilesViewModel::close, m_db, &DBManager::close, Qt::QueuedConnection);

    connect(m_db, &DBManager::canClose, m_thread, &QThread::quit);
    connect(m_thread, &QThread::finished, qApp, &QApplication::quit);

    connect(m_db, &DBManager::checkComplete, this, [this] () {
        setQuery(QSqlQuery("SELECT id, file_left, file_right, check_summ FROM checked_item;"));
        emit countFinded(rowCount());
    }, Qt::QueuedConnection);

    m_thread->start();
}

FilesViewModel::~FilesViewModel() {
    if (m_db) delete m_db;
    if (m_thread) delete m_thread;
    qDebug() << "~FilesViewModel";
}

QVariant FilesViewModel::data(const QModelIndex &index, int role) const
{
    if(role < Qt::UserRole)
        return  QSqlTableModel::data(index, role);

    int column;
    QModelIndex modelIndex;

    switch(role) {
    case Id:
        column = role - Qt::UserRole - 1;
        modelIndex = this->index(index.row(), column);
        return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    case LeftFile:
        column = role - Qt::UserRole - 1;
        modelIndex = this->index(index.row(), column);
        return  QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    case RightFile:
        column = role - Qt::UserRole - 1;
        modelIndex = this->index(index.row(), column);
        return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    case CheckSumm:
        column = role - Qt::UserRole - 1;
        modelIndex = this->index(index.row(), column);
        return QSqlTableModel::data(modelIndex, Qt::DisplayRole);

    default:
        return QSqlTableModel::data(index, role);
    }

    return QSqlTableModel::data(index, role);
}

QHash<int, QByteArray> FilesViewModel::roleNames() const
{
    QHash<int , QByteArray> roleNames;

    roleNames[Id] = "id";
    roleNames[LeftFile] = "left_file";
    roleNames[RightFile] = "right_file";
    roleNames[CheckSumm] = "check_summ";

    return roleNames;
}

void FilesViewModel::start()
{
    emit started();
}

void FilesViewModel::clearTable()
{
    clear();
    emit clearTableDb();
}

void FilesViewModel::setFileList(const QFileInfoList &fileList, const DBTypes::ItemSides &side)
{
    emit maxProgress(fileList.count());
    emit setFileListDb(fileList, side);
}

void FilesViewModel::closeThread()
{
    dbInterrupt()->setInterruptState(true);
    emit close();
}
