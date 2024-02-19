#include "DashRectangle.h"
#include <QPainterPath>

DashRectangle::DashRectangle(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    connect(this, &QQuickPaintedItem::widthChanged, this, &DashRectangle::slotUpdate);
    connect(this, &QQuickPaintedItem::heightChanged, this, &DashRectangle::slotUpdate);
}

int DashRectangle::borderWitdh() const
{
    return m_borderWitdh;
}

void DashRectangle::setBorderWitdh(int arg)
{
    if ((m_borderWitdh != arg) && (arg > 0)) {
        m_borderWitdh = arg;
        QQuickPaintedItem::update();
        emit borderWitdhChanged(arg);
    }
}

int DashRectangle::radius() const
{
    return m_radius;
}

void DashRectangle::setRadius(int arg)
{
    if ((m_radius != arg) && (arg > 0)) {
        m_radius = arg;
        QQuickPaintedItem::update();
        emit radiusChanged(arg);
    }
}

QColor DashRectangle::borderColor() const
{
    return m_borderColor;
}

void DashRectangle::setBorderColor(QColor borderColor)
{
    if (m_borderColor == borderColor)
        return;

    m_borderColor = borderColor;
    emit borderColorChanged(m_borderColor);
}

void DashRectangle::slotUpdate()
{
    QQuickPaintedItem::update();
}

void DashRectangle::paint(QPainter *painter)
{
    QPen pen;  // creates a default pen
    pen.setStyle(Qt::DashLine);
    pen.setWidth(m_borderWitdh);
    pen.setBrush(m_borderColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    painter->setPen(pen);
    painter->drawRoundedRect(QRectF(m_borderWitdh, m_borderWitdh, width() - 2*m_borderWitdh, height() - 2*m_borderWitdh), m_radius, m_radius);
}
