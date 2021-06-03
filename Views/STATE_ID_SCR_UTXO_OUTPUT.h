
#ifndef STATE_ID_SCR_UTXO_OUTPUT_H
#define STATE_ID_SCR_UTXO_OUTPUT_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_UTXO_OUTPUT_Entry(QVariant msg);
void SCR_UTXO_OUTPUT_Exit(QVariant msg);
void EVT_UTXO_INFO_CHECKOUT_TX_RELATED_HANDLER(QVariant msg);
void EVT_UTXO_INFO_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_UTXO_INFO_VIEW_TX_RELATED_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_UTXO_OUTPUT_H
