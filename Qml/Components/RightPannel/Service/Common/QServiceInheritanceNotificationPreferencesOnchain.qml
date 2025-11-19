import QtQuick 2.0
import "../../../../Components/origins"
import "../../../../Components/customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

Column {
    id: _item
    width: parent.width
    spacing: 12

    property var inheritance
    property var emails_preference: inheritance.notification_preferences

    signal textEditClicked()
    Item {
        width: _item.width
        height: 20
        QLato {
            font.weight: Font.Bold
            font.pixelSize: 16
            text: STR.STR_QML_852
            anchors.left: parent.left
        }
        QTextLink {
            width: 29
            height: 20
            anchors.right: parent.right
            text: STR.STR_QML_849
            color: "#031F2B"
            anchors.verticalCenter: parent.verticalCenter
            onTextClicked: {
                textEditClicked()
            }
        }
    }

    Rectangle {
        width: _item.width
        height: myemailItems.implicitHeight+32
        radius: 12
        color: "#F5F5F5"
        Column {
            id: myemailItems
            width: parent.width - 32
            spacing: 16
            anchors.centerIn: parent
            QLato {
                text: ClientController.user.email
                width: parent.width
                elide: Text.ElideRight
                font.weight: Font.Bold
            }
            Rectangle {
                width: 311
                height: 1
                color: "#EAEAEA"
            }
            Row {
                width: parent.width
                QLato {
                    width: parent.width - 60
                    height: 20
                    horizontalAlignment: Text.AlignLeft
                    anchors.verticalCenter: parent.verticalCenter
                    text: STR.STR_QML_2019
                }
                QLato {
                    width: 60
                    font.weight: Font.Bold
                    anchors.verticalCenter: parent.verticalCenter
                    text: emails_preference.email_me_wallet_config ? "Yes" : "No"
                    color: "#031F2B"
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }

    Repeater {
        model: emails_preference.beneficiary_notifications.length
        Rectangle {
            width: _item.width
            height: emailItems.implicitHeight+32
            radius: 12
            color: "#F5F5F5"
            Column {
                id: emailItems
                width: parent.width - 32
                spacing: 16
                anchors.centerIn: parent
                QLato {
                    text: emails_preference.beneficiary_notifications[index].email
                    width: parent.width
                    elide: Text.ElideRight
                    font.weight: Font.Bold
                }
                Rectangle {
                    width: 311
                    height: 1
                    color: "#EAEAEA"
                }
                Row {
                    width: parent.width
                    QLato {
                        width: parent.width - 60
                        height: 20
                        horizontalAlignment: Text.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                        text: STR.STR_QML_2020
                    }
                    QLato {
                        width: 60
                        font.weight: Font.Bold
                        anchors.verticalCenter: parent.verticalCenter
                        text: emails_preference.beneficiary_notifications[index].notify_timelock_expires? "Yes" : "No"
                        color: "#031F2B"
                        horizontalAlignment: Text.AlignRight
                    }
                }
                Row {
                    width: parent.width
                    QLato {
                        width: parent.width - 60
                        height: 20
                        horizontalAlignment: Text.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                        text: STR.STR_QML_2021
                    }
                    QLato {
                        width: 60
                        font.weight: Font.Bold
                        anchors.verticalCenter: parent.verticalCenter
                        text: emails_preference.beneficiary_notifications[index].notify_wallet_changes? "Yes" : "No"
                        color: "#031F2B"
                        horizontalAlignment: Text.AlignRight
                    }
                }
                Row {
                    width: parent.width
                    QLato {
                        width: parent.width - 60
                        height: 20
                        horizontalAlignment: Text.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                        text: STR.STR_QML_2022
                    }
                    QLato {
                        width: 60
                        font.weight: Font.Bold
                        anchors.verticalCenter: parent.verticalCenter
                        text: emails_preference.beneficiary_notifications[index].include_wallet_config? "Yes" : "No"
                        color: "#031F2B"
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}
