
#ifndef STATE_ID_SCR_REVIEW_SHARED_WALLET_H
#define STATE_ID_SCR_REVIEW_SHARED_WALLET_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_REVIEW_SHARED_WALLET_Entry(QVariant msg);
void SCR_REVIEW_SHARED_WALLET_Exit(QVariant msg);
void EVT_REVIEW_SHARED_WALLET_BACK_HANDLER(QVariant msg);
void EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_REVIEW_SHARED_WALLET_H
