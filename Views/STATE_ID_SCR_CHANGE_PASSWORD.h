
#ifndef STATE_ID_SCR_CHANGE_PASSWORD_H
#define STATE_ID_SCR_CHANGE_PASSWORD_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_CHANGE_PASSWORD_Entry(QVariant msg);
void SCR_CHANGE_PASSWORD_Exit(QVariant msg);
void EVT_CHANGE_PASSWORD_REQUEST_HANDLER(QVariant msg);
void EVT_CHANGE_PASSWORD_BACK_HANDLER(QVariant msg);
void EVT_CHANGE_PASSWORD_SUCCESS_HANDLED(QVariant msg);

#endif // STATE_ID_SCR_CHANGE_PASSWORD_H
