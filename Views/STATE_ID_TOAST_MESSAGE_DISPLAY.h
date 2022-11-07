
#ifndef STATE_ID_TOAST_MESSAGE_DISPLAY_H
#define STATE_ID_TOAST_MESSAGE_DISPLAY_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void TOAST_MESSAGE_DISPLAY_Entry(QVariant msg);
void TOAST_MESSAGE_DISPLAY_Exit(QVariant msg);
void EVT_CLOSE_TOAST_MESSAGE_HANDLER(QVariant msg);

#endif // STATE_ID_TOAST_MESSAGE_DISPLAY_H
