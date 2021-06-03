
#ifndef STATE_ID_SCR_APP_SETTINGS_H
#define STATE_ID_SCR_APP_SETTINGS_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_APP_SETTING_Entry(QVariant msg);
void SCR_APP_SETTING_Exit(QVariant msg);
void EVT_APP_SETTING_UPDATE_SETTING_HANDLER(QVariant msg);
void EVT_APP_SETTING_CHANGE_PASSPHRASE_HANDLER(QVariant msg);
void EVT_APP_SETTING_REQUEST_RESTART_HANDLER(QVariant msg);
void EVT_APP_SETTING_REQUEST_RESCAN_HANDLER(QVariant msg);
void EVT_APP_SETTING_BACK_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_APP_SETTINGS_H
