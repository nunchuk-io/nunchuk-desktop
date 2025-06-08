#ifndef HOTKEY_H
#define HOTKEY_H

#include "Commons/QStateFlow.h"
#include "ISignerManagement.h"
#include <QObject>

class HotKey : public QStateFlow, public ISignerManagement {
    Q_OBJECT

public:
    HotKey() {};
    ~HotKey() {};

public slots:
    void createHotKey();
    void requestBackupHotKey(const QString &masterSignerId);
    void finishBackupHotKey();
private:
};

#endif // HOTKEY_H