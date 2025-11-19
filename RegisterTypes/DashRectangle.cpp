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
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (m_borderWitdh > 0) {
        QPen pen;
        pen.setStyle(m_isDashed ? Qt::DashLine : Qt::SolidLine);
        pen.setWidth(m_borderWitdh);
        pen.setBrush(m_borderColor);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter->setPen(pen);
    } else {
        painter->setPen(Qt::NoPen); // no border
    }

    painter->setBrush(m_color);

    const QRectF rect = (m_borderWitdh > 0)
        ? QRectF(m_borderWitdh, m_borderWitdh,
                 width() - 2 * m_borderWitdh, height() - 2 * m_borderWitdh)
        : QRectF(0, 0, width(), height());

    painter->drawRoundedRect(rect, m_radius, m_radius);
}


bool DashRectangle::isDashed() const {
    return m_isDashed;
}

void DashRectangle::setIsDashed(bool isDashed) {
    if (m_isDashed == isDashed)
        return;
    m_isDashed = isDashed;
    QQuickPaintedItem::update();
    emit isDashedChanged(isDashed);
}

QColor DashRectangle::color() const {
    return m_color;
}

void DashRectangle::setColor(const QColor &color) {
    if (m_color == color)
        return;
    m_color = color;
    QQuickPaintedItem::update();
    emit colorChanged(m_color);
}