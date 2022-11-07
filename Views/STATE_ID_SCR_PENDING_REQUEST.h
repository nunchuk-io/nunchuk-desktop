
#ifndef STATE_ID_SCR_PENDING_REQUEST_H
#define STATE_ID_SCR_PENDING_REQUEST_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_PENDING_REQUEST_Entry(QVariant msg);
void SCR_PENDING_REQUEST_Exit(QVariant msg);
void EVT_PENDING_REQUEST_ACCEPT_HANDLER(QVariant msg);
void EVT_PENDING_REQUEST_IGNORE_HANDLER(QVariant msg);
void EVT_PENDING_REQUEST_BACK_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_PENDING_REQUEST_H
