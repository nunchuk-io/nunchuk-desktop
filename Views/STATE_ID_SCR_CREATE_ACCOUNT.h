
#ifndef STATE_ID_SCR_CREATE_ACCOUNT_H
#define STATE_ID_SCR_CREATE_ACCOUNT_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_CREATE_ACCOUNT_Entry(QVariant msg);
void SCR_CREATE_ACCOUNT_Exit(QVariant msg);
void EVT_CREATE_ACCOUNT_REQUEST_HANDLER(QVariant msg);
void EVT_CREATE_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);
void EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);
void EVT_CHANGE_PASSWORD_SHOW_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_CREATE_ACCOUNT_H
