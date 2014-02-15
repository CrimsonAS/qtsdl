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

