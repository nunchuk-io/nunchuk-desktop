#ifndef QERRORCODE_H
#define QERRORCODE_H

#include <QObject>
#include "qUtils.h"

class QWarningMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString contentDisplay READ contentDisplay NOTIFY contentDisplayChanged)
    Q_PROPERTY(QString  explaination    READ explaination   NOTIFY explainationChanged)
    Q_PROPERTY(QString  what READ what NOTIFY whatChanged)
    Q_PROPERTY(int      code READ code NOTIFY codeChanged)
    Q_PROPERTY(int      type READ type WRITE setType NOTIFY typeChanged)
public:
    QWarningMessage();
    ~QWarningMessage();

    QString contentDisplay();

    int code() const;
    void setCode(int code);

    QString what() const;
    void setWhat(const QString &what);

    void setWarningMessage(int code, const QString &what, EWARNING::WarningType type = EWARNING::WarningType::EXCEPTION_MSG, const QString& explain = "");
    void resetWarningMessage();

    int type() const;
    void setType(int type);

    QString explaination() const;
    void setExplaination(const QString &explaination);

private:
    int code_;
    QString what_;
    int type_;
    QString explaination_;
    QString content_;

signals:
    void codeChanged();
    void whatChanged();
    void typeChanged();
    void explainationChanged();
    void contentDisplayChanged();
};

#endif // QERRORCODE_H
