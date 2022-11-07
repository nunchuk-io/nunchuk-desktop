#ifndef COMMONDEFINES_H
#define COMMONDEFINES_H


#define QAPP_WIDTH_EXPECTED   1280
#define QAPP_HEIGHT_EXPECTED  910
#define QAPP_GAP_HEIGHT       120
#define QWINDOW_HEIGHT_PREFER (QAPP_HEIGHT_EXPECTED + QAPP_GAP_HEIGHT)
#define QWINDOW_WIDTH_PREFER  (QAPP_WIDTH_EXPECTED + QAPP_GAP_HEIGHT)

#define POPUP_NUM_MAX   4
#define ALEN(array)     (sizeof(array)/sizeof(*(array)))

#define MAIN_VIEWPORT_QML               "qrc:/main.qml"
#define JS_SCREEN_TRANSITION_FUNCTION   "screen_Transition"
#define JS_POPUPS_TRANSITION_FUNCTION   "load_Popup"
#define JS_TOASTS_TRANSITION_FUNCTION   "load_Toast"


#endif // COMMONDEFINES_H
