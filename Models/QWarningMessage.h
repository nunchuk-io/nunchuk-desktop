#ifndef QERRORCODE_H
#define QERRORCODE_H

#include <QObject>

class EWARNING: public QObject
{
    Q_OBJECT
    Q_ENUMS(WarningType)
public:
    enum class WarningType {
        NONE_MSG,
        SUCCESS_MSG,
        WARNING_MSG,
        ERROR_MSG,
        EXCEPTION_MSG
    };
};
Q_DECLARE_METATYPE(EWARNING::WarningType)

class POPUP: public QObject
{
    Q_OBJECT
    Q_ENUMS(PopupType)
    Q_ENUMS(WalletImportType)
public:
    enum class PopupType {
        PopupNone,
        PopupTop,
        PopupCenter,
        PopupBottom
    };
    enum class WalletImportType {
        IMPORT_WALLET_DB = 0,
        IMPORT_WALLET_DESCRIPTOR,
        IMPORT_WALLET_CONFIGFILE,
        IMPORT_WALLET_QRCODE_KEYSTONE,
        IMPORT_WALLET_QRCODE_PASSPORT
    };
};
Q_DECLARE_METATYPE(POPUP::PopupType)

class QWarningMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  contentDisplay  READ contentDisplay NOTIFY contentDisplayChanged)
    Q_PROPERTY(QString  explaination    READ explaination   NOTIFY explainationChanged)
    Q_PROPERTY(QString  what READ what NOTIFY whatChanged)
    Q_PROPERTY(int      code READ code NOTIFY codeChanged)
    Q_PROPERTY(int      type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int      popupType READ popupType WRITE setPopupType NOTIFY popupTypeChanged)
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

    int popupType() const;
    void setPopupType(int popup);

private:
    int code_;
    QString what_;
    int type_;
    QString explaination_;
    QString content_;
    int popupType_;
signals:
    void codeChanged();
    void whatChanged();
    void typeChanged();
    void explainationChanged();
    void contentDisplayChanged();
    void popupTypeChanged();
};
typedef QSharedPointer<QWarningMessage> QWarningMessagePtr;

#endif // QERRORCODE_H
