import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    Rectangle {
        id: mask
        width: popupWidth
        height: popupHeight
        radius: 8
        visible: false
    }

    Rectangle {
        id: content
        width: popupWidth
        height: popupHeight
        color: "#F1FAFE"
        radius: 8
        anchors.centerIn: parent
        visible: false
    }

    OpacityMask {
        anchors.fill: content
        source: content
        maskSource: mask

        QText {
            opacity: 0.7
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            text: "Add New Wallet"
            color: "#031F2B"
            font.weight: Font.ExtraBold
            font.pixelSize: 24
        }

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION)
            }
        }

        Row {
            id: step
            width: 644
            height: 31
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 84
            }
            QAddStep {
                step: 1
                stepName: "Wallet Configuration"
                currentStep: 2
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 2
                stepName: "Signer Configuration"
                currentStep: 2
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 3
                stepName: "Confirmation"
                currentStep: 2
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            width: 269
            height: 454
            radius: 8
            color: Qt.rgba(255, 255, 255, 0.5)
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 139
            }

            QText {
                anchors {
                    left: parent.left
                    leftMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                text: "Choose from Existing Signers"
                color: "#031F2B"
                font.pixelSize: 14
                font.weight: Font.Bold
            }

            Flickable {
                id: flickerSignerList
                width: 269
                height: 190
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true;//contentHeight > contentDisplay.height
                contentHeight: contentDisplay.height
                ScrollBar.vertical: ScrollBar { active: true }
                anchors {
                    left: parent.left
                    leftMargin: 0
                    top: parent.top
                    topMargin: 45
                }

                Column {
                    id: contentDisplay
                    QListView {
                        id: mastersigners
                        width: 269
                        height: 40*mastersigners.count
                        signal updateState()
                        property var signerSelected: []
                        clip: true
                        interactive: false
                        model: AppModel.masterSignerList
                        delegate: Rectangle {
                            id: delegateMastersigner
                            property bool isSelected: false
                            property string signername: master_signer_name
                            property bool checkedState: (master_signer_checked || delegateMastersigner.isSelected || (mastersigners.signerSelected[index] === undefined ? false : mastersigners.signerSelected[index]))
                            enabled: !master_signer_checked
                            color: delegateMastersigner.checkedState ? Qt.rgba(0, 0, 0, 0.1): "transparent"
                            width: 269
                            height: 40
                            Rectangle {
                                width: 269
                                height: 1
                                color: "#C9DEF1"
                                anchors.bottom: parent.bottom
                            }
                            Row {
                                height: 24
                                spacing: 8
                                anchors {
                                    left: parent.left
                                    leftMargin: 16
                                    verticalCenter: parent.verticalCenter
                                }
                                Image {
                                    width: 24
                                    height: 24
                                    source: delegateMastersigner.checkedState ? "qrc:/Images/Images/SignerChecked.png" : "qrc:/Images/Images/SignerUnChecked.png"
                                }

                                QText {
                                    width: 200
                                    elide: Text.ElideRight
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pixelSize: 14
                                    color: delegateMastersigner.checkedState ? "#9CAEB8" :  "#031F2B"
                                    text: master_signer_name
                                }
                            }

                            Connections {
                                target: mastersigners
                                onUpdateState : {
                                    delegateMastersigner.isSelected = false
                                    mastersigners.signerSelected[index] = false
                                }
                            }

                            Rectangle {
                                width: 63
                                height: 21
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                color: "#C9DEF1"
                                radius: 4
                                visible: master_signer_isSoftware
                                QText {
                                    text: "SOFTWARE"
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 10
                                    anchors.centerIn: parent
                                    color: "#031F2B"
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked:{
                                    console.log("master_signer_need_passphrase ", master_signer_need_passphrase)
                                    if(master_signer_isSoftware && master_signer_need_passphrase && !delegateMastersigner.isSelected){
                                        passphraseModel.visible = true
                                        passphraseModel.signerIndex = index
                                        passphraseModel.signerId = master_signer_id
                                    }
                                    else{
                                        delegateMastersigner.isSelected = !delegateMastersigner.isSelected
                                        mastersigners.signerSelected[index] = delegateMastersigner.isSelected
                                        delegateMastersigner.checkedState = master_signer_checked || delegateMastersigner.isSelected || (mastersigners.signerSelected[index] === undefined ? false : mastersigners.signerSelected[index])
                                    }
                                }
                            }
                        }
                    }

                    QListView {
                        id: remotesigners
                        width: 269
                        height: 40*remotesigners.count
                        signal updateState()
                        property var signerSelected: []
                        clip: true
                        interactive: false
                        model: AppModel.remoteSignerList
                        delegate: Rectangle {
                            id: delegateRemoteSigner
                            property bool isSelected: false
                            property bool checkedState: (single_signer_checked || delegateRemoteSigner.isSelected || (remotesigners.signerSelected[index] === undefined ? false : remotesigners.signerSelected[index]))
                            enabled: !single_signer_checked
                            color: delegateRemoteSigner.checkedState ? Qt.rgba(0, 0, 0, 0.1): "transparent"
                            width: 269
                            height: 40
                            Rectangle {
                                width: 269
                                height: 1
                                color: "#C9DEF1"
                                anchors.bottom: parent.bottom
                            }
                            Row {
                                height: 24
                                spacing: 8
                                anchors {
                                    left: parent.left
                                    leftMargin: 16
                                    verticalCenter: parent.verticalCenter
                                }
                                Image {
                                    width: 24
                                    height: 24
                                    source: delegateRemoteSigner.checkedState ? "qrc:/Images/Images/SignerChecked.png" : "qrc:/Images/Images/SignerUnChecked.png"
                                }
                                QText {
                                    width: 150
                                    elide: Text.ElideRight
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pixelSize: 14
                                    color: delegateRemoteSigner.checkedState  ? "#9CAEB8" :  "#031F2B"
                                    text: singleSigner_name
                                }
                            }

                            Rectangle {
                                width: 63
                                height: 21
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                color: "#C9DEF1"
                                radius: 4
                                QText {
                                    text: "AIR-GAPPED"
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 10
                                    anchors.centerIn: parent
                                    color: "#031F2B"
                                }
                            }

                            Connections {
                                target: remotesigners
                                onUpdateState : {
                                    delegateRemoteSigner.isSelected = false
                                    remotesigners.signerSelected[index] = false
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked:{
                                    delegateRemoteSigner.isSelected = !delegateRemoteSigner.isSelected
                                    remotesigners.signerSelected[index] = delegateRemoteSigner.isSelected
                                    delegateRemoteSigner.checkedState = (single_signer_checked || delegateRemoteSigner.isSelected || (remotesigners.signerSelected[index] === undefined ? false : remotesigners.signerSelected[index]))
                                }
                            }
                        }
                    }
                }
            }

            QButtonIcon {
                id: assignTowallet
                width: 161
                height: 32
                iconInRight: true
                type: eSECOND
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 257
                }
                label: "ASSIGN TO WALLET"
                icons: ["d_chevron_24px_031F2B.png","d_chevron_24px_9CAEB8.png","d_chevron_24px_F1FAFE.png","d_chevron_24px_F1FAFE.png"]
                onButtonClicked: {
                    // Master signers
                    var masterIndexSelected = [];
                    for(var i = 0; i < mastersigners.signerSelected.length; i++){
                        if(true === mastersigners.signerSelected[i]){ masterIndexSelected.push(i); }
                    }
                    if(masterIndexSelected.length > 0){
                        QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER, masterIndexSelected)
                        mastersigners.updateState()
                    }

                    // Remote signers
                    var remoteIndexSelected = [];
                    for(var j = 0; j < remotesigners.signerSelected.length; j++){
                        if(true === remotesigners.signerSelected[j]){ remoteIndexSelected.push(j); }
                    }
                    if(remoteIndexSelected.length > 0){
                        QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER, remoteIndexSelected)
                        remotesigners.updateState()
                    }
                }
            }

            QText {
                anchors {
                    left: parent.left
                    leftMargin: 16
                    top: parent.top
                    topMargin: 337
                }
                text: "Or Add a New Signer"
                font.family: "Montserrat"
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.pixelSize: 14
            }

            QButtonMedium {
                id: addNewWallet
                width: 160
                height: 32
                radius: 12
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 400
                }
                type: eOUTLINE_NORMAL
                label: "ADD NEW SIGNER"
                fontPixelSize: 10
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ADD_SIGNER_TO_WALLET_REQUEST)
                }
            }
            QText {
                text: "Airgapped signers supported"
                anchors.horizontalCenter: addNewWallet.horizontalCenter
                anchors.bottom: addNewWallet.top
                anchors.bottomMargin: 4
                font.pixelSize: 10
                font.family: "Lato"
                color: "#35ABEE"
                visible: false
            }
        }

        QText {
            anchors {
                left: parent.left
                leftMargin: 349
                top: parent.top
                topMargin: 147
            }
            font.pixelSize: 16
            font.weight: Font.Bold
            color: "#031F2B"
            text: "Signer Configuration"
        }
        Row {
            anchors {
                left: parent.left
                leftMargin: 349
                top: parent.top
                topMargin: 184
            }
            width: 187
            height: 21
            spacing: 4
            QText {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 10
                font.weight: Font.DemiBold
                color: "#031F2B"
                text: "NUMBER OF SIGNERS:"
            }
            QText {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                font.weight: Font.DemiBold
                color: "#35ABEE"
                text: AppModel.newWalletInfo.walletN
            }
            QText {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
                color: "#323E4A"
                text: "(Maximum 15)"
            }
        }

        QListView {
            id: signerAssigned
            width: 418
            height: 195
            spacing: 8
            clip: true
            anchors {
                left: parent.left
                leftMargin: 341
                top: parent.top
                topMargin: 216
            }
            ScrollBar.vertical: ScrollBar { active: true }
            model: AppModel.newWalletInfo.walletSingleSignerAssigned
            delegate: Item {
                id: signerAssigneddlg
                signal updateSelectedState()
                width: 418
                height: single_signer_isRemote ? 73 : 53
                Rectangle {
                    id: rect
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 415
                    height: single_signer_isRemote ? 70 : 50
                    color: Qt.rgba(255, 255, 255)
                }
                DropShadow {
                    anchors.fill: rect
                    verticalOffset: 2
                    cached: true
                    radius: 8
                    samples: 16
                    color: Qt.rgba(0, 0, 0, 0.15)
                    source: rect
                }

                Rectangle {
                    id: indicatiorValid
                    width: 4
                    height: parent.height - 18
                    radius: 2
                    color: "#C9DEF1"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: rect.verticalCenter
                    }
                }

                Column {
                    id: txt
                    width: 350
                    height: single_signer_isRemote ? 53 : 37
                    anchors {
                        left: parent.left
                        leftMargin: 28
                        verticalCenter: indicatiorValid.verticalCenter
                    }

                    Item {
                        width: parent.width
                        height: 21

                        QText {
                            width: parent.width - 63
                            height: 21
                            verticalAlignment: Text.AlignVCenter
                            text: singleSigner_name
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            color: "#031F2B"
                            anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            width: 63
                            height: 21
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            color: "#C9DEF1"
                            visible: single_signer_isRemote
                            radius: 4
                            QText {
                                text: "AIR-GAPPED"
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                    }

                    QText {
                        width: parent.width
                        height: 16
                        font.pixelSize: 12
                        color: "#031F2B"
                        font.family: "Lato"
                        text: "XFP: " + singleSigner_masterFingerPrint
                        font.capitalization: Font.AllUppercase
                    }

                    QText {
                        width: parent.width
                        height: single_signer_isRemote ? 16 : 0
                        visible: single_signer_isRemote
                        font.pixelSize: 10
                        color: "#839096"
                        font.family: "Lato"
                        text: "BIP32 Path: " + singleSigner_derivationPath
                    }
                }

                QImage {
                    anchors {
                        right: parent.right
                        rightMargin: 20
                        verticalCenter: indicatiorValid.verticalCenter
                    }
                    source: "qrc:/Images/Images/Delete.png"
                    MouseArea {
                        id: dlgRightMouse
                        anchors.fill: parent
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER, index)
                        }
                    }
                }
            }
        }

        Rectangle {
            width: 419
            height: 132
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 111
            }
            color: Qt.rgba(201, 222, 241, 0.5)

            Row {
                id: requiredNumberInfo
                height: 21
                anchors {
                    left: parent.left
                    leftMargin: 8
                    top: parent.top
                    topMargin: 14
                }
                spacing: 4
                QText {
                    height: 21
                    font.pixelSize: 10
                    font.weight: Font.Bold
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#323E4A"
                    text: "REQUIRED SIGNATURES:"
                }
                QText {
                    height: 21
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#35ABEE"
                    text: AppModel.newWalletInfo.walletM
                }
                QText {
                    height: 21
                    font.pixelSize: 14
                    font.weight: Font.DemiBold
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#839096"
                    text: "of"
                }
                QText {
                    height: 21
                    font.pixelSize: 14
                    font.weight: Font.DemiBold
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#839096"
                    text: signerAssigned.count
                }
            }

            QText {
                anchors {
                    left: parent.left
                    leftMargin: 8
                    top: requiredNumberInfo.bottom
                }
                text: "Number of Signatures Required to Unlock Funds"
                font.pixelSize: 12
                font.family: "Lato"
                color: "#031F2B"
            }

            Rectangle {
                width: 120
                height: 48
                anchors {
                    left: parent.left
                    leftMargin: 8
                    top: parent.top
                    topMargin: 63
                }
                color: Qt.rgba(255, 255, 255, 0.5)
                border.color: "#C9DEF1"
                radius: 4

                Row {
                    height: 21
                    anchors.centerIn: parent
                    spacing: 10

                    Rectangle {
                        width: 21
                        height: 21
                        radius: 21
                        color: minus.pressed ? "#031F2B" : "#F1FAFE"
                        border.color: minus.pressed ?"#F1FAFE" : "#031F2B"
                        Rectangle {
                            width: 8
                            height: 2
                            color: minus.pressed ?"#F1FAFE" : "#031F2B"
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            id: minus
                            anchors.fill: parent
                            onClicked: {
                                if(AppModel.newWalletInfo.walletM > 0){
                                    AppModel.newWalletInfo.walletM--;
                                }
                            }
                        }
                    }

                    Item {
                        width: 21
                        height: 21
                        QText {
                            anchors.centerIn: parent
                            font.pixelSize: 16
                            font.weight: Font.DemiBold
                            color: "#031F2B"
                            text: AppModel.newWalletInfo.walletM
                        }
                    }

                    Rectangle {
                        width: 21
                        height: 21
                        radius: 21
                        color: plusbtn.pressed ? "#031F2B" : "#F1FAFE"
                        border.color: plusbtn.pressed ?"#F1FAFE" : "#031F2B"
                        Rectangle {
                            width: 8
                            height: 2
                            color: plusbtn.pressed ?"#F1FAFE" : "#031F2B"
                            anchors.centerIn: parent
                        }
                        Rectangle {
                            width: 2
                            height: 8
                            color: plusbtn.pressed ?"#F1FAFE" : "#031F2B"
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            id: plusbtn
                            anchors.fill: parent
                            onClicked: {
                                if(AppModel.newWalletInfo.walletM < signerAssigned.count){
                                    AppModel.newWalletInfo.walletM++;
                                }
                            }
                        }
                    }
                }
            }
        }

        QButtonTextLink {
            width: 203
            height: 24
            label: "BACK TO PREVIOUS"
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 43
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK)
            }
        }

        QTextButton {
            width: 120
            height: 48
            label.text: "Cancel"
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                left: parent.left
                leftMargin: 424
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION)
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: "Next: Review Wallet"
            label.font.pixelSize: 16
            type: eTypeA
            enabled: (AppModel.newWalletInfo.walletM > 0) && (signerAssigned.count > 0) && (AppModel.newWalletInfo.walletM <= signerAssigned.count)
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW)
            }
        }

        QPassphraseInput {
            id: passphraseModel
            property int    signerIndex: -1
            property string signerId: ""
            color: Qt.rgba(255, 255, 255, 0.7)
            anchors.fill: parent
            visible: false
            onCloseClicked: {
                passphraseModel.visible = false
                passphraseModel.valid = true
                passphraseModel.errorText = ""
            }
            onSendPassphraseClicked: {
                passphraseModel.valid = true
                passphraseModel.errorText = ""
                var signerObj = { "passphraseInput"    : passphrase,
                                  "mastersignerId"     : passphraseModel.signerId};
                QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE, signerObj)
                if(AppModel.newWalletInfo.warningMessage.type === EWARNING.NONE_MSG){
                    mastersigners.contentItem.children[passphraseModel.signerIndex].isSelected = true
                    mastersigners.signerSelected[passphraseModel.signerIndex] = true
                    passphraseModel.visible = false
                    passphraseModel.textInputted = ""
                    console.log(mastersigners.signerSelected)
                }
                else{
                    passphraseModel.valid = false
                    passphraseModel.errorText = "Something wrong please try again"
                }
            }
        }
    }
}
