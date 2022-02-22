#ifndef DBMANAGER_GLOBAL_H
#define DBMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QObject>
#include <QString>
#include <QHash>
#include <QMetaType>


#if defined(DBMANAGER_LIBRARY)
#  define DBMANAGER_EXPORT Q_DECL_EXPORT
#else
#  define DBMANAGER_EXPORT Q_DECL_IMPORT
#endif

namespace DBTypes {
Q_NAMESPACE

const QString dbName = QStringLiteral("comparator_db.sqlite");
const QString dbDriver = QStringLiteral("QSQLITE");

typedef struct _item {
    int id;
    QString filename_left;
    QString filename_right;
    QString check_summ;
} ITEM;

class DBMANAGER_EXPORT CompareTypes : public QObject
{
    Q_GADGET

    explicit CompareTypes(QObject* parent = Q_NULLPTR) : QObject(parent) {}
    virtual ~CompareTypes() {}
public:


    enum ItemSides  {
        Left,
        Right
    };
    Q_ENUM(ItemSides)
};

typedef CompareTypes::ItemSides ItemSides;

class DBMANAGER_EXPORT InterruptControl {
    InterruptControl() {}
    ~InterruptControl() {}
    Q_DISABLE_COPY(InterruptControl)

    static InterruptControl* mPtr;
public:
    static InterruptControl* instance();

    inline void setInterruptState(bool state);
    inline bool state() const noexcept;

private:
    QAtomicInteger<bool> interrupt{false};
};

}

#define dbInterrupt() \
    DBTypes::InterruptControl::instance()

#endif // DBMANAGER_GLOBAL_H
