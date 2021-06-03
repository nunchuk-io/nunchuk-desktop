#include "QRCodeItem.h"
QSharedPointer<QRGenerator> QRCodeItem::m_QRgen = QSharedPointer<QRGenerator>(NULL);
QRCodeItem::QRCodeItem()
{
    m_borderWitdh  = 0;
    if (m_QRgen.data() == NULL) {
        m_QRgen = QSharedPointer<QRGenerator>(new QRGenerator());
    }
    connect(this, &QQuickPaintedItem::widthChanged, this, &QRCodeItem::slotUpdate);
    connect(this, &QQuickPaintedItem::heightChanged, this, &QRCodeItem::slotUpdate);
}

QRCodeItem::~QRCodeItem()
{}

QString QRCodeItem::textInput() const
{
    return m_textInput;
}

int QRCodeItem::borderWitdh() const
{
    return m_borderWitdh;
}

QString QRCodeItem::save(QString name, QString path)
{
    if(name.isEmpty()) {
        return "";
    } else {
        int border = 10;
        QImage qrImage = m_QRgen.data()->qrImage(name, QSize(this->width(), this->height()), border);

        QDir dir(path);
        if(dir.exists() == false){
        }
        qrImage.save(path+"Img0.png", "PNG");
        return path + "Img0.png";
    }
}

void QRCodeItem::paint(QPainter *painter)
{
    if(NULL != painter &&  !m_textInput.isEmpty() ){
        QImage qrImage = m_QRgen.data()->qrImage(m_textInput, QSize(this->width(), this->height()), m_borderWitdh);
        painter->drawImage(QRectF(0, 0, qrImage.width(), qrImage.height()), qrImage);
    }
}

void QRCodeItem::setTextInput(QString arg)
{
    if (m_textInput != arg){
        m_textInput = arg;
        QQuickPaintedItem::update();
        emit textInputChanged(arg);
    }
}

void QRCodeItem::setBorderWitdh(int arg)
{
    if ((m_borderWitdh != arg) && (arg > 0)) {
        m_borderWitdh = arg;
        QQuickPaintedItem::update();
        emit borderWitdhChanged(arg);
    }
}

void QRCodeItem::slotUpdate()
{
    QQuickPaintedItem::update();;
}
