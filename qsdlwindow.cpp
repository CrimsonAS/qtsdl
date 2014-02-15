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
