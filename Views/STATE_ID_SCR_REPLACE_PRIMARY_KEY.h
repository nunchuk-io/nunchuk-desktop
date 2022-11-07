
#ifndef STATE_ID_SCR_REPLACE_PRIMARY_KEY_H
#define STATE_ID_SCR_REPLACE_PRIMARY_KEY_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_REPLACE_PRIMARY_KEY_Entry(QVariant msg);
void SCR_REPLACE_PRIMARY_KEY_Exit(QVariant msg);
void EVT_REPLACE_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_REPLACE_PRIMARY_KEY_H
