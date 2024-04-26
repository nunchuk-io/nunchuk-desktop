/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Popups"
import "../../Components/customizes/Signers"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootAddsignerToWallet
    readonly property int eHARDWARE_KEY: 1
    readonly property int eADD_KEY: 2
    property int selectType: eHARDWARE_KEY
    property string keySpec: ""
    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: selectType === eHARDWARE_KEY ? addHardwareKey : addKey
    }
    property string signerNameInputed: ""
    Component {
        id: addHardwareKey
        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_101
            onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
            QTextInputBox {
                id: signerName
                mode: eEDIT_MODE
                width: 532
                heightMin: 56
                placeholder.text: STR.STR_QML_102
                maximumLength: 106
                anchors {
                    left: parent.left
                    leftMargin: 40
                    top: parent.top
                    topMargin: 100
                }
                color: Qt.rgba(255, 255, 255, 0.5)
                border.color: "#C9DEF1"
                onTypingFinished: {
                    signerNameInputed = signerName.textOutput
                }
            }
            TabView {
                id: tabselect
                width: 728
                height: 505
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 197
                }
                Tab {
                    title: STR.STR_QML_103
                    Item {
                        id: tabmastersigner
                        QText {
                            id: text
                            anchors {
                                left: parent.left
                                top: parent.top
                                topMargin: 16
                            }
                            text: STR.STR_QML_676
                            color: "#031F2B"
                            font.pixelSize: 16
                            font.weight: Font.DemiBold
                            font.family: "Lato"
                        }
                        QImage {
                            id: nodevice
                            visible: !devicelist.visible
                            anchors {
                                left: text.left
                                top: text.bottom
                                topMargin: 8
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
                                top: parent.top
                                topMargin: 16
                            }
                            spacing: 8
                            currentIndex: -1
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
                                    layer.enabled: true
                                    layer.effect: DropShadow {
                                        source: rect
                                        verticalOffset: 2
                                        radius: 8
                                        samples: 16
                                        color: Qt.rgba(0, 0, 0, 0.15)
                                    }
                                    Rectangle {
                                        visible: index == devicelist.currentIndex
                                        width: 8
                                        height: parent.height
                                        color: "#F6D65D"
                                    }
                                    QIcon {
                                        anchors {
                                            left: parent.left
                                            leftMargin: 16
                                            verticalCenter: parent.verticalCenter
                                        }
                                        iconSize: 24
                                        source: index == devicelist.currentIndex ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
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
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        devicelist.currentIndex = index
                                        if(signerName.textOutput === ""){
                                            signerName.textOutput = device_type
                                            signerNameInputed = signerName.textOutput
                                        }
                                    }
                                }
                            }
                        }
                        QRefreshButton {
                            width: 160
                            height: 32
                            label: STR.STR_QML_105
                            fontPixelSize: 14
                            anchors {
                                left: parent.left
                                top:  parent.top
                                topMargin: 32 + text.height + (devicelist.count > 0 ? ( devicelist.height) : nodevice.height)
                            }
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH)
                            }
                        }
                        QTextButton {
                            width: 150
                            height: 48
                            label.text: STR.STR_QML_106
                            label.font.pixelSize: 16
                            label.font.family: "Lato"
                            type: eTypeE
                            enabled: (signerName.textOutput !== "") && (devicelist.currentIndex !== -1)
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 40
                            }
                            onButtonClicked: {
                                if(devicelist.currentIndex !== -1){
                                    var masterSignerObj = {
                                        "key_name"              : signerName.textOutput,
                                        "deviceIndexSelected"   : devicelist.currentIndex,
                                        "key_yes_accept"        : false
                                    };
                                    QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER, masterSignerObj)
                                }
                            }
                        }
                        Connections {
                            target: AppModel
                            onNotifySignerExist: {
                                showPopupInfo(isSoftware, fingerPrint)
                            }
                        }
                        Connections {
                            target: _info
                            onYesClicked: {
                                if (tabselect.currentIndex === 0) {
                                    var masterSignerObj = {
                                        "key_name"              : signerName.textOutput,
                                        "deviceIndexSelected"   : devicelist.currentIndex,
                                        "key_yes_accept"        : true
                                    };
                                    QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER, masterSignerObj)
                                }
                            }
                        }
                    }
                }
                Tab {
                    title: STR.STR_QML_107
                    Item {
                        id: tabremoteSigner
                        // width: 576
                        // height: _col.childrenRect.height
                        Column {
                            id: _col
                            spacing: 16
                            Item {
                                width: 576
                                height: 1
                            }
                            QText {
                                text: STR.STR_QML_108
                                color: "#323E4A"
                                font.pixelSize: 16
                            }

                            QTextAreaBoxTypeA  {
                                id: _key_spec
                                boxWidth: 576
                                boxHeight: 176
                                label: STR.STR_QML_127
                                textInputted: keySpec
                                onTypingFinished: {
                                    keySpec = _key_spec.textInputted
                                }
                                isValid: true
                                input.verticalAlignment: Text.AlignTop
                                input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                            }
                            Item {
                                width: 576
                                height: 48
                                Row {
                                    spacing: 16
                                    QIconTextButton {
                                        width: 280
                                        height: 36
                                        iconInRight: true
                                        label: STR.STR_QML_1288
                                        icons: ["QR-dark.svg", "QR-dark.svg", "QR-dark.svg","QR-dark.svg"]
                                        fontPixelSize: 16
                                        iconSize: 16
                                        type: eTypeD
                                        onButtonClicked: {
                                            qrscaner.open()
                                        }
                                    }
                                    QIconTextButton {
                                        width: 280
                                        height: 36
                                        iconInRight: true
                                        label: STR.STR_QML_1210
                                        icons: ["importFile.svg", "importFile.svg", "importFile.svg","importFile.svg"]
                                        fontPixelSize: 16
                                        iconSize: 16
                                        type: eTypeD
                                        onButtonClicked: {
                                            fileDialog.open()
                                        }
                                    }
                                }
                            }
                        }


                        Connections {
                            target: qrscaner
                            onTagFound: {
                                var jsonstring = "";
                                jsonstring = AppModel.parseQRSigners(qrscaner.tags)
                                if(jsonstring !== ""){
                                    tabremoteSigner.jsonProcess(jsonstring)
                                    qrscaner.close()
                                }
                            }
                        }
                        function jsonProcess(jsonstring){
                            if(jsonstring !== "") {
                                var jsonobj = JSON.parse(jsonstring);
                                var descriptor = ('descriptor' in jsonobj) ? jsonobj.descriptor : ""
                                keySpec = descriptor
                            }
                        }

                        Connections{
                            target: fileDialog
                            onAccepted: {
                                var jsonstring = "";
                                jsonstring = AppModel.parseJSONSigners(fileDialog.file)
                                tabremoteSigner.jsonProcess(jsonstring)
                            }
                        }

                        Connections {
                            target: AppModel
                            onNotifySignerExist: {
                                showPopupInfo(isSoftware, fingerPrint)
                            }
                        }

                        Connections {
                            target: _info
                            onYesClicked: {
                                if (tabselect.currentIndex === 1) {
                                    selectType = eADD_KEY
                                }
                            }
                        }

                        QTextButton {
                            id:btnContinue
                            width: 99
                            height: 48
                            label.text: STR.STR_QML_097
                            label.font.pixelSize: 16
                            label.font.family: "Lato"
                            type: eTypeE
                            enabled: (signerName.textOutput !== "")
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 36
                            }
                            onButtonClicked: {
                                tabremoteSigner.startAddSigner()
                            }
                        }
                        function startAddSigner(){
                            createRemoteBusyBox.open()
                            timerRemoteSigner.start()
                        }
                        Timer {
                            id: timerRemoteSigner
                            interval: 1000
                            repeat: false
                            onTriggered: {
                                var remoteSignerObj = {
                                    "key_name"       : signerName.textOutput,
                                    "key_spec"       : keySpec,
                                    "key_tag"        : "",
                                    "key_yes_accept" : false,
                                }
                                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER, remoteSignerObj)
                                timerRemoteSigner.stop()
                            }
                        }
                    }
                }
                style: TabViewStyle {
                    frameOverlap: 1
                    tab: Rectangle {
                        implicitWidth: 364
                        implicitHeight: 48
                        color: "transparent"
                        Row{
                            spacing: 4
                            anchors.centerIn: parent
                            QText {
                                id: txt
                                anchors.verticalCenter: parent.verticalCenter
                                text: styleData.title
                                color: styleData.selected ? "#031F2B" : "#839096"
                                font.pixelSize: 16
                                font.weight: Font.DemiBold
                                font.family: "Lato"
                            }
                            Rectangle {
                                anchors.verticalCenter: parent.verticalCenter
                                border.color: "#EAEAEA"
                                border.width: 1
                                color: "#FFFFFF"
                                implicitWidth: 76
                                implicitHeight: 24
                                radius: 20
                                visible: styleData.index === 1
                                QText {
                                    anchors.centerIn: parent
                                    text: "Advanced"
                                    color: styleData.selected ? "#031F2B" : "#839096"
                                    font.pixelSize: 12
                                    font.weight: Font.Normal
                                    font.family: "Lato"
                                }
                            }
                        }
                        Rectangle {
                            color: "#031F2B"
                            implicitWidth: 364
                            height: 2
                            anchors.bottom: parent.bottom
                            visible: styleData.selected
                        }
                    }

                    frame: Rectangle { color: "transparent" }
                }
            }
            QButtonTextLink {
                height: 24
                label: STR.STR_QML_059
                anchors {
                    left: parent.left
                    leftMargin: 40
                    bottom: parent.bottom
                    bottomMargin: 40
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST)
                }
            }
            Rectangle {
                id: stateAddSigner
                visible: AppModel.addSignerStep !== -1
                anchors.fill: parent
                color: Qt.rgba(0, 0, 0, 0.9)
                anchors.bottom: parent.bottom
                radius: 24
                MouseArea {
                    anchors.fill: parent
                    onClicked: {}
                }
                Loader {
                    id: busyIndi
                    anchors.centerIn: parent
                    sourceComponent: rootAddsignerToWallet.addSignerComp[AppModel.addSignerStep]
                }
            }
            QQrImportScanner {
                id: qrscaner
            }
            FileDialog {
                id: fileDialog
                fileMode: FileDialog.OpenFile
            }
        }
    }

    Component {
        id: addKey
        QAddKeySelectType {
            onKeyTypeSelected: {
                var remoteSignerObj = {
                    "key_name"      : signerNameInputed,
                    "key_spec"      : keySpec,
                    "key_tag"       : hardwareTag,
                    "key_yes_accept": true,
                }
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER, remoteSignerObj)
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
                text: STR.STR_QML_083
            }
            QText {
                width: 263
                height: 24
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 16
                font.family: "Lato"
                text: STR.STR_QML_117
            }
        }
    }
    Component {
        id: addSignerStep1
        Column {
            spacing: 8
            width: 500
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
                text: STR.STR_QML_118
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
                text: STR.STR_QML_119
            }
            Rectangle {
                width: 500
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                color: Qt.rgba(255, 255, 255, 0.2)
                radius: 4
                QText {
                    width: parent.width - 50
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F1FAFE"
                    font.pixelSize: 14
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    text: AppModel.newKeySignMessage
                }
            }
            QText {
                width: 328
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 14
                wrapMode: Text.WordWrap
                font.family: "Lato"
                text: "Hash"
            }
            Rectangle {
                width: 500
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                color: Qt.rgba(255, 255, 255, 0.2)
                radius: 4
                QText {
                    width: parent.width - 50
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F1FAFE"
                    font.pixelSize: 14
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    text: AppModel.newKeySignMessageSHA256
                    font.capitalization: Font.AllUppercase
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
                text: STR.STR_QML_120
            }
        }
    }
    Popup {
        id: createRemoteBusyBox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: boxMask
            width: popupWidth
            height: popupHeight
            radius: 24
            color: Qt.rgba(0, 0, 0, 0.8)
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: boxMask.width
                    height: boxMask.height
                    radius: 24
                }
            }
            Column {
                spacing: 8
                anchors.centerIn: parent
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
                    width: 700
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    wrapMode: Text.WrapAnywhere
                    text: STR.STR_QML_121 + signerNameInputed
                }
                QText {
                    width: 328
                    height: 42
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 18
                    wrapMode: Text.WordWrap
                    font.family: "Lato"
                    text: STR.STR_QML_122
                }
            }
        }
    }


    QPopupInfoTwoButtons {
        id: _info
        signal yesClicked()
        title: STR.STR_QML_661
        labels: [STR.STR_QML_433,STR.STR_QML_432]
        funcs: [
            function() { yesClicked() },
            function() {}
        ]
    }

    function showPopupInfo(isSoftware, fingerPrint){
        if (isSoftware) {
            _info.contentText = STR.STR_QML_1283.arg(fingerPrint.toUpperCase())
            _info.contentTextTwo = STR.STR_QML_1284
            _info.open()
        }
        else {
            _info.contentText = STR.STR_QML_1283.arg(fingerPrint.toUpperCase())
            _info.contentTextTwo = ""
            _info.open()
        }
    }

    Connections {
        target: AppModel
        onStartCreateRemoteSigner: { }
        onFinishedCreateRemoteSigner: {
            createRemoteBusyBox.close()
        }
    }
}
