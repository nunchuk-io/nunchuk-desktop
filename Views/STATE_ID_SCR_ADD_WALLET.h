
#ifndef STATE_ID_SCR_ADD_WALLET_H
#define STATE_ID_SCR_ADD_WALLET_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_ADD_WALLET_Entry(QVariant msg);
void SCR_ADD_WALLET_Exit(QVariant msg);
void EVT_ADD_WALLET_IMPORT_HANDLER(QVariant msg);
void EVT_ADD_WALLET_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST_HANDLER(QVariant msg);
void EVT_ADD_WALLET_IMPORT_SUCCEED_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_ADD_WALLET_H
