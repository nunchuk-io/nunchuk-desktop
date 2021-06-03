
#ifndef STATE_ID_SCR_EXPORT_WALLET_H
#define STATE_ID_SCR_EXPORT_WALLET_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_EXPORT_WALLET_Entry(QVariant msg);
void SCR_EXPORT_WALLET_Exit(QVariant msg);
void EVT_EXPORT_WALLET_HANDLER(QVariant msg);
void EVT_EXPORT_WALLET_DESCRIPTORS_HANDLER(QVariant msg);
void EVT_EXPORT_WALLET_TO_COLDCARD_HANDLER(QVariant msg);
void EVT_EXPORT_WALLET_BACK_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_EXPORT_WALLET_H
