#ifndef QCOMMONENUMS_H
#define QCOMMONENUMS_H

#include <qglobal.h>

enum class LAYER : char
{
    LAYER_BASE      = 0x1,
    LAYER_SCREEN    = 0x2,
    LAYER_ONSCREEN  = 0x3,
    LAYER_POPUP     = 0x4,
    LAYER_TOAST     = 0x5,
};

enum class LIMIT : int
{
    NONE = 0,
    SEC_1 = 1000,
    SEC_2 = 2000,
    SEC_3 = 3000,
    SEC_4 = 4000,
    SEC_5 = 5000,
    SEC_6 = 6000,
    SEC_7 = 7000,
    SEC_8 = 8000,
    SEC_9 = 9000,
    SEC_10 = 10000,
};

enum class VIEWER_INDEX : uint
{
    VIEWER_1ST  = 0x1,
    VIEWER_2ND  = 0x2,
    VIEWER_3RD  = 0x3,
    VIEWER_4TH  = 0x4,
    VIEWER_5TH  = 0x4,
};

#endif // QCOMMONENUMS_H
