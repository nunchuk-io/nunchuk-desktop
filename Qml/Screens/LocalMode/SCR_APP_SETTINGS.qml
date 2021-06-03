import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    id: rootSettings
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
            id: scrtitle
            height: 36
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            text: "Settings"
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
                if(rootSettings.anyChanged){
                    modelConfirmUpdateSetting.visible = true
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                }
                AppSetting.changePassphraseResult = 0
            }
        }

        // Body
        Row {
            anchors.fill: parent
            Item {
                width: parent.width/2
                height: parent.height
                Item {
                    width: 342
                    height: 600
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 74
                    }
                    QText {
                        id: displayUnit
                        height: 24
                        text: "Display Unit"
                        font.family: "Montserrat"
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        color: "#031F2B"
                    }

                    Column {
                        id: unitslection
                        anchors.top: displayUnit.bottom
                        anchors.topMargin: 8
                        spacing: 8
                        property int unitSelected: AppSetting.unit
                        property bool enableFixedPrecision: AppSetting.enableFixedPrecision
                        Row {
                            width: parent.width
                            height: 24
                            QRadioButtonTypeA {
                                id: btc
                                width: 100
                                height: 24
                                label: "BTC"
                                fontFamily: "Montserrat"
                                fontWeight: unitslection.selected ? Font.ExtraBold : Font.DemiBold
                                selected: (unitslection.unitSelected === NUNCHUCKTYPE.BTC)
                                onButtonClicked: { unitslection.unitSelected = NUNCHUCKTYPE.BTC }
                            }

                            Row {
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 4
                                enabled: (unitslection.unitSelected === NUNCHUCKTYPE.BTC)
                                QImage {
                                    width: 16
                                    height: 16
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: unitslection.enableFixedPrecision ? "qrc:/Images/Images/Checked_n.png" : "qrc:/Images/Images/UnChecked_n.png"
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {unitslection.enableFixedPrecision = !unitslection.enableFixedPrecision}
                                    }
                                }
                                QText {
                                    width: 111
                                    height: 16
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: "Show Fixed Precision"
                                    font.family: "Lato"
                                    font.pixelSize: 12
                                }
                            }

                        }

                        QRadioButtonTypeA {
                            id: satoshi
                            height: 24
                            label: "Satoshi (1 BTC = 100,000,000 sat)"
                            fontWeight: satoshi.selected ? Font.ExtraBold : Font.DemiBold
                            selected: (unitslection.unitSelected === NUNCHUCKTYPE.SATOSHI)
                            onButtonClicked: { unitslection.unitSelected = NUNCHUCKTYPE.SATOSHI }
                        }

                        property bool anyChanged: (unitslection.unitSelected !== AppSetting.unit)
                                                  || (unitslection.enableFixedPrecision !== AppSetting.enableFixedPrecision)
                        function applySettings(){
                            AppSetting.unit = unitslection.unitSelected
                            AppSetting.enableFixedPrecision = unitslection.enableFixedPrecision
                        }
                    }

                    Rectangle {
                        id: line1
                        width: parent.width
                        height: 1
                        anchors.top: unitslection.bottom
                        anchors.topMargin: 16
                        color: "#C9DEF1"
                    }

                    Item {
                        id: dbencriptiontitle
                        width: parent.width
                        height: 24
                        anchors.top: line1.bottom
                        anchors.topMargin: 24
                        QText {
                            width: 155
                            height: 24
                            text: "Database Encryption"
                            font.family: "Montserrat"
                            font.weight: Font.DemiBold
                            font.pixelSize: 16
                            color: "#031F2B"
                        }

                        QSwitch {
                            id: dbencriptionswitch
                            width: 85
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            checked: AppSetting.enableDBEncryption
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    dbencriptionswitch.checked = !dbencriptionswitch.checked
                                    inputpassphrase.textOutput = ""
                                    confirmpassphrase.textOutput = ""
                                    AppSetting.changePassphraseResult = 0
                                }
                            }

                            property bool anyChanged: (AppSetting.enableDBEncryption !== dbencriptionswitch.checked)
                            function applySettings(){
                                if(dbencriptionswitch.checked !== AppSetting.enableDBEncryption && false === dbencriptionswitch.checked){
                                    QMLHandle.sendEvent(EVT.EVT_APP_SETTING_CHANGE_PASSPHRASE, "")
                                }
                                AppSetting.enableDBEncryption = dbencriptionswitch.checked
                            }
                        }
                    }

                    QTextInputBox {
                        id: inputpassphrase
                        visible: (dbencriptionswitch.checked)
                        width: 342
                        height: (!dbencriptionswitch.checked) ? 0 : 56
                        anchors.top: dbencriptiontitle.bottom
                        anchors.topMargin: (!dbencriptionswitch.checked) ? 0 : 16
                        border.color: "#C9DEF1"
                        placeholder.text: "Enter passphrase"
                        echoMode: showpass.visiblity ? TextInput.Normal : TextInput.Password
                        color: (dbencriptionswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                        enabled: (dbencriptionswitch.checked)
                        errorText.text: {
                            if(AppSetting.changePassphraseResult === 0){
                                return "Passphrase must have at least 8 characters."
                            }
                            else if(AppSetting.changePassphraseResult === 4){   //AppSetting::ChangePassphraseResult::SUCCEED
                                return "Passphrase change successfully."
                            }
                            else{
                                return "Cannot change your inputted Passphrase";
                            }
                        }

                        errorText.visible: enabled
                        validInput: (inputpassphrase.length === 0 ) || (inputpassphrase.length >= 8)
                        errorText.color: {
                            if(AppSetting.changePassphraseResult === 0){
                                return (validInput ? "#839096" : "#E02247")
                            }
                            else if(AppSetting.changePassphraseResult === 4){   //AppSetting::ChangePassphraseResult::SUCCEED
                                return "#35ABEE";
                            }
                            else{
                                return "#E02247";
                            }
                        }

                        Image {
                            id: showpass
                            width: 24
                            height: 24
                            property bool visiblity: false
                            source: showpass.visiblity ? "qrc:/Images/Images/visibility.png":"qrc:/Images/Images/visibility-off.png"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 17
                            visible: (inputpassphrase.textOutput !== "") && (dbencriptionswitch.checked)
                            MouseArea {
                                anchors.fill: parent
                                onClicked: showpass.visiblity =!showpass.visiblity
                            }
                        }
                        onTypingFinished: {
                            AppSetting.passphrase = currentText
                            AppSetting.changePassphraseResult = 0
                        }
                    }

                    QTextInputBox {
                        id: confirmpassphrase
                        visible: (dbencriptionswitch.checked)
                        width: 342
                        height: (!dbencriptionswitch.checked) ? 0 : 56
                        anchors.top: inputpassphrase.bottom
                        anchors.topMargin: (!dbencriptionswitch.checked) ? 0 : 24
                        border.color: "#C9DEF1"
                        placeholder.text: "Re-enter to confirm passphrase"
                        echoMode: showconfirmpass.visiblity ? TextInput.Normal : TextInput.Password
                        color: (dbencriptionswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                        errorText.text: {
                            if(AppSetting.changePassphraseResult === 0){
                                return "Passphrase must have at least 8 characters."
                            }
                            else if(AppSetting.changePassphraseResult === 4){   //AppSetting::ChangePassphraseResult::SUCCEED
                                return "Passphrase change successfully."
                            }
                            else{
                                return "Cannot change your inputted Passphrase";
                            }
                        }
                        errorText.visible: enabled
                        validInput: (confirmpassphrase.length === 0 ) || (confirmpassphrase.length >= 8) && (inputpassphrase.textOutput === confirmpassphrase.textOutput)
                        errorText.color: {
                            if(AppSetting.changePassphraseResult === 0){
                                return (validInput ? "#839096" : "#E02247")
                            }
                            else if(AppSetting.changePassphraseResult === 4){   //AppSetting::ChangePassphraseResult::SUCCEED
                                return "#35ABEE";
                            }
                            else{
                                return "#E02247";
                            }
                        }
                        enabled: (dbencriptionswitch.checked)
                        Image {
                            id: showconfirmpass
                            width: 24
                            height: 24
                            property bool visiblity: false
                            source: showconfirmpass.visiblity ? "qrc:/Images/Images/visibility.png":"qrc:/Images/Images/visibility-off.png"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 17
                            visible: (confirmpassphrase.textOutput !== "") && (dbencriptionswitch.checked)
                            MouseArea {
                                anchors.fill: parent
                                onClicked: showconfirmpass.visiblity =!showconfirmpass.visiblity
                            }
                        }
                        onTypingFinished: {
                            AppSetting.confirmPassPhrase = currentText
                            AppSetting.changePassphraseResult = 0
                        }
                    }

                    QTextButton {
                        id: changepassbtn
                        visible: (dbencriptionswitch.checked)
                        width: 162
                        height: (!dbencriptionswitch.checked) ? 0 : 32
                        label.text: "Change Passphrase"
                        label.font.pixelSize: 14
                        type: eTypeB
                        anchors.top: confirmpassphrase.bottom
                        anchors.topMargin: (!dbencriptionswitch.checked) ? 0 : 24
                        anchors.right: parent.right
                        enabled: (inputpassphrase.length >= 8) && (confirmpassphrase.length >= 8) &&
                                 (inputpassphrase.textOutput === confirmpassphrase.textOutput) && (dbencriptionswitch.checked)
                        onButtonClicked: {
                            pleasewait.visible = true
                            AppSetting.changePassphraseResult = 0
                            setpassphraseTime.start()
                        }

                        Timer {
                            id: setpassphraseTime
                            interval: 100
                            onTriggered: {
                                QMLHandle.sendEvent(EVT.EVT_APP_SETTING_CHANGE_PASSPHRASE, inputpassphrase.textOutput)
                                pleasewait.visible = false
                            }
                        }
                    }

                    Rectangle {
                        id: line2
                        width: parent.width
                        height: 1
                        anchors.top: changepassbtn.bottom
                        anchors.topMargin: 16
                        color: "#C9DEF1"
                    }

                    Item {
                        id: hwidrivertitle
                        width: parent.width
                        height: 24
                        anchors.top: line2.bottom
                        anchors.topMargin: 16
                        QText {
                            width: 155
                            height: 24
                            text: "Custom HWI Driver"
                            font.family: "Montserrat"
                            font.weight: Font.DemiBold
                            font.pixelSize: 16
                            color: "#031F2B"
                        }

                        QSwitch {
                            id: hwidriverswitch
                            width: 85
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            checked: AppSetting.enableCustomizeHWIDriver
                            MouseArea {
                                anchors.fill: parent
                                onClicked: { hwidriverswitch.checked = !hwidriverswitch.checked }
                            }
                        }

                        property bool anyChanged: (AppSetting.enableCustomizeHWIDriver !== hwidriverswitch.checked)
                        function applySettings(){
                            AppSetting.enableCustomizeHWIDriver = hwidriverswitch.checked
                        }
                    }

                    QTextInputBox {
                        id: hwipath
                        visible: (hwidriverswitch.checked)
                        width: 342
                        height: (!hwidriverswitch.checked) ? 0 : 56
                        anchors.top: hwidrivertitle.bottom
                        anchors.topMargin: (!hwidriverswitch.checked) ? 0 : 16
                        border.color: "#C9DEF1"
                        placeholder.text: "Enter Path to Your HWI Executable"
                        textOutput: AppSetting.hwiPath
                        color: (hwidriverswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                        enabled: (hwidriverswitch.checked)

                        property bool anyChanged: (AppSetting.hwiPath !== hwipath.textOutput)
                        function applySettings(){
                            AppSetting.hwiPath = hwipath.textOutput
                        }
                    }
                }
            }

            Item {
                width: parent.width/2
                height: parent.height
                Item {
                    width: 342
                    height: 600
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 74
                    }

                    QText {
                        id: networksettingtit
                        width: 155
                        height: 24
                        text: "Network Settings"
                        font.family: "Montserrat"
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        color: "#031F2B"
                    }

                    Column {
                        id: servernodeselection
                        anchors.top: networksettingtit.bottom
                        anchors.topMargin: 8
                        spacing: 8
                        property bool enableCoreRPC: AppSetting.enableCoreRPC
                        QRadioButtonTypeA {
                            id: electrumradio
                            height: 24
                            label: "Connect to Electrum server"
                            fontFamily: "Montserrat"
                            fontWeight: electrumradio.selected ? Font.ExtraBold : Font.DemiBold
                            selected: !servernodeselection.enableCoreRPC
                            onButtonClicked: { servernodeselection.enableCoreRPC = false }
                        }

                        QRadioButtonTypeA {
                            id: corerpcradio
                            height: 24
                            label: "Connect to Core node"
                            fontFamily: "Montserrat"
                            fontWeight: corerpcradio.selected ? Font.ExtraBold : Font.DemiBold
                            selected: servernodeselection.enableCoreRPC
                            onButtonClicked: { servernodeselection.enableCoreRPC = true }
                        }

                        property bool anyChanged: (servernodeselection.enableCoreRPC !== AppSetting.enableCoreRPC)
                        function applySettings(){
                            AppSetting.enableCoreRPC = servernodeselection.enableCoreRPC
                        }
                    }

                    Rectangle {
                        id: line3
                        width: parent.width
                        height: 1
                        anchors.top: servernodeselection.bottom
                        anchors.topMargin: 16
                        color: "#C9DEF1"
                    }

                    Item {
                        id: electrumItem
                        width: parent.width
                        anchors.right: parent.right
                        anchors.top: line3.bottom
                        anchors.topMargin: 8
                        visible: !servernodeselection.enableCoreRPC
                        enabled: !servernodeselection.enableCoreRPC

                        Column {
                            id: devselection
                            property int primaryServer: AppSetting.primaryServer
                            property string mainnetServer: AppSetting.mainnetServer
                            property string testnetServer: AppSetting.testnetServer
                            spacing: 8
                            QRadioButtonTypeB {
                                id: mainnetsever
                                width: 342
                                selected: (devselection.primaryServer === NUNCHUCKTYPE.MAIN)
                                text.placeholder.text: "Mainnet Server"
                                text.textOutput: devselection.mainnetServer
                                onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.MAIN }
                                onButtonResetClicked: { mainnetsever.text.textOutput = MAINNET_SERVER}
                            }

                            QRadioButtonTypeB {
                                id: testnetsever
                                width: 342
                                selected: (devselection.primaryServer === NUNCHUCKTYPE.TESTNET)
                                text.placeholder.text: "Testnet Server"
                                text.textOutput: devselection.testnetServer
                                onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.TESTNET }
                                onButtonResetClicked: { testnetsever.text.textOutput = TESTNET_SERVER}
                            }

                            property bool anyChanged: (AppSetting.primaryServer !== devselection.primaryServer)
                                                      || (AppSetting.mainnetServer !== mainnetsever.text.textOutput)
                                                      || (AppSetting.testnetServer !== testnetsever.text.textOutput)
                            function applySettings(){
                                AppSetting.primaryServer = devselection.primaryServer
                                AppSetting.mainnetServer = mainnetsever.text.textOutput
                                AppSetting.testnetServer = testnetsever.text.textOutput
                            }
                        }

                        Column {
                            id: certItem
                            anchors.right: parent.right
                            anchors.top: devselection.bottom
                            anchors.topMargin: 8
                            spacing: !certswitch.checked ? 0 : 8
                            Item {
                                id: setonCert
                                width: 342
                                height: 24
                                anchors.right: parent.right
                                QText {
                                    text: "Set SSL Certificate"
                                    width: 155
                                    height: 24
                                    font.family: "Montserrat"
                                    font.weight: Font.DemiBold
                                    font.pixelSize: 14
                                    color: "#031F2B"
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                QSwitch {
                                    id: certswitch
                                    width: 85
                                    height: 20
                                    checked: AppSetting.enableCertificateFile
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: { certswitch.checked = !certswitch.checked }
                                    }
                                }

                                property bool anyChanged: (AppSetting.enableCertificateFile !== certswitch.checked)
                                function applySettings(){
                                    AppSetting.enableCertificateFile = certswitch.checked
                                }
                            }

                            QTextInputBox {
                                id: certPath
                                width: 342
                                height: !certswitch.checked ? 0 : 56
                                visible: certswitch.checked
                                anchors.right: parent.right
                                border.color: "#C9DEF1"
                                placeholder.text: "Enter Path to Your SSL Certificate"
                                textOutput: AppSetting.certificateFile
                                color: (certswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                                enabled: (certswitch.checked)

                                property bool anyChanged: (AppSetting.certificateFile !== certPath.textOutput)
                                function applySettings(){
                                    AppSetting.certificateFile = certPath.textOutput
                                }
                            }
                        }

                        Item {
                            id: torproxytitle
                            width: parent.width
                            height: 24
                            anchors.top: certItem.bottom
                            anchors.topMargin: 8

                            QText {
                                width: 155
                                height: 24
                                text: "Enable TOR Proxy"
                                font.family: "Montserrat"
                                font.weight: Font.DemiBold
                                font.pixelSize: 14
                                color: "#031F2B"
                            }

                            QSwitch {
                                id: torproxyswitch
                                width: 85
                                height: 20
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                checked: AppSetting.enableTorProxy
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: { torproxyswitch.checked = !torproxyswitch.checked }
                                }
                            }

                            property bool anyChanged: (AppSetting.enableTorProxy !== torproxyswitch.checked)
                            function applySettings(){
                                AppSetting.enableTorProxy = torproxyswitch.checked
                            }
                        }

                        Column {
                            id: torinputdata
                            spacing: 8
                            anchors.top: torproxytitle.bottom
                            anchors.topMargin: 8
                            anchors.right: parent.right
                            visible: torproxyswitch.checked
                            Row {
                                height: 56
                                spacing: 10
                                anchors.horizontalCenter: parent.horizontalCenter
                                QTextInputBox {
                                    id: toraddress
                                    width: 232
                                    height: parent.height
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "Proxy Address"
                                    textOutput: AppSetting.torProxyAddress
                                    color: (torproxyswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                                    enabled: (torproxyswitch.checked)
                                }

                                QTextInputBox {
                                    id: torport
                                    width: 102
                                    height: parent.height
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "Port"
                                    textOutput: AppSetting.torProxyPort
                                    color: (torproxyswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                                    enabled: (torproxyswitch.checked)
                                }
                            }

                            Row {
                                height: 56
                                spacing: 10
                                QTextInputBox {
                                    id: torproxyname
                                    width: 166
                                    height: 56
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "Proxy name"
                                    textOutput: AppSetting.torProxyName
                                    color: (torproxyswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                                    enabled: (torproxyswitch.checked)
                                }

                                QTextInputBox {
                                    id: torproxypassword
                                    width: 166
                                    height: 56
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "Proxy password"
                                    textOutput: AppSetting.torProxyPassword
                                    echoMode: TextInput.Password
                                    color: (torproxyswitch.checked) ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                                    enabled: (torproxyswitch.checked)
                                }
                            }

                            property bool anyChanged: (AppSetting.torProxyAddress !== toraddress.textOutput)
                                                      || ((AppSetting.torProxyPort).toString() !== torport.textOutput)
                                                      || (AppSetting.torProxyName !== torproxyname.textOutput)
                                                      || (AppSetting.torProxyPassword !== torproxypassword.textOutput)
                            function applySettings(){
                                AppSetting.torProxyAddress = toraddress.textOutput
                                AppSetting.torProxyPort = torport.textOutput
                                AppSetting.torProxyName = torproxyname.textOutput
                                AppSetting.torProxyPassword = torproxypassword.textOutput
                            }
                        }
                    }

                    Item {
                        id: corerpcItem
                        width: parent.width
                        anchors.right: parent.right
                        anchors.top: line3.bottom
                        anchors.topMargin: 16
                        visible: servernodeselection.enableCoreRPC
                        enabled: servernodeselection.enableCoreRPC

                        QText {
                            id: waringtit
                            width: parent.width
                            text: "If you switch to Core for the first time and your wallet has a history, a blockchain rescan would be needed to reconstruct the wallet. This process might take some time."
                            wrapMode: Text.WordWrap
                            font.family: "Montserrat"
                            font.pixelSize: 10
                            color: "#031F2B"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignJustify
                        }

                        // Core rpc
                        Column {
                            id: corerpcinputdata
                            width: parent.width
                            anchors.top: waringtit.bottom
                            anchors.topMargin: 8
                            spacing: 8
                            Row {
                                width: parent.width
                                height: 56
                                spacing: 10
                                anchors.horizontalCenter: parent.horizontalCenter
                                QTextInputBox {
                                    id: corerpcaddress
                                    width: 232
                                    height: 56
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "RPC host"
                                    textOutput: AppSetting.coreRPCAddress
                                    color: Qt.rgba(255, 255, 255, 0.3)
                                }

                                QTextInputBox {
                                    id: corerpcport
                                    width: 102
                                    height: 56
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "RPC Port"
                                    textOutput: AppSetting.coreRPCPort
                                    color: Qt.rgba(255, 255, 255, 0.3)
                                }
                            }

                            Row {
                                height: 56
                                spacing: 10
                                QTextInputBox {
                                    id: corerpcproxyname
                                    width: 166
                                    height: 56
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "RPC username"
                                    textOutput: AppSetting.coreRPCName
                                    color: Qt.rgba(255, 255, 255, 0.3)
                                }

                                QTextInputBox {
                                    id: corerpcproxypassword
                                    width: 166
                                    height: 56
                                    anchors.verticalCenter: parent.verticalCenter
                                    border.color: "#C9DEF1"
                                    placeholder.text: "RPC password"
                                    textOutput: AppSetting.coreRPCPassword
                                    echoMode: TextInput.Password
                                    color: Qt.rgba(255, 255, 255, 0.3)
                                }
                            }

                            property bool anyChanged: (AppSetting.coreRPCAddress !== corerpcaddress.textOutput)
                                                      || ((AppSetting.coreRPCPort).toString() !== corerpcport.textOutput)
                                                      || (AppSetting.coreRPCName !== corerpcproxyname.textOutput)
                                                      || (AppSetting.coreRPCPassword !== corerpcproxypassword.textOutput)
                            function applySettings(){
                                AppSetting.coreRPCAddress = corerpcaddress.textOutput
                                AppSetting.coreRPCPort = corerpcport.textOutput
                                AppSetting.coreRPCName = corerpcproxyname.textOutput
                                AppSetting.coreRPCPassword = corerpcproxypassword.textOutput
                            }
                        }

                        QText {
                            id: rescantit
                            anchors.top: corerpcinputdata.bottom
                            anchors.topMargin: 8
                            width: 155
                            height: 24
                            text: "Manual Rescan"
                            font.family: "Montserrat"
                            font.weight: Font.DemiBold
                            font.pixelSize: 16
                            color: "#031F2B"
                        }

                        Row {
                            width: parent.width
                            height: 56
                            spacing: 10
                            anchors.top: rescantit.bottom
                            anchors.topMargin: 8
                            anchors.horizontalCenter: parent.horizontalCenter
                            QTextInputBox {
                                id: inputHeightRescan
                                width: 232
                                height: 56
                                anchors.verticalCenter: parent.verticalCenter
                                border.color: "#C9DEF1"
                                placeholder.text: "Block height"
                                textOutput: "0"
                                color: AppSetting.enableCoreRPC ? Qt.rgba(255, 255, 255, 0.3) : Qt.rgba(0, 0, 0, 0.1)
                                enabled: AppSetting.enableCoreRPC
                                validator: RegExpValidator { regExp: /^[1-9][0-9]*$/ }
                            }

                            QTextButton {
                                id: rescanbtn
                                width: 100
                                height: 40
                                label.text: "Rescan"
                                label.font.pixelSize: 14
                                type: eTypeB
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: AppSetting.enableCoreRPC
                                onButtonClicked: {
                                    QMLHandle.sendEvent(EVT.EVT_APP_SETTING_REQUEST_RESCAN, inputHeightRescan.textOutput)
                                    QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                                }
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            id: pleasewait
            anchors.fill: parent
            visible: false
            MouseArea {anchors.fill: parent; onClicked: {}}
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8

            Rectangle {
                id: pleasewaitContent
                width: 340
                height: 56
                color: "#F1FAFE"
                radius: 4
                anchors.top: parent.top
                anchors.topMargin: 280
                anchors.left: parent.left
                anchors.leftMargin: 30
                QText {
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    text: "Please wait, processing ..."
                    color: "#031F2B"
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    font.pixelSize: 14
                }
            }

            DropShadow {
                anchors.fill: pleasewaitContent
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: pleasewaitContent
            }
        }

        // ModelConfirm settting
        Rectangle {
            id: modelConfirmUpdateSetting
            anchors.fill: parent
            visible: false
            MouseArea {anchors.fill: parent; onClicked: {}}
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8

            Rectangle {
                id: modelConfirmUpdateSettingMask
                anchors.centerIn: parent
                width: 488
                height: 250
                radius: 8
                visible: false
            }

            Rectangle {
                id: modelConfirmUpdateSettingContent
                width: 488
                height: 250
                color: "#F1FAFE"
                anchors.centerIn: parent
                visible: false
                Rectangle {
                    width: parent.width
                    height: 2
                    color: "Red"
                }
            }

            OpacityMask {
                id: modelConfirmUpdateSettingOpacity
                anchors.fill: modelConfirmUpdateSettingContent
                source: modelConfirmUpdateSettingContent
                maskSource: modelConfirmUpdateSettingMask

                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 51
                    horizontalAlignment: Text.AlignHCenter
                    text: "Apply Changes?"
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.DemiBold
                    font.pixelSize: 24
                }

                QText {
                    width: 355
                    height: 63
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 91
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    text: "Please note that some changes require an app reboot."
                    font.family: "Lato"
                    font.pixelSize: 14
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 50
                    spacing: 16
                    QTextButton {
                        width: 100
                        height: 32
                        label.text: "Discard Changes"
                        label.font.pixelSize: 10
                        type: eTypeB
                        radius: 12
                        onButtonClicked: {
                            modelConfirmUpdateSetting.visible = false
                            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                        }
                    }

                    QTextButton {
                        width: 100
                        height: 32
                        label.text: "Apply"
                        radius: 12
                        label.font.pixelSize: 10
                        type: eTypeA
                        onButtonClicked: {
                            rootSettings.applySettings()
                            modelConfirmUpdateSetting.visible = false
                            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_UPDATE_SETTING)
                        }
                    }
                }
            }

            DropShadow {
                anchors.fill: modelConfirmUpdateSettingOpacity
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: modelConfirmUpdateSettingOpacity
            }
        }

        // ModelConfirm restart
        Rectangle {
            id: modelRestartApp
            anchors.fill: parent
            visible: false
            MouseArea {anchors.fill: parent; onClicked: {}}
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8

            Rectangle {
                id: modelInfomRestartMask
                anchors.centerIn: parent
                width: 488
                height: 250
                radius: 8
                visible: false
            }

            Rectangle {
                id: modelInfomRestart
                visible: false
                width: 488
                height: 250
                color: "#F1FAFE"
                anchors.centerIn: parent
                radius: 8
                Rectangle {
                    width: parent.width
                    height: 2
                    color: "Red"
                }
            }

            OpacityMask {
                id: modelInfomRestartOpacity
                anchors.fill: modelInfomRestart
                source: modelInfomRestart
                maskSource: modelInfomRestartMask
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 51
                    horizontalAlignment: Text.AlignHCenter
                    text: "Restart application"
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.DemiBold
                    font.pixelSize: 24
                }

                QText {
                    width: 355
                    height: 63
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 91
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    text: "Please restart the application to apply the new settings."
                    font.family: "Lato"
                    font.pixelSize: 14
                }

                QTextButton {
                    width: 100
                    height: 32
                    label.text: "Restart"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 50
                    label.font.pixelSize: 10
                    type: eTypeB
                    radius: 12
                    onButtonClicked: {
                        modelRestartApp.visible = false
                        QMLHandle.sendEvent(EVT.EVT_APP_SETTING_REQUEST_RESTART)
                    }
                }
            }

            DropShadow {
                anchors.fill: modelInfomRestartOpacity
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: modelInfomRestartOpacity
            }
        }
    }

    property bool anyChanged: unitslection.anyChanged || dbencriptionswitch.anyChanged || hwidrivertitle.anyChanged
                              || hwipath.anyChanged || devselection.anyChanged || servernodeselection.anyChanged || corerpcinputdata.anyChanged
                              || torproxytitle.anyChanged || torinputdata.anyChanged || setonCert.anyChanged || certPath.anyChanged

    function applySettings(){
        unitslection.applySettings()
        dbencriptionswitch.applySettings()
        hwidrivertitle.applySettings()
        hwipath.applySettings()
        devselection.applySettings()
        torproxytitle.applySettings()
        torinputdata.applySettings()
        setonCert.applySettings()
        certPath.applySettings()
        servernodeselection.applySettings()
        corerpcinputdata.applySettings()
    }

    Connections {
        target: AppModel
        onRequireRestartApp : { modelRestartApp.visible = true }
    }
}
