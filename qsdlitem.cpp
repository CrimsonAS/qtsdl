#include "qsdlitem.h"
#include <QQmlInfo>

QSdlItem::QSdlItem(QObject *parent)
    : QObject(parent)
    , m_window(0)
{
}

QSdlItem::~QSdlItem()
{
}

QSdlWindow *QSdlItem::window() const
{
    return m_window;
}

int QSdlItem::x() const
{
    return m_x;
}

void QSdlItem::setX(int newX)
{
    qDebug() << "Changing X to " << newX;
    if (m_x == newX)
        return;

    m_x = newX;
    emit xChanged();
}

int QSdlItem::y() const
{
    return m_y;
}

void QSdlItem::setY(int newY)
{
    qDebug() << "Changing Y to " << newY;
    if (m_y == newY)
        return;

    m_y = newY;
    emit yChanged();
}

void QSdlItem::render()
{
    qDebug() << "Render for " << this << " window " << window();
    foreach (QObject *child, children()) {
        QSdlItem *item = qobject_cast<QSdlItem *>(child);
        if (!item)
            continue;
        qDebug() << "Rendering for " << item << " window " << item->m_window;
        item->render();
    }
}


QQmlListProperty<QObject> QSdlItem::data()
{
    return QQmlListProperty<QObject>(this, 0, QSdlItem::data_append,
                                             QSdlItem::data_count,
                                             QSdlItem::data_at,
                                             QSdlItem::data_clear);
}

void QSdlItem::data_append(QQmlListProperty<QObject> *prop, QObject *object)
{
    QSdlItem *fo = qobject_cast<QSdlItem *>(object);
    if (!fo) {
        qWarning("What type of item is this? Not a QSdlItem!");
        return;
    }

    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    qDebug() << "appending " << fo << " to " << that;
    fo->setParent(that);
    fo->m_window = that->m_window;

    // change m_window of all children, too
    foreach (QObject *c, fo->children()) {
        QSdlItem *citem = qobject_cast<QSdlItem *>(c);
        if (!citem)
            continue;
        citem->m_window = that->m_window;
    }
}

int QSdlItem::data_count(QQmlListProperty<QObject> *prop)
{
    // TODO: don't abuse children for this
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    return that->children().count();
}

QObject *QSdlItem::data_at(QQmlListProperty<QObject> *prop, int idx)
{
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    return that->children().at(idx);
}

void QSdlItem::data_clear(QQmlListProperty<QObject> *prop)
{
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    foreach (QObject *obj, that->children()) {
        obj->setParent(0);
    }
}


