
#ifndef STATE_ID_SCR_ONLINE_ADD_CONTACTS_H
#define STATE_ID_SCR_ONLINE_ADD_CONTACTS_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_ONLINE_ADD_CONTACTS_Entry(QVariant msg);
void SCR_ONLINE_ADD_CONTACTS_Exit(QVariant msg);
void EVT_ONLINE_ADD_CONTACTS_SEARCH_ID_HANDLER(QVariant msg);
void EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION_HANDLER(QVariant msg);
void EVT_ONLINE_ADD_CONTACTS_BACK_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_ONLINE_ADD_CONTACTS_H
