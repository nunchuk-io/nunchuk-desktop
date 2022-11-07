#ifndef QNUNCHUKMATRIXEVENT_H
#define QNUNCHUKMATRIXEVENT_H

#include <QObject>
#include "nunchukmatrix.h"

class QNunchukMatrixEvent : public QObject
{
    Q_OBJECT
public:
    QNunchukMatrixEvent();
    QNunchukMatrixEvent(nunchuk::NunchukMatrixEvent e);
    ~QNunchukMatrixEvent();
    nunchuk::NunchukMatrixEvent nunchukEvent() const;

    QString get_type() const;
    QString get_content() const;
    QString get_event_id() const;
    QString get_room_id() const;
    QString get_sender() const;
    time_t get_ts() const;

    void set_type(const QString& value);
    void set_content(const QString& value);
    void set_event_id(const QString& value);
    void set_room_id(const QString& value);
    void set_sender(const QString& value);
    void set_ts(time_t value);

private:
    nunchuk::NunchukMatrixEvent evt_;
};

#endif // QNUNCHUKMATRIXEVENT_H
