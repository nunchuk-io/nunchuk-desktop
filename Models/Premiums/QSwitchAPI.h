#ifndef QSWITCHAPI_H
#define QSWITCHAPI_H

#include <QObject>

#define FREE_USER      -1
#define USER_WALLET     0
#define GROUP_WALLET    1
class QSwitchAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool qIsByzantine READ getIsByzantine WRITE setIsByzantine NOTIFY IsByzantineChanged)
    Q_PROPERTY(int  qAddHardware READ getAddHardware WRITE setAddHardware NOTIFY AddHardwareChanged)
public:
    explicit QSwitchAPI(int mode);
    static bool IsByzantine();
protected:
    bool getIsByzantine() const;
    void setIsByzantine(bool val);
    int getAddHardware() const;
    void setAddHardware(int val);
signals:
    void IsByzantineChanged();
    void AddHardwareChanged();
protected:
    int m_mode {};
    int mAddHardware {};
};

#endif // QSWITCHAPI_H
