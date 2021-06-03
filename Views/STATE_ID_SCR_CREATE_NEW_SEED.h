
#ifndef STATE_ID_SCR_CREATE_NEW_SEED_H
#define STATE_ID_SCR_CREATE_NEW_SEED_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_CREATE_NEW_SEED_Entry(QVariant msg);
void SCR_CREATE_NEW_SEED_Exit(QVariant msg);
void EVT_CREATE_NEW_SEED_BACK_HANDLER(QVariant msg);
void EVT_CREATE_NEW_SEED_SUCCEED_HANDLER(QVariant msg);
void EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_CREATE_NEW_SEED_H
