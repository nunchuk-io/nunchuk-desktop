import QtQuick
import "../../../../Components/origins"
import "../../../../Components/customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

Column {
    id: _item
    width: parent.width
    spacing: 12

    property var inheritance
    property var emails_preference: inheritance.notification_preferences
    readonly property bool isConfigurablePlan: inheritance && inheritance.onchain_inheritance_policy !== undefined
                                               && inheritance.onchain_inheritance_policy.type === "CONFIGURABLE"
    readonly property bool hasFallbackNotices: inheritance && inheritance.onchain_inheritance_policy !== undefined
                                               && inheritance.onchain_inheritance_policy.type === "CONFIGURABLE"
                                               && inheritance.onchain_inheritance_policy.fallbacks !== undefined
                                               && inheritance.onchain_inheritance_policy.fallbacks.length > 0
    function unlockNoticesText(item) {
        var names = []
        if (item.notify_timelock_expires) names.push(STR.STR_QML_2489)
        var fallbacks = item.notify_fallbacks !== undefined ? item.notify_fallbacks.slice().sort(function(a, b) { return a - b }) : []
        for (var i = 0; i < fallbacks.length; i++) names.push(STR.STR_QML_2356.arg(fallbacks[i] + 1))
        return names.length > 0 ? names.join(", ") : STR.STR_QML_432
    }

    signal textEditClicked()
    Item {
        width: _item.width
        height: 20
        QLato {
            font.weight: Font.Bold
            font.pixelSize: 16
            text: isConfigurablePlan ? STR.STR_QML_2564 : STR.STR_QML_852
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
                        width: parent.width - (hasFallbackNotices ? 220 : 60)
                        height: 20
                        horizontalAlignment: Text.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                        text: hasFallbackNotices ? STR.STR_QML_2488 : STR.STR_QML_2020
                    }
                    QLato {
                        width: hasFallbackNotices ? 220 : 60
                        font.weight: Font.Bold
                        anchors.verticalCenter: parent.verticalCenter
                        text: hasFallbackNotices ? unlockNoticesText(emails_preference.beneficiary_notifications[index]) : (emails_preference.beneficiary_notifications[index].notify_timelock_expires ? "Yes" : "No")
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
