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
        id: contentMasked
        anchors.fill: content
        source: content
        maskSource: mask

        Loader {
            id: loadercontent
            anchors.fill: parent
            sourceComponent: maincontent
        }

        Component {
            id: maincontent
            Item {
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
                        QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_NEW_SIGNER)
                    }
                }
                Row {
                    spacing: 48
                    anchors.top: parent.top
                    anchors.topMargin: 122
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item {
                        width: 246
                        height: 396
                        Column {
                            spacing: 4
                            Rectangle {
                                width: 96
                                height: 96
                                radius: width
                                color: "#F5F5F5"
                                anchors.horizontalCenter: parent.horizontalCenter
                                QImage {
                                    width: 60
                                    height: 60
                                    anchors.centerIn: parent
                                    source: "qrc:/Images/Images/hardware_add-60px.png"
                                }
                            }
                            QText {
                                width: 210
                                height: 56
                                text: "Add hardware signer"
                                horizontalAlignment: Text.AlignHCenter
                                font.family: "Lato"
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                wrapMode: Text.WordWrap
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QText {
                                width: 210
                                height: 112
                                text: "Add your existing hardware signers to Nunchuk. Wired and air-gapped signers supported."
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Light
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QTextButton {
                                width: 180
                                height: 48
                                label.text: "Add Hardware Signer"
                                label.font.pixelSize: 16
                                label.font.family: "Lato"
                                type: eTypeB
                                anchors.horizontalCenter: parent.horizontalCenter
                                onButtonClicked: {
                                    loadercontent.sourceComponent = beforeStartHardware
                                }
                            }
                        }
                    }
                    Item {
                        width: 246
                        height: 450
                        Column {
                            spacing: 4
                            Rectangle {
                                width: 96
                                height: 96
                                radius: width
                                color: "#F5F5F5"
                                anchors.horizontalCenter: parent.horizontalCenter
                                QImage {
                                    width: 60
                                    height: 60
                                    anchors.centerIn: parent
                                    source: "qrc:/Images/Images/software_add-60px.png"
                                }
                            }
                            QText {
                                width: 210
                                height: 56
                                text: "Add Nunchuk software signer"
                                horizontalAlignment: Text.AlignHCenter
                                font.family: "Lato"
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                wrapMode: Text.WordWrap
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QText {
                                width: 210
                                height: 112
                                text: "Nunchuk software signer is a companion signer that is managed by the app itself."
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Light
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QTextButton {
                                width: 180
                                height: 48
                                label.text: enabled ? "Add Software Signer" : "Already Added"
                                label.font.pixelSize: 16
                                label.font.family: "Lato"
                                type: eTypeB
                                anchors.horizontalCenter: parent.horizontalCenter
                                enabled: AppModel.limitSoftwareSigner ? (AppModel.softwareSignerDeviceList.count === 0) : true
                                onButtonClicked: {
                                    loadercontent.sourceComponent = beforeStartSoftware
                                }
                            }
                        }
                    }
                }
            }
        }

        Component {
            id: beforeStartHardware
            Item {
                QText {
                    width: 163
                    height: 27
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 24
                    }
                    text: "Before you start"
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
                        loadercontent.sourceComponent = maincontent
                    }
                }

                Column {
                    spacing: 24
                    width: 536
                    height: 336
                    anchors.top: parent.top
                    anchors.topMargin: 144
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle {
                        width: 96
                        height: 96
                        radius: width
                        color: "#F5F5F5"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QImage {
                            width: 60
                            height: 60
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/info-60px.png"
                        }
                    }
                    QText {
                        width: 500
                        height: 28
                        text: "Please make sure that you have done the following."
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                    }

                    Column {
                        id: guide
                        spacing: 16
                        anchors.horizontalCenter: parent.horizontalCenter
                        Item {
                            width: 536
                            height: 84
                            Rectangle {
                                id:guidenum1
                                width: 24
                                height: 24
                                radius: width
                                color: "#031F2B"
                                border.width: 2
                                border.color: "#031F2B"
                                QText {
                                    anchors.centerIn: parent
                                    color: "#FFFFFF"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    text: "1"
                                }
                            }

                            Column {
                                width: parent.width - 28
                                anchors.left: guidenum1.right
                                anchors.leftMargin: 8
                                spacing: 8
                                QText {
                                    width: parent.width
                                    color: "#031F2B"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    font.family: "Montserrat"
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: "Initialize your signer device"
                                }
                                QText {
                                    width: parent.width
                                    color: "#323E4A"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    wrapMode: Text.WordWrap
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: "Follow your vendor's instruction on how to set up a device with a seed."
                                }
                            }
                        }

                        Item {
                            width: 536
                            height: 84
                            Rectangle {
                                id:guidenum2
                                width: 24
                                height: 24
                                radius: width
                                color: "#031F2B"
                                border.width: 2
                                border.color: "#031F2B"
                                QText {
                                    anchors.centerIn: parent
                                    color: "#FFFFFF"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    text: "2"
                                }
                            }

                            Column {
                                width: parent.width - 28
                                anchors.left: guidenum2.right
                                anchors.leftMargin: 8
                                spacing: 8

                                QText {
                                    width: parent.width
                                    color: "#031F2B"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    font.family: "Montserrat"
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: "Ensure your device is plugged in and unlocked"
                                }

                                QText {
                                    width: parent.width
                                    color: "#323E4A"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    wrapMode: Text.WordWrap
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: "You might need to enter a device <b>PIN</b> and/or a <b>PASSPHRASE</b> to unlock your device, if they are enabled."
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
                        bottomMargin: 40
                    }
                    onButtonClicked: {
                        loadercontent.sourceComponent = maincontent
                    }
                }

                QTextButton {
                    width: 200
                    height: 48
                    label.text: "Continue"
                    label.font.pixelSize: 16
                    type: eTypeA
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_REQUEST)
                    }
                }
            }
        }

        Component {
            id: beforeStartSoftware
            Item {
                QText {
                    width: 163
                    height: 27
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 24
                    }
                    text: "Before you start"
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
                        loadercontent.sourceComponent = maincontent
                    }
                }

                Column {
                    spacing: 24
                    width: 536
                    height: 336
                    anchors.top: parent.top
                    anchors.topMargin: 144
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle {
                        width: 96
                        height: 96
                        radius: width
                        color: "#F5F5F5"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QImage {
                            width: 60
                            height: 60
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/warning_amber-60px.png"
                        }
                    }
                    QText {
                        width: 500
                        height: 28
                        text: "Warning: This is a companion software signer that we provide for your convenience. Please use with caution. Generally speaking, software signers offer weaker security guarantees than hardware signers, and should be used in conjunction with hardware signers."
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
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
                        loadercontent.sourceComponent = maincontent
                    }
                }

                Row {
                    height: 48
                    spacing: 16
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    QTextButton {
                        width: 280
                        height: 48
                        label.text: "Recover Signer With Mnemonic"
                        label.font.pixelSize: 16
                        type: eTypeB
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED)
                        }
                    }

                    QTextButton {
                        width: 200
                        height: 48
                        label.text: "Create New Signer"
                        label.font.pixelSize: 16
                        type: eTypeA
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED)
                        }
                    }
                }
            }
        }
    }
}
