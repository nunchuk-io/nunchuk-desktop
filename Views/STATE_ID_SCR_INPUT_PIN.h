
#ifndef STATE_ID_SCR_INPUT_PIN_H
#define STATE_ID_SCR_INPUT_PIN_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_INPUT_PIN_Entry(QVariant msg);
void SCR_INPUT_PIN_Exit(QVariant msg);
void EVT_INPUT_PIN_SEND_PIN_HANDLER(QVariant msg);
void EVT_INPUT_PIN_CLOSE_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_INPUT_PIN_H
