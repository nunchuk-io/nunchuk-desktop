import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    id: rootinputpassphrase
    QPassphraseInput {
        anchors.fill: parent
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_INPUT_PASSPHRASE_CLOSE)
        }
        onSendPassphraseClicked: {

        }
    }
}
