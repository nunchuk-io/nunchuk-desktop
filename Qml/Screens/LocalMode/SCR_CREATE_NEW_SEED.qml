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
    property string textMnemonic: AppModel.mnemonic
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

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION)
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_NEW_SEED)
                }
            }
        }

        QText {
            width: 163
            height: 27
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            text: "Confirm signer mnemonic"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.ExtraBold
            font.pixelSize: 24
        }

        QText {
            width: 540
            text: "Please confirm the following words."
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: notification.visible ? 48 : 58
            }
            color: "#031F2B"
            font.family: "Lato"
            font.pixelSize: 16
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
        }

        QNotification {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 81
            }
            visible: false
            label: "Incorrect words, please try again."
            currentStatus: EWARNING.EXCEPTION_MSG
            onCloseRequest: {visible = false}
        }

        Column {
            spacing: 48
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 150
            }

            QSeedSelectionItem {
                id: random1
                width: 560
                height: 60
                onItemClicked: notification.visible = false
            }

            QSeedSelectionItem {
                id: random2
                width: 560
                height: 60
                onItemClicked: notification.visible = false
            }

            QSeedSelectionItem {
                id: random3
                width: 560
                height: 60
                onItemClicked: notification.visible = false
            }

            Component.onCompleted: {
                var arr = [];
                var mnemomics = textMnemonic.split(' ');
                while(arr.length < 9){
                    var r = Math.floor(Math.random() * 23) + 1;
                    if(arr.indexOf(r) === -1) arr.push(r);
                }

                random1.indexArray = [arr[0], arr[1], arr[2]];
                random1.indexNeeded = random1.indexArray[(Math.floor(Math.random() * 2) + 1)];
                random1.phraseNeeded = mnemomics[random1.indexNeeded];
                random1.phraseArray = [mnemomics[random1.indexArray[0]], mnemomics[random1.indexArray[1]], mnemomics[random1.indexArray[2]] ];

                random2.indexArray = [arr[3], arr[4], arr[5]];
                random2.indexNeeded = random2.indexArray[(Math.floor(Math.random() * 2) + 1)];
                random2.phraseNeeded = mnemomics[random2.indexNeeded];
                random2.phraseArray = [mnemomics[random2.indexArray[0]], mnemomics[random2.indexArray[1]], mnemomics[random2.indexArray[2]] ];

                random3.indexArray = [arr[6], arr[7], arr[8]];
                random3.indexNeeded = random3.indexArray[(Math.floor(Math.random() * 2) + 1)];
                random3.phraseNeeded = mnemomics[random3.indexNeeded];
                random3.phraseArray = [mnemomics[random3.indexArray[0]], mnemomics[random3.indexArray[1]], mnemomics[random3.indexArray[2]] ];
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
                QMLHandle.sendEvent(EVT.EVT_CREATE_NEW_SEED_BACK)
            }
        }

        QTextButton {
            width: 200
            height: 48
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            label.text: "Continue"
            label.font.pixelSize: 16
            type: eTypeA
            onButtonClicked: {
                notification.visible = false
                var valid = (random1.phraseNeeded === random1.phraseSelect) && (random2.phraseNeeded === random2.phraseSelect) && (random3.phraseNeeded === random3.phraseSelect)
                if(valid){
                    QMLHandle.sendEvent(EVT.EVT_CREATE_NEW_SEED_SUCCEED)
                }
                else{
                    notification.visible = true
                }
            }
        }
    }
}
