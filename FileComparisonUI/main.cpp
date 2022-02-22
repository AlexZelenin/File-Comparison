#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "comparecontroller.h"
#include "filesviewmodel.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CompareController controller;
    FilesViewModel model;

    QObject::connect(&controller, &CompareController::fileList,
                     &model, &FilesViewModel::setFileList);
    QObject::connect(&controller, &CompareController::start,
                     &model, &FilesViewModel::start);
    QObject::connect(&controller, &CompareController::clearTable,
                     &model, &FilesViewModel::clearTable);
    QObject::connect(&controller, &CompareController::closeTh,
                     &model, &FilesViewModel::closeThread);

    qRegisterMetaType<DBTypes::ItemSides>("DBTypes::ItemSides");
    qmlRegisterUncreatableType<DBTypes::CompareTypes>("Compare.Types", 1, 0,
                                                  "CompareTypes", "For types");

    engine.rootContext()->setContextProperty("controller", &controller);
    engine.rootContext()->setContextProperty("filesviewmodel", &model);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
