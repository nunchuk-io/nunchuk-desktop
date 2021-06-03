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
            text: "Recover an existing seed"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.ExtraBold
            font.pixelSize: 24
        }
        QText {
            width: 540
            text: "Please enter the mnemonic recovery phrase."
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: notification.visible ? 48 : 58
            }
            color: "#031F2B"
            font.family: "Lato"
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
            verticalAlignment: Text.AlignVCenter
        }

        QNotification {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 81
            }
            visible: AppModel.mnemonic === "-101"
            label: "Wrong, Please check your mnemonic phrase to make sure they are correct"
            currentStatus: EWARNING.EXCEPTION_MSG
            onCloseRequest: {visible = false; AppModel.mnemonic = ""}
        }

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION)
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER)
                }
            }
        }

        Grid {
            id: gridmmonic
            width: 730
            height: 500
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 150
            columns: 6
            spacing: 12
            property bool isFullFill: false
            property string mnemonicstr: ""
            Repeater {
                id: inputrepeater
                model: 24
                QTextInputBoxTypeB {
                    id: inputmnemonic
                    label: " " + ((index+1) > 9 ? (index+1) : "0"+(index+1)) + "."
                    boxWidth: (gridmmonic.width - gridmmonic.spacing*(gridmmonic.columns-1)) / gridmmonic.columns
                    boxHeight: 48
                    onTextInputtedChanged: {
                        var result = 0;
                        var str = ""
                        for(var i = 0; i < inputrepeater.count ; i++){
                            if(inputrepeater.itemAt(i).textInputted !== "") {
                                if(result === 0){ str = inputrepeater.itemAt(i).textInputted }
                                else{ str = str + " " + inputrepeater.itemAt(i).textInputted }
                                result++;
                            }
                        }
                        gridmmonic.mnemonicstr = str
                        gridmmonic.isFullFill = (result >= 12) ? true : false
                        AppModel.mnemonic = ""

                        var targetY = inputmnemonic.y + inputmnemonic.height + gridmmonic.y
                        var targetX = inputmnemonic.x + gridmmonic.x
                        if(textBoxFocus){
                            suggestionList.currentIndexBox = index
                            suggestionList.textSearch = inputmnemonic.textInputted
                        }
                    }
                    onTextBoxFocusChanged: {
                        if(textBoxFocus){
                            suggestionList.currentIndexBox = index
                            suggestionList.y = inputmnemonic.y + inputmnemonic.height + gridmmonic.y
                            suggestionList.x = inputmnemonic.x + gridmmonic.x
                            suggestionList.textSearch = inputmnemonic.textInputted
                        }
                        else{
                            if(suggestionList.currentIndexBox === index){
                                suggestionList.currentIndexBox = -1
                                suggestionList.textSearch = ""
                            }
                            else{
                                suggestionList.textSearch = ""
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
                leftMargin: 32
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_RECOVER_SOFTWARE_SIGNER_BACK)
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: "Continue"
            label.font.pixelSize: 16
            type: eTypeA
            enabled: (gridmmonic.isFullFill)
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_RECOVER_SOFTWARE_SIGNER_REQUEST, gridmmonic.mnemonicstr)
            }
        }

        Item {
            id: suggestionList
            property int currentIndexBox: -1
            property string textSearch: ""
            width: suggestionList.visible ? (gridmmonic.width - gridmmonic.spacing*(gridmmonic.columns-1)) / gridmmonic.columns : 0
            height: suggestionList.visible ? Math.min(sglst.contentHeight + 10, 250): 0
            visible: suggestionList.currentIndexBox !== -1 && sglst.count > 0
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }

            Rectangle {
                id: bg
                anchors {
                    fill: parent
                    leftMargin: 5
                    rightMargin: 5
                    topMargin: 5
                    bottomMargin: 5
                }
                radius: 4
                color: "#FFFFFF"
                visible: false
            }

            DropShadow {
                anchors.fill: bg
                verticalOffset: 3
                cached: true
                radius: 16
                samples: 24
                color: Qt.rgba(0, 0, 0, 0.4)
                source: bg

                ListView {
                    id: sglst
                    anchors.centerIn: parent
                    width: parent.width - 10
                    height: suggestionList.visible ? Math.min(sglst.contentHeight, 240): 0
                    model: AppModel.suggestMnemonics
                    clip: true
                    ScrollBar.vertical: ScrollBar { active: true }
                    delegate: Rectangle {
                        width: sglst.width
                        height: textsuggest.visible ? 240/10 : 0
                        color: suggestMouse.containsMouse ? "#F1FAFE" : "#FFFFFF"
                        QText {
                            id: textsuggest
                            anchors.centerIn: parent
                            text: modelData
                            color: suggestMouse.pressed ?"#031F2B" :  suggestMouse.containsMouse ? "#35ABEE" : "#031F2B"
                            font.pixelSize: 16
                            font.weight: suggestMouse.containsMouse ? Font.ExtraBold : Font.Normal
                            visible: {
                                var modelText = modelData
                                var inputText = suggestionList.textSearch
                                return (modelText.toLowerCase().includes(inputText.toLowerCase()))
                            }
                        }

                        MouseArea {
                            id: suggestMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if(suggestionList.currentIndexBox !== -1) {inputrepeater.itemAt(suggestionList.currentIndexBox).textInputted = modelData}
                                suggestionList.currentIndexBox = -1
                                suggestionList.textSearch = ""
                            }
                        }
                    }
                }
            }
        }
    }
}
