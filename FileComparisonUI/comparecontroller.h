#ifndef COMPARECONTROLLERH_H
#define COMPARECONTROLLERH_H

#include <QObject>
#include <QFileInfoList>
#include <QThread>

#include "dbmanager.h"

class CompareController : public QObject
{
    Q_OBJECT

public:
    explicit CompareController(QObject *parent = Q_NULLPTR);
    virtual ~CompareController();

public slots:
    Q_INVOKABLE void openDir(const DBTypes::ItemSides& type);
    Q_INVOKABLE void executeCompare();
    Q_INVOKABLE void clear();
    Q_INVOKABLE void closeThread();

signals:
    void pathLeftSelected(const QString&);
    void pathRightSelected(const QString&);
    void countInLeft(const int&);
    void countInRight(const int&);
    void fileList(const QFileInfoList&, const DBTypes::ItemSides&);

    void start();
    void complete();
    void clearTable();
    void closeTh();

    void setRow(const int&);

private:
    QFileInfoList m_fileListLeft;
    QFileInfoList m_fileListRight;
    QThread *m_thread;
};

#endif // COMPARECONTROLLERH_H
