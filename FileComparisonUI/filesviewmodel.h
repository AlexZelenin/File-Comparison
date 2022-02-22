#ifndef FILESVIEWMODEL_H
#define FILESVIEWMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QSqlTableModel>
#include <QQmlEngine>
#include <QThread>
#include <QFileInfoList>

#include "dbmanager.h"

class FilesViewModel : public QSqlTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_ADDED_IN_MINOR_VERSION(1)

public:
    explicit FilesViewModel(QSqlTableModel* paretn = Q_NULLPTR);
    virtual ~FilesViewModel();

    enum Roles {
        Id = Qt::UserRole + 1,
        LeftFile,
        RightFile,
        CheckSumm
    };

public:
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void started();
    void setFileListDb(const QFileInfoList& fileList, const DBTypes::ItemSides& side);
    void complete();
    void progressChanged(const int&);
    void maxProgress(const int&);
    void countFinded(const int&);
    void clearTableDb();
    void clearCompleted();
    void close();

public slots:
    void start();
    void clearTable();
    void setFileList(const QFileInfoList& fileList, const DBTypes::ItemSides& side);
    void closeThread();

private:
    QThread *m_thread;
    DBManager *m_db;
};

#endif // FILESVIEWMODEL_H
