#include <QQmlInfo>
#include <QFile>

#include <SDL.h>

#include "qsdlwindow.h"
#include "qsdlrectangle.h"

QSdlRectangle::QSdlRectangle(QObject *parent)
    : QSdlItem(parent)
{
}

QSdlRectangle::~QSdlRectangle()
{
}

QColor QSdlRectangle::color() const
{
    return m_color;
}

void QSdlRectangle::setColor(const QColor &color)
{
    m_color = color;
    emit colorChanged();
}

void QSdlRectangle::render()
{
    SDL_Rect rect;
    rect.x = x();
    rect.y = y();
    rect.w = width();
    rect.h = height();
    SDL_SetRenderDrawColor(window()->renderer(), color().red(), color().green(), color().blue(), color().alpha());
    SDL_RenderFillRect(window()->renderer(), &rect);
    QSdlItem::render();
}




