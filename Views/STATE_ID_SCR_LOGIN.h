
#ifndef STATE_ID_SCR_LOGIN_H
#define STATE_ID_SCR_LOGIN_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_LOGIN_Entry(QVariant msg);
void SCR_LOGIN_Exit(QVariant msg);
void EVT_LOGIN_PERFORM_PASSWORD_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_LOGIN_H
