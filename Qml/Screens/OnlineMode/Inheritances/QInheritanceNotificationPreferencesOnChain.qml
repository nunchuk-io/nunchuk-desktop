import QtQuick
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property var inheritance
    property var emails_preference: inheritance.notification_preferences
    readonly property bool isConfigurablePlan: inheritance && inheritance.onchain_inheritance_policy !== undefined
                                               && inheritance.onchain_inheritance_policy.type === "CONFIGURABLE"
    readonly property bool hasFallbackNotices: inheritance && inheritance.onchain_inheritance_policy !== undefined
                                               && inheritance.onchain_inheritance_policy.type === "CONFIGURABLE"
                                               && inheritance.onchain_inheritance_policy.fallbacks !== undefined
                                               && inheritance.onchain_inheritance_policy.fallbacks.length > 0
    property var oldInheritance: null
    readonly property bool markChanges: isConfigurablePlan && oldInheritance !== null && oldInheritance !== undefined
                                        && oldInheritance.notification_preferences !== undefined
    function oldItemOf(email) {
        if (!markChanges) return null
        var list = oldInheritance.notification_preferences.beneficiary_notifications
        if (!list) return null
        for (var i = 0; i < list.length; i++) {
            if (list[i].email === email) return list[i]
        }
        return null
    }
    function sortedFallbacks(item) {
        return item && item.notify_fallbacks !== undefined ? item.notify_fallbacks.slice().sort(function(a, b) { return a - b }) : []
    }
    function changedColor(item, field) {
        if (!markChanges) return "#031F2B"
        var old = oldItemOf(item.email)
        if (!old) return "#CF4018"
        if (field === "unlock") {
            var changed = (old.notify_timelock_expires === true) !== (item.notify_timelock_expires === true)
                          || sortedFallbacks(old).join(",") !== sortedFallbacks(item).join(",")
            return changed ? "#CF4018" : "#031F2B"
        }
        return (old[field] === true) !== (item[field] === true) ? "#CF4018" : "#031F2B"
    }
    function unlockNoticesText(item) {
        var names = []
        if (item.notify_timelock_expires) names.push(STR.STR_QML_2489)
        var fallbacks = item.notify_fallbacks !== undefined ? item.notify_fallbacks.slice().sort(function(a, b) { return a - b }) : []
        for (var i = 0; i < fallbacks.length; i++) names.push(STR.STR_QML_2356.arg(fallbacks[i] + 1))
        return names.length > 0 ? names.join(", ") : STR.STR_QML_432
    }

    implicitHeight: content.implicitHeight

    Column {
        id: content
        anchors.fill: parent
        spacing: 12
        QLato {
            font.weight: Font.Bold
            font.pixelSize: 16
            text: isConfigurablePlan ? STR.STR_QML_2564 : STR.STR_QML_852
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
                            text: hasFallbackNotices ? unlockNoticesText(modelData) : (modelData.notify_timelock_expires ? "Yes" : "No")
                            color: changedColor(modelData, hasFallbackNotices ? "unlock" : "notify_timelock_expires")
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
                            color: changedColor(modelData, "notify_wallet_changes")
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
                            color: changedColor(modelData, "include_wallet_config")
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
        }
    }
}
