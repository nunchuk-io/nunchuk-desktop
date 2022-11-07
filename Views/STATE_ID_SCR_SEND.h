
#ifndef STATE_ID_SCR_SEND_H
#define STATE_ID_SCR_SEND_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_SEND_Entry(QVariant msg);
void SCR_SEND_Exit(QVariant msg);
void EVT_SEND_ADD_DESTINATION_REQUEST_HANDLER(QVariant msg);
void EVT_SEND_CREATE_TRANSACTION_REQUEST_HANDLER(QVariant msg);
void EVT_SEND_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_SEND_CREATE_TRANSACTION_SUCCEED_HANDLER(QVariant msg);
void EVT_SEND_BACK_HOME_SHARED_WL_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_SEND_H
