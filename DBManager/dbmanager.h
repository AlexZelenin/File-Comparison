#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "DBManager_global.h"

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QFileInfoList>
#include <QMutex>
#include <QAtomicInteger>

class DBMANAGER_EXPORT DBManager : public QObject
{
    Q_OBJECT

    enum FieldType {
        LeftFile,
        CheckSumm,
        RightFile,
    };

public:
    explicit DBManager(QObject* parent = Q_NULLPTR);
    virtual ~DBManager();

public slots:
    void insertFileList(const QFileInfoList& list, const DBTypes::ItemSides& table_type);
    void findTheSame();
    void clearTables();
    void close();

private:
    void init();
    QString fileCheckSumm(const QString& filename) const;
    void insertToCheckedItem(const QList<DBTypes::ITEM> list_items);

signals:
    void error(const QString&);
    void setRow(const int&);
    void checkComplete();
    void clearComplete();
    void canClose();

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
