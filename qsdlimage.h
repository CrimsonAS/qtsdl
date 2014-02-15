#ifndef QSDLIMAGE_H
#define QSDLIMAGE_H

#include <QUrl>
#include <QObject>

#include "qsdlitem.h"

struct SDL_Texture;

class QSdlImage : public QSdlItem
{
    Q_OBJECT
public:
    QSdlImage(QObject *parent = 0);
    ~QSdlImage();

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    QUrl source() const;
    void setSource(const QUrl &source);

    void render();

signals:
    void sourceChanged();

private:
    SDL_Texture *m_texture;
    QUrl m_source;
};

#endif

