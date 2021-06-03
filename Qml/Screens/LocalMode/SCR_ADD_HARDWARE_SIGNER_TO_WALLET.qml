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
    id: rootAddsignerToWallet
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
            width: 163
            height: 27
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            text: "Add New Signer"
            color: "#031F2B"
            font.family: "Montserrat"
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
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK)
            }
        }

        Loader {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 60
            }
            sourceComponent: tabselect.currentIndex == 0 ? masterNotification : singleNotification
        }

        Component {
            id: masterNotification
            QNotification {
                visible: AppModel.deviceList.warningMessage.type === EWARNING.EXCEPTION_MSG
                label: AppModel.deviceList.warningMessage.contentDisplay
                currentStatus: AppModel.deviceList.warningMessage.type
                onCloseRequest: AppModel.deviceList.warningMessage.type = EWARNING.NONE_MSG
            }
        }

        Component {
            id: singleNotification
            QNotification {
                visible: AppModel.singleSignerInfo.warningMessage.type === EWARNING.EXCEPTION_MSG
                label: AppModel.singleSignerInfo.warningMessage.contentDisplay
                currentStatus: AppModel.singleSignerInfo.warningMessage.type
                onCloseRequest: AppModel.singleSignerInfo.warningMessage.type = EWARNING.NONE_MSG
            }
        }

        QText {
            id: titname
            width: 500
            anchors {
                left: parent.left
                leftMargin: 40
                top: notification.bottom
                topMargin: notification.visible ? 10 : 16
            }
            text: "Start by giving your signer a unique name."
            color: "#031F2B"
            font.weight: Font.DemiBold
            font.family: "Montserrat"
            font.pixelSize: 14
        }

        QTextInputBox {
            id: signerName
            mode: eEDIT_MODE
            width: 532
            heightMin: 56
            placeholder.text: "Signer Name"
            maximumLength: 106
            anchors {
                left: parent.left
                leftMargin: 40
                top: titname.bottom
                topMargin: notification.visible ? 10 : 16
            }
            color: Qt.rgba(255, 255, 255, 0.5)
            border.color: "#C9DEF1"
            enabled: !pinModel.visible
        }

        TabView {
            id: tabselect
            width: 800
            height: 505
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 197
            }
            onCurrentIndexChanged: {
                AppModel.deviceList.warningMessage.type = EWARNING.NONE_MSG
                AppModel.singleSignerInfo.warningMessage.type = EWARNING.NONE_MSG
            }

            Tab {
                title: "Wired Signer"
                Item {
                    id: tabmastersigner
                    QText {
                        id: titguide
                        width: 374
                        height: 21
                        anchors {
                            left: parent.left
                            leftMargin: 40
                            top: parent.top
                            topMargin: 24
                        }
                        text: "Please plug in, unlock your signer device and select it from the list below."
                        color: "#031F2B"
                        font.weight: Font.DemiBold
                        font.family: "Montserrat"
                        font.pixelSize: 14
                    }

                    QImage {
                        id:nodevice
                        visible: !devicelist.visible
                        anchors {
                            left: parent.left
                            leftMargin: 40
                            top: titguide.bottom
                            topMargin: 16
                        }
                        source: "qrc:/Images/Images/Signer_Level2.png"
                    }

                    QListView {
                        id: devicelist
                        property bool needPin: false
                        visible: devicelist.count
                        width: 342
                        height: Math.min(230, (devicelist.count*44) + ((devicelist.count-1)*8))
                        model: AppModel.deviceList
                        anchors {
                            left: parent.left
                            leftMargin: 40
                            top: titguide.bottom
                            topMargin: 16
                        }
                        spacing: 8
                        currentIndex: (devicelist.count) == 1 && AppModel.deviceList.containsAddable? 0 : -1
                        clip: true
                        interactive : devicelist.count > 3
                        ScrollBar.vertical: ScrollBar { active: true }
                        delegate: Item {
                            width: 342
                            height: 44
                            Rectangle {
                                id: rect
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: parent.width - 2
                                height: 40
                                color: Qt.rgba(255, 255, 255)

                                Rectangle {
                                    visible: index == devicelist.currentIndex
                                    width: 8
                                    height: parent.height
                                    color: "#F6D65D"
                                }

                                Image {
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                    source: index == devicelist.currentIndex ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
                                }

                                Column {
                                    width: 290
                                    height: 37
                                    anchors {
                                        left: parent.left
                                        leftMargin: 48
                                        verticalCenter: parent.verticalCenter
                                    }
                                    QText {
                                        width: parent.width
                                        height: 21
                                        font.family: "Montserrat"
                                        font.pixelSize: 14
                                        color: "#031F2B"
                                        font.weight: Font.DemiBold
                                        text: device_type
                                    }

                                    QText {
                                        width: parent.width
                                        height: 16
                                        font.family: "Lato"
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        text: "XFP: " + device_master_fingerprint
                                        font.capitalization: Font.AllUppercase
                                    }
                                }
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
                                anchors.fill: parent
                                visible: !device_usable_to_add
                                color: Qt.rgba(255, 255, 255, 0.5)
                                Rectangle {
                                    width: 56
                                    height: 21
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    anchors.rightMargin: 9
                                    color: "#C9DEF1"
                                    radius: 4
                                    QText {
                                        text: "ADDED"
                                        font.pixelSize: 10
                                        font.weight: Font.ExtraBold
                                        font.family: "Lato"
                                        color: "#F1FAFE"
                                        anchors.centerIn: parent
                                    }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                enabled: device_usable_to_add
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    devicelist.currentIndex = index
                                    devicelist.needPin = device_needs_pin_sent
                                    console.log("device_needs_pin_sent ", device_needs_pin_sent)
                                }
                            }

                            Component.onCompleted: {
                                if(0 === devicelist.currentIndex && index === devicelist.currentIndex){
                                    devicelist.currentIndex = index
                                    devicelist.needPin = device_needs_pin_sent
                                }
                            }
                        }
                    }

                    QRefreshButton {
                        width: 160
                        height: 32
                        label: "Refresh Devices"
                        fontPixelSize: 14
                        anchors {
                            left: parent.left
                            leftMargin: 40
                            top: titguide.bottom
                            topMargin: 32 + (devicelist.count > 0 ? ( devicelist.height) : nodevice.height)
                        }
                        onButtonClicked: {
                            AppModel.deviceList.warningMessage.type = EWARNING.NONE_MSG
                            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH)
                        }
                    }

                    QTextButton {
                        width: 150
                        height: 48
                        label.text: "Add Signer"
                        label.font.pixelSize: 16
                        label.font.family: "Lato"
                        type: eTypeA
                        enabled: (signerName.textOutput !== "") && (devicelist.currentIndex !== -1)
                        anchors {
                            right: parent.right
                            rightMargin: 40
                            bottom: parent.bottom
                            bottomMargin: 40
                        }
                        onButtonClicked: {
                            if(devicelist.currentIndex !== -1){
                                tabmastersigner.startAddSigner()
                            }
                        }
                    }

                    function startAddSigner(){
                        var masterSignerObj = { "signerNameInputted"    : signerName.textOutput,
                                                "deviceIndexSelected"   : devicelist.currentIndex };
                        if(devicelist.needPin){
                            pinModel.visible = true
                            pinModel.signerName = signerName.textOutput
                            pinModel.signerIndex = devicelist.currentIndex
                            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN, masterSignerObj)
                        }
                        else{
                            stateAddSigner.visible = true
                            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER, masterSignerObj)
                        }
                    }
                }
            }

            Tab {
                title: "Advanced: Air-Gapped Signer"
                Item {
                    id: tabremoteSigner
                    QText {
                        anchors {
                            left: parent.left
                            leftMargin: 40
                            top: parent.top
                            topMargin: 24
                        }
                        text: "Add Air-Gapped Signer manually by entering your signer’s spec."
                        color: "#323E4A"
                        font.pixelSize: 16
                    }

                    Flickable {
                        id: signerinforInput
                        width: 532
                        height: Math.min(300, contentHeight)
                        flickableDirection: Flickable.VerticalFlick
                        clip: true
                        interactive: signerinforInput.height < signerinforInput.contentHeight
                        contentHeight: xpubOrPubl.height + bip32.height + master_key_fgp.height + 80
                        ScrollBar.vertical: ScrollBar { active: true }
                        anchors {
                            left: parent.left
                            leftMargin: 40
                            top: parent.top
                            topMargin: 61
                        }

                        QTextInputBox {
                            id: xpubOrPubl
                            width: 532
                            heightMin: 56
                            mode: eEDIT_MODE
                            placeholder.text: AppModel.newWalletInfo.walletEscrow ? "XPUB or Public Key" : "XPUB"
                            color: Qt.rgba(255, 255, 255, 0.5)
                            border.color: "#C9DEF1"
                            validInput: AppModel.newWalletInfo.walletEscrow ? (AppModel.singleSignerInfo.signerPublickey !== "false")
                                                                            : (AppModel.singleSignerInfo.signerXpub !== "false")
                            errorText.text: "The data inputted is not valid"
                            onTypingFinished: AppModel.newWalletInfo.walletEscrow ? (AppModel.singleSignerInfo.signerPublickey = "")
                                                                                  : (AppModel.singleSignerInfo.signerXpub = "")
                        }

                        QText {
                            id: asssitXpub
                            anchors.top: xpubOrPubl.bottom
                            visible: xpubOrPubl.validInput
                            text: "You can also enter the Public Key if used for Escrow Wallets"
                            color: "#839096"
                            font.family: "Lato"
                            font.pixelSize: 12
                        }

                        QTextInputBox {
                            id: bip32
                            width: 532
                            heightMin: 56
                            mode: eEDIT_MODE
                            placeholder.text: "BIP32 Path" + (bip32.textOutput !== "" ? "" : !bip32.textActiveFocus ? " (e.g. \"m/48h/0h/1h\")" : "")
                            anchors {
                                top: xpubOrPubl.bottom
                                topMargin: 32
                            }
                            color: Qt.rgba(255, 255, 255, 0.5)
                            border.color: "#C9DEF1"
                            validInput: (AppModel.singleSignerInfo.signerDerivationPath !== "false")
                            errorText.text: "The data inputted is not valid"
                            onTypingFinished: AppModel.singleSignerInfo.signerDerivationPath = ""
                        }

                        QTextInputBox {
                            id: master_key_fgp
                            width: 532
                            heightMin: 56
                            mode: eEDIT_MODE
                            placeholder.text: "Master Key Fingerprint"
                            anchors {
                                top: bip32.bottom
                                topMargin: 24
                            }
                            color: Qt.rgba(255, 255, 255, 0.5)
                            border.color: "#C9DEF1"
                            validInput: (AppModel.singleSignerInfo.signerMasterFingerPrint !== "false")
                            errorText.text: "The data inputted is not valid"
                            onTypingFinished: AppModel.singleSignerInfo.signerMasterFingerPrint = ""
                        }

                        Connections {
                            target: qrcameraLoader.item
                            onScanFinished: {
                                if(count > 0){
                                    var json = result[0]
                                    console.log(json)
                                    var obj = JSON.parse(json);
                                    if('xpub' in obj){ // Multisig
                                        xpubOrPubl.textOutput = obj.xpub
                                        master_key_fgp.textOutput = obj.xfp
                                        bip32.textOutput = obj.path
                                    }
                                    else if('ExtPubKey' in obj){ // Singlesig | Watch only wallet
                                        xpubOrPubl.textOutput = obj.ExtPubKey
                                        master_key_fgp.textOutput = obj.MasterFingerprint
                                        if(path.includes("m/")){
                                            bip32.textOutput = obj.AccountKeyPath
                                        }
                                        else{
                                            bip32.textOutput = "m/" + obj.AccountKeyPath
                                        }
                                    }
                                    else{   // Invalid
                                        xpubOrPubl.textOutput = "Invalid QR"
                                        master_key_fgp.textOutput = "Invalid QR"
                                        bip32.textOutput = "Invalid QR"
                                    }
                                }
                                qrscaner.visible = false
                            }
                        }
                    }

                    Column {
                        id: hozlinespace
                        spacing: 12
                        anchors.verticalCenter: signerinforInput.verticalCenter
                        anchors.left: signerinforInput.right
                        anchors.leftMargin: 10
                        Rectangle {
                            width: 4
                            height: 110
                            radius: 2
                            anchors.horizontalCenter: parent.horizontalCenter
                            border.color: "#839096"
                        }

                        QText {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "OR"
                            color: "#595959"
                            font.pixelSize: 16
                            font.family: "Lato"
                            font.weight: Font.ExtraBold
                        }

                        Rectangle {
                            width: 4
                            height: 110
                            radius: 2
                            anchors.horizontalCenter: parent.horizontalCenter
                            border.color: "#839096"
                        }
                    }

                    QText {
                        anchors {
                            left: hozlinespace.right
                            leftMargin: 50
                            top: parent.top
                            topMargin: 24
                        }
                        text: "Via QR Code"
                        color: "#323E4A"
                        font.pixelSize: 16
                    }

                    QQrButton {
                        width: 100
                        height: 100
                        anchors.left: hozlinespace.right
                        anchors.leftMargin: 50
                        anchors.verticalCenter: hozlinespace.verticalCenter
                        onButtonClicked: qrscaner.visible = true
                    }

                    QTextButton {
                        width: 150
                        height: 48
                        label.text: "Add Signer"
                        label.font.pixelSize: 16
                        label.font.family: "Lato"
                        type: eTypeA
                        enabled: (signerName.textOutput !== "") &&
                                 (xpubOrPubl.textOutput !== "") &&
                                 (bip32.textOutput !== "") &&
                                 (master_key_fgp.textOutput !== "")
                        anchors {
                            right: parent.right
                            rightMargin: 40
                            bottom: parent.bottom
                            bottomMargin: 40
                        }
                        onButtonClicked: {
                            tabremoteSigner.startAddSigner()
                        }
                    }

                    function startAddSigner(){
                        stateAddSigner.visible = true
                        timerRemoteSigner.start()
                    }

                    Timer {
                        id: timerRemoteSigner
                        interval: 1000
                        repeat: false
                        onTriggered: {
                            var remoteSignerObj = { "signerNameInputted"    : signerName.textOutput,
                                "xpubOrPublInputted"    : xpubOrPubl.textOutput,
                                "bip32Inputted"         : bip32.textOutput ,
                                "masterFingerPrintInputted" : master_key_fgp.textOutput };
                            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER, remoteSignerObj)
                            stateAddSigner.visible = false
                            timerRemoteSigner.stop()
                        }
                    }
                }
            }

            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    implicitWidth: 400
                    implicitHeight: 48
                    color: "transparent"
                    QText {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        color: styleData.selected ? "#031F2B" : "#839096"
                        font.pixelSize: 14
                        font.weight: Font.DemiBold
                        font.family: "Montserrat"
                    }
                    Rectangle {
                        color: "#F6D65D"
                        implicitWidth: 400
                        height: 4
                        anchors.bottom: parent.bottom
                        visible: styleData.selected
                    }
                }

                frame: Rectangle { color: "transparent" }
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
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK)
            }
        }

        Rectangle {
            id: stateAddSigner
            visible: false
            anchors.fill: parent
            color: Qt.rgba(0, 0, 0, 0.9)
            anchors.bottom: parent.bottom
            radius: 8
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }
            Loader {
                id: busyIndi
                anchors.centerIn: parent
                sourceComponent: (AppModel.addSignerStep < 0 || AppModel.addSignerStep > 2) ? null : rootAddsignerToWallet.addSignerComp[AppModel.addSignerStep]
            }
        }

        QNumPad {
            id: pinModel
            anchors.fill: parent
            visible: false
            property int    signerIndex: -1
            property string signerName: ""
            onSenPINClicked : {
                var masterSignerObj = { "signerNameInputted"    : pinModel.signerName,
                                        "deviceIndexSelected"   : pinModel.signerIndex,
                                        "pinInputted"           : pinModel.pinInputted};
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN, masterSignerObj)
            }
        }

        Rectangle {
            id: qrscaner
            anchors.fill: parent
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8
            visible: false

            Rectangle {
                id: qrmask
                width: 500
                height: 500
                radius: 8
                visible: false
            }

            Rectangle {
                id: qrbg
                width: 500
                height: 500
                color: "#F1FAFE"
                radius: 8
                anchors.centerIn: parent
                visible: false
                Rectangle {
                    height: 4
                    width: parent.width
                    color: "#F6D65D"
                }
            }

            Rectangle {
                width: 500
                height: 500
                anchors.centerIn: parent
            }

            OpacityMask {
                id: opamask
                anchors.fill: qrbg
                source: qrbg
                maskSource: qrmask

                QCloseButton {
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    onClicked: qrscaner.visible = false
                }

                QText {
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 30
                    font.weight: Font.ExtraBold
                    text: "Scanning for QR code"
                }

                Loader {
                    id: qrcameraLoader
                    sourceComponent: qrscaner.visible ? qrcameraComp : null
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 30
                }
            }

            DropShadow {
                anchors.fill: opamask
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: opamask
            }
        }
    }

    property var addSignerComp: [addSignerStep0, addSignerStep1, addSignerStep2]

    Component {
        id: addSignerStep0
        Column {
            spacing: 8
            Item {
                width: 52
                height: 52
                QBusyIndicator {
                    width: 44
                    height: 44
                    anchors.centerIn: parent
                }
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QText {
                width: 186
                height: 36
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                text: "Add New Signer"
            }
            QText {
                width: 263
                height: 24
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 16
                font.family: "Lato"
                text: "Nunchuk is connecting to your device."
            }
        }
    }

    Component {
        id: addSignerStep1
        Column {
            spacing: 8
            width: 300
            Item {
                width: 52
                height: visible ? 52 : 0
                anchors.horizontalCenter: parent.horizontalCenter
                QBusyIndicator {
                    width: 44
                    height: 44
                    anchors.centerIn: parent
                }
            }
            QText {
                width: 186
                height: 36
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#F6D65D"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "Montserrat"
                text: "Signer Health Check"
            }
            QText {
                width: 420
                height: 24*lineCount
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#F6D65D"
                font.pixelSize: 14
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                horizontalAlignment: Text.AlignHCenter
                text: "Test signing message. Please wait and confirm the action on your signer device."
            }
            Item {
                width: 275
                height: 48
                anchors.horizontalCenter: parent.horizontalCenter
                Rectangle {
                    width: 275
                    height: 40
                    anchors.centerIn: parent
                    color: Qt.rgba(255, 255, 255, 0.2)
                    radius: 4
                    QText {
                        width: 186
                        height: 36
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                        color: "#F1FAFE"
                        font.pixelSize: 12
                        font.family: "Lato"
                        text: AppModel.masterSignerInfo.message
                    }
                }
            }
        }
    }

    Component {
        id: addSignerStep2
        Column {
            spacing: 8
            QProgressbar {
                id: progresBar
                anchors.horizontalCenter: parent.horizontalCenter
                percentage: AppModel.addSignerPercentage
            }
            QText {
                width: 186
                height: 36
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                text: "Processing..."
            }
            QText {
                width: 328
                height: 42
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                color: "#F6D65D"
                font.pixelSize: 16
                font.family: "Lato"
                text: "The signer is being added. Once complete, you can assign this signer to new wallets."
            }
        }
    }

    Connections {
        target: AppModel
        onAddSignerPercentageChanged : {
            if(AppModel.addSignerPercentage === 100){
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT, AppModel.masterSignerInfo.masterSignerHealth)
            }
        }
        onSentPINToDeviceResult : {
            if(result === EWARNING.NONE_MSG){
                pinModel.visible = false
                stateAddSigner.visible = true
            }
            else{
                // Warning message
                pinModel.warning = "The PIN you entered is incorrect. Please try again."
                pinModel.validInput = false
                pinModel.pinInputted = ""
            }
            console.log("onSentPINToDeviceResult ", result)
        }
    }

    Component {
        id: qrcameraComp
        QQrScanner {
            width: 300
            height: 300
        }
    }
}
