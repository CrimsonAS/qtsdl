#ifndef QSDLITEM_H
#define QSDLITEM_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector>

class QSdlWindow;

class QSdlItem : public QObject
{
    Q_OBJECT

    // why can't i type this as QSdlItem?!
    Q_PROPERTY(QObject *parent READ parentItem WRITE setParentItem NOTIFY parentItemChanged)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false)

    Q_CLASSINFO("DefaultProperty",  "data")
public:
    QSdlItem(QObject *parent = 0);
    ~QSdlItem();

    QSdlWindow *window() const;

    virtual void render();
    virtual bool mousePress(int x, int y);
    virtual bool mouseRelease(int x, int y);

    QQmlListProperty<QObject> data();
    static void data_append(QQmlListProperty<QObject> *, QObject *);
    static int data_count(QQmlListProperty<QObject> *);
    static QObject *data_at(QQmlListProperty<QObject> *, int);
    static void data_clear(QQmlListProperty<QObject> *);

    int x() const;
    int y() const;
    void setX(int x);
    void setY(int y);

    int width() const;
    int height() const;
    void setWidth(int width);
    void setHeight(int height);

    QObject *parentItem() const;
    void setParentItem(QObject *parent);

signals:
    void xChanged();
    void yChanged();

    void widthChanged();
    void heightChanged();

    void pressed();
    void released();

    void parentItemChanged();

protected:
    QSdlWindow *m_window;
    QSdlItem *m_parentItem;
    QVector<QSdlItem *> m_childItems;
    QQmlListProperty<QObject> m_data;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};

#endif

