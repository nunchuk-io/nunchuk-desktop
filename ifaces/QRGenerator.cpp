#include "QRGenerator.h"
#include <QDebug>
QRGenerator::QRGenerator(){}

QRGenerator::~QRGenerator(){}

QImage QRGenerator::qrImage(QString text, QSize _size, int _border){
    return generate(text, _size, _border);
}

QImage QRGenerator::generate(QString text, QSize _size, int _border)
{
    QImage img_ret;
    const int border = _border;
    const QrCode qr = QrCode::encodeText(text.toStdString().c_str(), QrCode::Ecc::LOW);
    int size = qr.getSize();
    QImage img(size, size, QImage::Format_RGB888);
    img.fill(Qt::white);
    if(size > 0){
        for (int _y = 0; _y < size; _y++) {
            for (int _x = 0; _x < size; _x++) {
                if(0 != qr.getModule(_x, _y)){
                    img.setPixelColor(_x, _y, QColor(Qt::black));
                }
            }
        }
    }
    img_ret = img.scaled(_size.width() - 2*border, _size.height() - 2*border, Qt::KeepAspectRatioByExpanding);
    QImage imgResult(_size.width(), _size.height(), QImage::Format_ARGB32);
    imgResult.fill(Qt::white);
    QPainter painter(&imgResult);
    QPoint destPos = QPoint(border, border);
    painter.drawImage(destPos, img_ret);
    painter.end();
    return imgResult;
}
