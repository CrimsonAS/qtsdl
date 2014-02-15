#include "qsdlitem.h"
#include <QQmlInfo>

QSdlItem::QSdlItem(QObject *parent)
    : QObject(parent)
    , m_window(0)
    , m_parentItem(0)
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

int QSdlItem::width() const
{
    return m_width;
}

void QSdlItem::setWidth(int newWidth)
{
    if (m_width == newWidth)
        return;

    m_width = newWidth;
    emit widthChanged();
}

int QSdlItem::height() const
{
    return m_height;
}

void QSdlItem::setHeight(int newHeight)
{
    if (m_height == newHeight)
        return;

    m_height = newHeight;
    emit heightChanged();
}


// #define RENDER_DEBUG

void QSdlItem::render()
{
#ifdef RENDER_DEBUG
    qDebug() << "Render for " << this << " window " << window();
#endif
    foreach (QSdlItem *item, m_childItems) {
#ifdef RENDER_DEBUG
        qDebug() << "Rendering for " << item << " window " << item->m_window;
#endif
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
    fo->setParentItem(that);
    fo->m_window = that->m_window;

    // change m_window of all children, too
    foreach (QSdlItem *citem, fo->m_childItems) {
        citem->m_window = that->m_window;
    }
}

int QSdlItem::data_count(QQmlListProperty<QObject> *prop)
{
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    return that->m_childItems.count();
}

QObject *QSdlItem::data_at(QQmlListProperty<QObject> *prop, int idx)
{
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    return that->m_childItems.at(idx);
}

void QSdlItem::data_clear(QQmlListProperty<QObject> *prop)
{
    QSdlItem *that = static_cast<QSdlItem *>(prop->object);
    foreach (QSdlItem *obj, that->m_childItems) {
        obj->setParentItem(0);
    }
}

bool QSdlItem::mousePress(int mx, int my)
{
    // first, see if a child wants to handle it
    foreach (QSdlItem *it, m_childItems) {
        if (it->mousePress(mx, my))
            return true;
    }

    int ix = mx - x();
    int iy = my - y();

    qDebug() << ix << iy << width() << height();

    if (ix >= 0 && ix <= width() &&
        iy >= 0 && iy <= height()) {
        qDebug() << "Pressed " << this;
        emit pressed();
        return true;
    }

    return false;
}

bool QSdlItem::mouseRelease(int mx, int my)
{
    // first, see if a child wants to handle it
    foreach (QSdlItem *it, m_childItems) {
        if (it->mouseRelease(mx, my))
            return true;
    }

    int ix = mx - x();
    int iy = my - y();

    if (ix >= 0 && ix <= width() &&
        iy >= 0 && iy <= height()) {
        qDebug() << "Released " << this;
        emit released();
        // TODO: emit clicked() if the initial press and the release was on us
        return true;
    }
    return false;
}

QObject *QSdlItem::parentItem() const
{
    return m_parentItem;
}

void QSdlItem::setParentItem(QObject *pi)
{
    if (m_parentItem == pi)
        return;

    QSdlItem *p = qobject_cast<QSdlItem*>(pi);
    if (!p)
        return;


    if (m_parentItem)
        m_parentItem->m_childItems.remove(m_parentItem->m_childItems.indexOf(this));
    m_parentItem = p;
    if (p)
        p->m_childItems.append(this);
    emit parentItemChanged();
}
