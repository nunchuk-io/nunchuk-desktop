import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property var inheritance
    property var emails_preference: inheritance.notification_preferences

    implicitHeight: content.implicitHeight

    Column {
        id: content
        anchors.fill: parent
        spacing: 12
        QLato {
            font.weight: Font.Bold
            font.pixelSize: 16
            text: STR.STR_QML_852
        }

        Rectangle {
            width: _item.width
            height: ownerItems.implicitHeight + 32
            radius: 12
            color: "#F5F5F5"
            Column {
                id: ownerItems
                width: parent.width - 32
                spacing: 16
                anchors.centerIn: parent
                QLato {
                    text: ClientController.user.email
                    width: parent.width
                    elide: Text.ElideRight
                    font.weight: Font.Bold
                }
                Rectangle { width: 311; height: 1; color: "#EAEAEA" }
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
            model: emails_preference && emails_preference.beneficiary_notifications ? emails_preference.beneficiary_notifications : []
            Rectangle {
                width: _item.width
                height: emailItems.implicitHeight + 32
                radius: 12
                color: "#F5F5F5"
                Column {
                    id: emailItems
                    width: parent.width - 32
                    spacing: 16
                    anchors.centerIn: parent
                    QLato {
                        text: modelData.email
                        width: parent.width
                        elide: Text.ElideRight
                        font.weight: Font.Bold
                    }
                    Rectangle { width: 311; height: 1; color: "#EAEAEA" }
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
                            text: modelData.notify_timelock_expires ? "Yes" : "No"
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
                            text: modelData.notify_wallet_changes ? "Yes" : "No"
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
                            text: modelData.include_wallet_config ? "Yes" : "No"
                            color: "#031F2B"
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
        }
    }
}
