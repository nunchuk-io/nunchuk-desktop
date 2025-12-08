import QtQuick 2.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property var draftWallet: GroupWallet.qIsByzantine ? GroupWallet : UserWallet
    property var stateScreen: SignerManagement
    readonly property int hardwareType: draftWallet.qAddHardware
    readonly property int eADD_INFORMATION: 0
    readonly property int eADD_REFRESH_DEVICE: 1
    readonly property int eADD_LOADING: 2
    readonly property int eADD_SUCCESSFULLY: 3
    readonly property int eADD_ERROR: 4
    readonly property int eADD_VIA_FILE: 5
    readonly property int eADD_VIA_QR: 6
    QSupportedKeys {
        id: supportedKeys
    }
    readonly property var map_hardware: supportedKeys.allKeys
    readonly property var hardware: map_hardware.find(function(e) {if (e.type === hardwareType) return true; else return false})
    readonly property string _HARDWARE: hardware ? hardware.name : ""
    readonly property string _HARDWARE_TYPE: hardware ? hardware.device_type : ""
    readonly property string _HARDWARE_TAG: hardware ? hardware.tag : ""

    QPopupInfo{
        id:_info1
        contentText: STR.STR_QML_932
        usingMin: true
    }

    Connections {
        target: draftWallet
        onAddHardwareAlert: {
            if (_item.visible) {
                _info1.open()
            }
        }
    }
}
