#include "comparecontroller.h"

#include <QFileDialog>
#include <QDir>

#include <QDebug>

#include "dbmanager.h"

CompareController::CompareController(QObject *parent)
    : QObject{parent}
{

}

CompareController::~CompareController()
{
    qDebug() << "~CompareController";
}

void CompareController::openDir(const DBTypes::ItemSides& type) {
    QString path = QFileDialog::getExistingDirectory(Q_NULLPTR, tr("Выбор директории"), ".");

    QDir dir(path);

    const int& count = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot).count();

    switch(type) {
    case DBTypes::ItemSides::Left:
        emit pathLeftSelected(path);
        emit countInLeft(count);
        m_fileListLeft = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        break;
    case DBTypes::ItemSides::Right:
        emit pathRightSelected(path);
        emit countInRight(count);
        m_fileListRight = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
        break;
    default:
        break;
    }
}

void CompareController::executeCompare()
{
    emit fileList(m_fileListLeft, DBTypes::ItemSides::Left);
    emit fileList(m_fileListRight, DBTypes::ItemSides::Right);

    emit start();
}

void CompareController::clear()
{
    emit clearTable();
}

void CompareController::closeThread()
{
    emit closeTh();
}
