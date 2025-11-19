#ifndef DASHRECTANGLE_H
#define DASHRECTANGLE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>

class DashRectangle : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int      borderWitdh     READ borderWitdh    WRITE setBorderWitdh    NOTIFY borderWitdhChanged)
    Q_PROPERTY(int      radius          READ radius         WRITE setRadius         NOTIFY radiusChanged)
    Q_PROPERTY(QColor   borderColor     READ borderColor    WRITE setBorderColor    NOTIFY borderColorChanged)
    Q_PROPERTY(bool     isDashed        READ isDashed       WRITE setIsDashed       NOTIFY isDashedChanged)
    Q_PROPERTY(QColor   color           READ color          WRITE setColor          NOTIFY colorChanged)

public:
    explicit DashRectangle(QQuickItem *parent = nullptr);

    int borderWitdh() const;
    void setBorderWitdh(int);
    int radius() const;
    void setRadius(int);
    QColor borderColor() const;
    void setBorderColor(QColor borderColor);

    bool isDashed() const;
    void setIsDashed(bool isDashed);

    QColor color() const;
    void setColor(const QColor &color);

public slots:
    void slotUpdate();

private:
    int m_borderWitdh {0};
    int m_radius {0};
    QColor m_borderColor {Qt::black};
    bool m_isDashed {true};
    QColor m_color {Qt::white};
protected:
    virtual void paint(QPainter *painter);
signals:
    void borderWitdhChanged(int borderWitdh);
    void radiusChanged(int radius);
    void borderColorChanged(QColor borderColor);
    void isDashedChanged(bool isDashed);
    void colorChanged(QColor color);
};

#endif // DASHRECTANGLE_H
