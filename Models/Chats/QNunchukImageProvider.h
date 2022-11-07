#ifndef QNUNCHUKIMAGEPROVIDER_H
#define QNUNCHUKIMAGEPROVIDER_H
#include <QObject>
#include <QAtomicPointer>
#include <QQuickImageProvider>
#include <QtCore/QReadWriteLock>
#include <QtCore/QThread>
#include <room.h>
#include <connection.h>
#include <ssosession.h>
#include <user.h>
#include <uriresolver.h>
#include <jobs/mediathumbnailjob.h>

using namespace Quotient;
using Quotient::BaseJob;

class ThumbnailResponse : public QQuickImageResponse
{
    Q_OBJECT
public:
    ThumbnailResponse(Connection* c, QString id, QSize size);
    ~ThumbnailResponse() override = default;

private slots:
    // All these run in the main thread, not QML thread
    void startRequest();
    void prepareResult();
    void doCancel();

private:
    Connection* c;
    const QString mediaId;
    const QSize requestedSize;
    Quotient::MediaThumbnailJob* job = nullptr;
    QImage image;
    QString errorStr;
    mutable QReadWriteLock lock; // Guards ONLY these two above
    // The following overrides run in QML thread
    QQuickTextureFactory *textureFactory() const override;
    QString errorString() const override;
    void cancel() override;
};

class QNunchukImageProvider : public QQuickAsyncImageProvider
{
public:
    QNunchukImageProvider();
    QQuickImageResponse* requestImageResponse(const QString& id, const QSize& requestedSize) override;
    void setConnection(Quotient::Connection* c);
private:
    QAtomicPointer<Quotient::Connection> m_connection;
    Q_DISABLE_COPY(QNunchukImageProvider)
};

#endif // QNUNCHUKIMAGEPROVIDER_H
