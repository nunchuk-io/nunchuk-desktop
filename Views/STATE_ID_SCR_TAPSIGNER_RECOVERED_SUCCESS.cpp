
#include "STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS.h"
#include "AppModel.h"
#include "localization/STR_CPP.h"

void SCR_TAPSIGNER_RECOVERED_SUCCESS_Entry(QVariant msg) {

}

void SCR_TAPSIGNER_RECOVERED_SUCCESS_Exit(QVariant msg) {

}

void EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->showToast(0,
                                    STR_CPP_110,
                                    EWARNING::WarningType::SUCCESS_MSG,
                                    STR_CPP_110);
}

