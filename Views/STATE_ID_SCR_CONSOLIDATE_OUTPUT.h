
#ifndef STATE_ID_SCR_CONSOLIDATE_OUTPUT_H
#define STATE_ID_SCR_CONSOLIDATE_OUTPUT_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_CONSOLIDATE_Entry(QVariant msg);
void SCR_CONSOLIDATE_Exit(QVariant msg);
void EVT_CONSOLIDATE_MAKE_TRANSACTION_HANDLER(QVariant msg);
void EVT_CONSOLIDATE_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_CONSOLIDATE_OUTPUT_H
