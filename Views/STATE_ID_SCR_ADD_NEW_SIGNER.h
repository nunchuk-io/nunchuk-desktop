
#ifndef STATE_ID_SCR_ADD_NEW_SIGNER_H
#define STATE_ID_SCR_ADD_NEW_SIGNER_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_ADD_NEW_SIGNER_Entry(QVariant msg);
void SCR_ADD_NEW_SIGNER_Exit(QVariant msg);
void EVT_ADD_HARDWARE_SIGNER_REQUEST_HANDLER(QVariant msg);
void EVT_ADD_NEW_SIGNER_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED_HANDLER(QVariant msg);
void EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_ADD_NEW_SIGNER_H
