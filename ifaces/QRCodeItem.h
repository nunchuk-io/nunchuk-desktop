#ifndef QRCODEITEM_H
#define QRCODEITEM_H

#include <QtCore>
#include <QQuickPaintedItem>
#include <QRect>
#include <QBrush>
#include <QSharedPointer>
#include "QRGenerator.h"

class QRCodeItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString  textInput       READ textInput      WRITE setTextInput      NOTIFY textInputChanged)
    Q_PROPERTY(int      borderWitdh     READ borderWitdh    WRITE setBorderWitdh    NOTIFY borderWitdhChanged)
public:
    QRCodeItem();
    ~QRCodeItem();
    QString textInput() const;
    int borderWitdh() const;
    QString save(QString name, QString path);
protected:
    void paint(QPainter *painter);

private:
    static QSharedPointer<QRGenerator> m_QRgen;

    QString m_textInput;
    int m_borderWitdh;
signals:
    void textInputChanged(QString arg);
    void borderWitdhChanged(int arg);

public:
    void setTextInput(QString arg);
    void setBorderWitdh(int arg);
    void slotUpdate();
};
#endif // QRCODEITEM_H
