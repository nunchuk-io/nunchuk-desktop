import QtQuick
import DataPool 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QSetupSecurityQuestions {
        onCloseClicked: {
            if (submitting || keyRecovery.securityQuestionUpdateInProgress) {
                return
            }
            ServiceSetting.servicesTag.keyRecovery.resetSecurityQuestionUpdate()
            closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        }

        onPrevClicked: {
            if (submitting || keyRecovery.securityQuestionUpdateInProgress) {
                return
            }
            ServiceSetting.servicesTag.keyRecovery.resetSecurityQuestionUpdate()
            closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        }
    }
}
