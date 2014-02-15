#include <QQmlInfo>
#include <QFile>

#include <SDL.h>

#include "qsdlwindow.h"
#include "qsdlimage.h"

QSdlImage::QSdlImage(QObject *parent)
    : QSdlItem(parent)
    , m_texture(0)
{
}

QSdlImage::~QSdlImage()
{
    SDL_DestroyTexture(m_texture);
}

QUrl QSdlImage::source() const
{
    return m_source;
}

void QSdlImage::setSource(const QUrl &source)
{
    if (!source.isLocalFile()) {
        qmlInfo(this) << "Source is not a local file: " << source;
        return;
    }

    m_source = source;

    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = 0;
    }

    emit sourceChanged();
}

void QSdlImage::render()
{
    if (!m_texture) {
    //    SDL_Surface *tmp = SDL_LoadBMP("abmp.bmp");
        SDL_Surface *tmp = SDL_LoadBMP(QFile::encodeName(m_source.toLocalFile()).constData());
        if (!tmp) {
            // TODO: error str
            qmlInfo(this) << "Error loading " << m_source << SDL_GetError();
            return;
        }

        m_texture = SDL_CreateTextureFromSurface(window()->renderer(), tmp);
        SDL_FreeSurface(tmp);
    }

    qDebug() << "Rendering texture";
    SDL_RenderCopy(window()->renderer(), m_texture, NULL, NULL);
    QSdlItem::render();
}




