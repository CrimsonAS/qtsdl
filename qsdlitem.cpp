/*
 * Copyright (c) 2014 Robin Burchell <robin+git@viroteck.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
