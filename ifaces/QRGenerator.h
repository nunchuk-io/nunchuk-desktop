#ifndef QRGENERATOR_H
#define QRGENERATOR_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QPainter>
#include "contrib/nayuki/QrCode.hpp"

using QRGeneratoren::QrCode;

class QRGenerator
{
public:
    QRGenerator();
    ~QRGenerator();
    QImage qrImage(QString text, QSize _size = QSize(115, 120), int _border=6);

private:
    QImage generate(QString text, QSize _size, int _border);
};

#endif // QRGENERATOR_H
