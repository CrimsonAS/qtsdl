#ifndef QSDLWINDOW_H
#define QSDLWINDOW_H

#include <QObject>

#include <SDL.h>

#include "qsdlitem.h"

class QSdlWindow : public QSdlItem
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    QSdlWindow(QObject *parent = 0);
    ~QSdlWindow();

    QString title() const;
    void setTitle(const QString &title);

    SDL_Renderer *renderer() const;
    void render();

signals:
    void titleChanged();

private:
    SDL_Window *m_sdl_window;
    SDL_Renderer *m_sdl_renderer;
};

#endif
