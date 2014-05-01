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

#include <QQmlEngine>
#include <QQmlComponent>
#include <QCoreApplication>
#include <QtQml>

#include "qsdlwindow.h"
#include "qsdlitem.h"
#include "qsdlimage.h"
#include "qsdlrectangle.h"

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        qWarning() << "SDL_Init Error: " << SDL_GetError();
        return -1;
    }

    QCoreApplication a(argc, argv);

    qmlRegisterType<QSdlWindow>("Sdl", 2, 0, "Window");
    qmlRegisterType<QSdlImage>("Sdl", 2, 0, "Image");
    qmlRegisterType<QSdlRectangle>("Sdl", 2, 0, "Rectangle");

    QQmlEngine e;
    QQmlComponent c(&e, QUrl::fromLocalFile("test.qml"));

    if (c.isError()) {
        qWarning() << "Error loading test.qml:";
        foreach (const QQmlError &e, c.errors()) {
            qWarning("%s - %d:%d: %s", qPrintable(e.url().toString()), e.line(), e.column(), qPrintable(e.description()));
            return -1;
        }
    }

    // TODO: don't hardcode, generate list of windows, render all
    QSdlWindow *win = static_cast<QSdlWindow *>(c.create());

    SDL_Event se;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&se)) {
            switch (se.type) {
                case SDL_QUIT:
                case SDL_KEYDOWN:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN: {
                    // TODO: button
                    SDL_MouseButtonEvent *ev = reinterpret_cast<SDL_MouseButtonEvent *>(&se);
                    win->mousePress(ev->x, ev->y);
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    SDL_MouseButtonEvent *ev = reinterpret_cast<SDL_MouseButtonEvent *>(&se);
                    win->mouseRelease(ev->x, ev->y);
                    break;
                }
                case SDL_WINDOWEVENT: {
                    SDL_WindowEvent *ev = reinterpret_cast<SDL_WindowEvent *>(&se);
                    switch (ev->event) {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            win->setWidth(ev->data1);
                            win->setHeight(ev->data2);
                            break;
                    }
                    break;
                }
            }
        }

        win->render();
    }

    SDL_Quit();
    return 0;
}

