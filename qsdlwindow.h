#ifndef QSDLWINDOW_H
#define QSDLWINDOW_H

#include <QObject>

#include <SDL.h>

#include "qsdlitem.h"

class QSdlWindow : public QSdlItem
{
    Q_OBJECT
public:
    QSdlWindow(QObject *parent = 0);
    ~QSdlWindow();

    SDL_Renderer *renderer() const;
    void render();

private:
    SDL_Window *m_sdl_window;
    SDL_Renderer *m_sdl_renderer;
};

#endif
