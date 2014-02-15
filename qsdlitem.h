#ifndef QSDLITEM_H
#define QSDLITEM_H

#include <QObject>
#include <QQmlListProperty>

class QSdlWindow;

class QSdlItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false)

    Q_CLASSINFO("DefaultProperty",  "data")
public:
    QSdlItem(QObject *parent = 0);
    ~QSdlItem();

    QSdlWindow *window() const;

    virtual void render();

    QQmlListProperty<QObject> data();
    static void data_append(QQmlListProperty<QObject> *, QObject *);
    static int data_count(QQmlListProperty<QObject> *);
    static QObject *data_at(QQmlListProperty<QObject> *, int);
    static void data_clear(QQmlListProperty<QObject> *);

    int x() const;
    int y() const;
    void setX(int newX);
    void setY(int newY);

signals:
    void xChanged();
    void yChanged();

protected:
    QSdlWindow *m_window;
    QQmlListProperty<QObject> m_data;
    int m_x;
    int m_y;
};

#endif

