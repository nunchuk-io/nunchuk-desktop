
#ifndef STATE_ID_SCR_INPUT_PASSPHRASE_H
#define STATE_ID_SCR_INPUT_PASSPHRASE_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_INPUT_PASSPHRASE_Entry(QVariant msg);
void SCR_INPUT_PASSPHRASE_Exit(QVariant msg);
void EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE_HANDLER(QVariant msg);
void EVT_INPUT_PASSPHRASE_CLOSE_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_INPUT_PASSPHRASE_H
