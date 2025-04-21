import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Signers"
import "../../customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Column {
    width: 352
    spacing: 16
    readonly property var tmpColors: [
        {colorStr: "#9EC063",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#2F466C",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#FAA077",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#1C652D",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#B4DCFF",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#7E519B",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#FDD95C",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#595959",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#D38FFF",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#CF4018",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#A66800",           url: "qrc:/Images/Images/User-dark.svg" },
    ]

    Item {
        height: 32
        width: parent.width
        Row {
            height: 32
            spacing: 8
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/mulitsig-dark.svg"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                width: 44
                height: 20
                anchors.verticalCenter: parent.verticalCenter
                text: STR.STR_QML_015
                font.weight: Font.Bold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
        Row {
            height: 32
            spacing: -4
            anchors {
                right: parent.right
                rightMargin: 9
            }
            visible: sandbox.url !== ""
            Repeater {
                model: Math.min(3, sandbox.userCount)
                Item {
                    width: 32
                    height: 32
                    property var colorData: tmpColors[index]
                    Rectangle {
                        border.width: 1
                        border.color: "#DEDEDE"
                        color: colorData.colorStr
                        radius: 16
                        width: 32
                        height: 32
                        visible: sandbox.userCount === 3
                        QIcon {
                            iconSize: 18
                            anchors.centerIn: parent
                            source: colorData.url
                        }
                    }
                    Rectangle {
                        border.width: 1
                        border.color: "#DEDEDE"
                        color: index === 2 ? "#031F2B" : colorData.colorStr
                        radius: 16
                        width: 32
                        height: 32
                        visible: sandbox.userCount !== 3
                        QIcon {
                            iconSize: 18
                            anchors.centerIn: parent
                            source: colorData.url
                            visible: index != 2
                        }
                        QLato {
                            width: 24
                            height: 18
                            anchors.centerIn: parent
                            text: qsTr("%1").arg(sandbox.userCount - 2)
                            font.weight: Font.Bold
                            color: "#FFFFFF"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            visible: index === 2
                        }
                    }
                }
            }
        }
    }
    Repeater {
        id: signers
        model: sandbox.groupKeys
        QAddGroupKey {
            signerData {
                single_name: modelData.singleSigner_name
                single_type: modelData.single_signer_type
                single_tag: modelData.single_signer_tag
                single_devicetype: modelData.single_signer_devicetype
                single_masterFingerPrint: modelData.singleSigner_masterFingerPrint
                single_account_index: modelData.single_signer_account_index
                single_checked: modelData.single_signer_checked
                single_is_local: modelData.single_signer_is_local
                single_value_key: modelData.single_signer_value_key
                single_derivationPath: modelData.singleSigner_derivationPath
                single_device_cardid: modelData.single_signer_device_cardid
                single_isOccupied: modelData.single_signer_isOccupied
                single_isReplaced: modelData.single_signer_isReplaced
                single_keyReplaced: modelData.single_signer_keyReplaced
            }

            onHardwareClicked: {
                sandbox.requestAddOrRepaceKey({ type: "add-key-shared-group-wallet", idx: index })
            }
            onRemoveClicked: {
                _confirmRemoveKey.idx = idx
                _confirmRemoveKey.open()
            }
            onRecoverKeyClick: {
                var obj = {
                    type: "add-key-shared-group-wallet",
                    idx: index,
                    fingerPrint: xfp
                }
                sandbox.requestAddOrRepaceKey(obj)
            }
            onBip32PathClick: {
                editBip32Path.clearError()
                editBip32Path.isShowListDevice = false
                editBip32Path.signerData = signerData
                editBip32Path.idx = index
                editBip32Path.xfp = xfp
                editBip32Path.open()
            }
        }
    }
}
