#include <QObject>
#include <QDebug>
#include <QQmlInfo>

#include "qsdlwindow.h"

QSdlWindow::QSdlWindow(QObject *parent)
    : QSdlItem(parent)
    , m_sdl_window(0)
    , m_sdl_renderer(0)
{
    m_sdl_window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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


