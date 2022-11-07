
#ifndef STATE_ID_SCR_SIGN_IN_H
#define STATE_ID_SCR_SIGN_IN_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_SIGN_IN_Entry(QVariant msg);
void SCR_SIGN_IN_Exit(QVariant msg);
void EVT_SIGN_IN_PASSWORD_REQUEST_HANDLER(QVariant msg);
void EVT_LOGIN_SUCCEEDED_HANDLER(QVariant msg);
void EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_SIGN_IN_H
