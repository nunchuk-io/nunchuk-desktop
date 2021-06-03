
#ifndef STATE_ID_ROOT_H
#define STATE_ID_ROOT_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void ROOT_Entry(QVariant msg);
void ROOT_Exit(QVariant msg);
void EVT_STARTING_APPLICATION_LOCALMODE_HANDLER(QVariant msg);
void EVT_HOME_REQUEST_NOPASS_HANDLER(QVariant msg);
void EVT_ONS_CLOSE_REQUEST_HANDLER(QVariant msg);
void EVT_LOGIN_REQUEST_HANDLER(QVariant msg);
void EVT_ROOT_PROMT_PIN_HANDLER(QVariant msg);
void EVT_ROOT_PROMT_PASSPHRASE_HANDLER(QVariant msg);

#endif // STATE_ID_ROOT_H
