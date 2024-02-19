import QtQuick 2.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QSetupSecurityQuestions {
        onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)

        onPrevClicked: {
            closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        }
    }
}
