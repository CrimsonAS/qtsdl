#ifndef QSDLRECTANGLE
#define QSDLRECTANGLE

#include <QColor>
#include <QObject>

#include "qsdlitem.h"

struct SDL_Texture;

class QSdlRectangle : public QSdlItem
{
    Q_OBJECT
public:
    QSdlRectangle(QObject *parent = 0);
    ~QSdlRectangle();

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    QColor color() const;
    void setColor(const QColor &color);

    void render();

signals:
    void colorChanged();

private:
    QColor m_color;
};

#endif

