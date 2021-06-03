
#ifndef STATE_ID_SCR_UTXOS_H
#define STATE_ID_SCR_UTXOS_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_UTXOS_Entry(QVariant msg);
void SCR_UTXOS_Exit(QVariant msg);
void EVT_UTXOS_SORT_REQUEST_HANDLER(QVariant msg);
void EVT_UTXOS_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_UTXOS_CONSOLIDATE_REQUEST_HANDLER(QVariant msg);
void EVT_UTXOS_ITEM_SELECTED_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_UTXOS_H
