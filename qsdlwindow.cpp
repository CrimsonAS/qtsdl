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

#include <QObject>
#include <QDebug>
#include <QQmlInfo>

#include "qsdlwindow.h"

QSdlWindow::QSdlWindow(QObject *parent)
    : QSdlItem(parent)
    , m_sdl_window(0)
    , m_sdl_renderer(0)
{
    setX(100);
    setY(100);
    setWidth(640);
    setHeight(480);

    m_sdl_window = SDL_CreateWindow("QSdlWindow", x(), y(), width(), height(), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!m_sdl_window) {
        qmlInfo(this) << "SDL_CreateWindow Error: " << SDL_GetError();
        return;
    }

    m_sdl_renderer = SDL_CreateRenderer(m_sdl_window, -1, 0);
    if (!m_sdl_renderer) {
        // TODO: errmsg
        qmlInfo(this) << "SDL_CreateRenderer failed!";
        return;
    }

    m_window = this;
}

QSdlWindow::~QSdlWindow()
{
    SDL_DestroyRenderer(m_sdl_renderer);
    SDL_DestroyWindow(m_sdl_window);
}

SDL_Renderer *QSdlWindow::renderer() const
{
    return m_sdl_renderer;
}

void QSdlWindow::render()
{
    SDL_RenderClear(m_sdl_renderer);

    // render children
    QSdlItem::render();

    SDL_RenderPresent(m_sdl_renderer);
}

QString QSdlWindow::title() const
{
    const char *t = SDL_GetWindowTitle(m_sdl_window);
    return QString::fromUtf8(t);
}

void QSdlWindow::setTitle(const QString &title)
{
    SDL_SetWindowTitle(m_sdl_window, title.toUtf8().constData());
}
