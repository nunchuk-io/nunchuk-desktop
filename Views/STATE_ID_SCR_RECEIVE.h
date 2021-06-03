
#ifndef STATE_ID_SCR_RECEIVE_H
#define STATE_ID_SCR_RECEIVE_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_RECEIVE_Entry(QVariant msg);
void SCR_RECEIVE_Exit(QVariant msg);
void EVT_BTN_GEN_NEW_ADDRESS_HANDLER(QVariant msg);
void EVT_RECEIVE_COPY_ADDRESS_HANDLER(QVariant msg);
void EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS_HANDLER(QVariant msg);
void EVT_RECEIVE_DISPLAY_ADDRESS_HANDLER(QVariant msg);
void EVT_RECEIVE_ADDRESS_BALANCE_HANDLER(QVariant msg);
void EVT_RECEIVE_BACK_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_RECEIVE_H
