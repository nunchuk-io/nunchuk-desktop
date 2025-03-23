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

Item {
    width: 352
    height: 480
    Column {
        anchors.fill: parent
        spacing: 12
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
        Rectangle {
            width: 350
            height: 452
            QListView {
                id: signerlist
                anchors.fill: parent
                model: newWalletInfo.walletSingleSignerAssigned
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QSandboxKeyDeletage {
                    width: signerlist.width
                    signerData {
                        single_name: singleSigner_name
                        single_type: single_signer_type
                        single_tag: single_signer_tag
                        single_devicetype: single_signer_devicetype
                        single_masterFingerPrint: singleSigner_masterFingerPrint
                        single_account_index: single_signer_account_index
                        single_checked: single_signer_checked
                        single_is_local: single_signer_is_local
                        single_value_key: single_signer_value_key
                        single_derivationPath: singleSigner_derivationPath
                        single_device_cardid: single_signer_device_cardid
                    }
                }
            }
        }
    }
}
