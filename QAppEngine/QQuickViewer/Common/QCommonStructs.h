#ifndef QCOMMONSTRUCTS_H
#define QCOMMONSTRUCTS_H
#include <QString>
#include <QVariant>
#include <stddef.h>
#include "QCommonDefines.h"
#include "QCommonEnums.h"

typedef struct popup_data
{
    uint        id;
    LIMIT       duration;
    QString     QmlPath;
    QVariant    msg;
    void        (*funcEntry)(QVariant msg);
    void        (*funcExit)(QVariant msg);
} POPUP_DATA;

typedef struct st_application_states
{
    uint        id;
    void        (*funcEntry)(QVariant msg);
    void        (*funcExit)(QVariant msg);
    LAYER       layerbase;
    LIMIT       duration;
    QString     QmlPath;
} APPLICATION_STATE;

typedef struct _st_state_trigger
{
    uint                    evt;
    void                    (*func)(QVariant msg);
    const APPLICATION_STATE *trans;
}STATE_TRIGGER;

typedef struct _st_state_system
{
    uint                    id;
    const STATE_TRIGGER     *trigger;
    uint                    size;
    const APPLICATION_STATE *state;
}STATE_SYSTEM;

#endif // QCOMMONSTRUCTS_H
