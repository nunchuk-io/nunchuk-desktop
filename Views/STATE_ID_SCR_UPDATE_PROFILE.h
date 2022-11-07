
#ifndef STATE_ID_SCR_UPDATE_PROFILE_H
#define STATE_ID_SCR_UPDATE_PROFILE_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_UPDATE_PROFILE_Entry(QVariant msg);
void SCR_UPDATE_PROFILE_Exit(QVariant msg);
void EVT_UPDATE_PROFILE_REQUEST_HANDLER(QVariant msg);
void EVT_UPDATE_PROFILE_BACK_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_UPDATE_PROFILE_H
