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
import QtQuick
import QtQuick.Controls
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Signers"
import "../../../../../localization/STR_QML.js" as STR

Column {
    id: _policy
    property var dashInfo
    property bool isKeyHolderLimited: false
    property string primaryDateText: ""
    property string primarySubText: ""
    readonly property var primary: dashInfo ? dashInfo.primaryPolicy : ({})
    readonly property bool isCustomized: primary && primary.is_customized === true
    readonly property var fallbacks: dashInfo ? dashInfo.fallbackPolicies : []
    readonly property var generalErrors: dashInfo ? dashInfo.generalValidationErrors : []

    signal keyInheritanceClicked(var key)
    signal keyHardwareClicked(var key)
    signal keyServerClicked(var key)
    signal keyBackupClicked(var key)
    signal keyChangeClicked(var key)
    signal refreshClicked()
    signal primaryTimelockClicked()
    signal customizePrimaryClicked()
    signal addFallbackClicked()
    signal editFallbackClicked(var fallback)
    signal removeFallbackClicked(var fallback)
    signal fallbackTimelockClicked(var fallback)

    spacing: 12

    function keysOf(section, fallbackIndex) {
        var all = dashInfo ? dashInfo.keys : []
        var ret = []
        for (var i = 0; i < all.length; i++) {
            if (all[i].section !== section) continue
            if (section === "FALLBACK" && all[i].fallback_index !== fallbackIndex) continue
            ret.push(all[i])
        }
        return ret
    }
    function requiresText(m, n) {
        if (n === 1) return STR.STR_QML_2352
        if (m === n) return STR.STR_QML_2361.arg(n)
        return STR.STR_QML_2351.arg(m).arg(n)
    }
    function afterTimelockText() {
        if (!isCustomized) return STR.STR_QML_2347
        if (primary.m === 1) return STR.STR_QML_2349.arg(primary.n)
        var words = STR.STR_QML_2374.split(",")
        var word = primary.m >= 1 && primary.m <= words.length ? words[primary.m - 1] : primary.m
        return STR.STR_QML_2348.arg(word).arg(primary.m).arg(primary.n)
    }

    component ErrorMessages: Column {
        id: _errors
        property var messages: []
        width: parent.width
        spacing: 4
        visible: messages.length > 0
        Repeater {
            model: _errors.messages
            QLato {
                width: _errors.width
                text: modelData
                color: "#CF4018"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
        }
    }

    QLato {
        width: parent.width
        text: STR.STR_QML_1889
        lineHeightMode: Text.FixedHeight
        lineHeight: 20
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft
    }
    QOnChainKeyList {
        isKeyHolderLimited: _policy.isKeyHolderLimited
        model: _policy.keysOf("SPENDING", -1)
        onInheritanceClicked: function(key) { _policy.keyInheritanceClicked(key) }
        onHardwareClicked: function(key) { _policy.keyHardwareClicked(key) }
        onServerClicked: function(key) { _policy.keyServerClicked(key) }
        onBackupClicked: function(key) { _policy.keyBackupClicked(key) }
        onChangeClicked: function(key) { _policy.keyChangeClicked(key) }
    }
    Item {
        width: parent.width
        height: 44
        QRefreshButtonA {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 70
            height: 36
            color: "transparent"
            border.color: "transparent"
            iconSize: 18
            iconSpacing: 4
            label: STR.STR_QML_652
            fontPixelSize: 12
            onButtonClicked: {
                _policy.refreshClicked()
                stopRefresh()
            }
        }
    }
    QLato {
        width: parent.width
        text: _policy.afterTimelockText()
        lineHeightMode: Text.FixedHeight
        lineHeight: 20
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft
    }
    Item {
        width: parent.width
        height: _primaryTitle.height + (_primaryRequires.visible ? _primaryRequires.height + 4 : 0)
        Column {
            spacing: 4
            QLato {
                id: _primaryTitle
                text: STR.STR_QML_2350
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
            }
            QLato {
                id: _primaryRequires
                visible: _policy.isCustomized
                text: _policy.requiresText(_policy.primary.m, _policy.primary.n)
                font.pixelSize: 14
                horizontalAlignment: Text.AlignLeft
            }
        }
        Rectangle {
            anchors {
                right: parent.right
                top: parent.top
            }
            width: _badgeText.paintedWidth + 16
            height: 24
            radius: 4
            color: "#CF4018"
            QLato {
                id: _badgeText
                anchors.centerIn: parent
                text: STR.STR_QML_1600
                color: "#FFFFFF"
                font.pixelSize: 14
            }
        }
    }
    QOnChainKeyList {
        isKeyHolderLimited: _policy.isKeyHolderLimited
        model: _policy.keysOf("PRIMARY", -1)
        onInheritanceClicked: function(key) { _policy.keyInheritanceClicked(key) }
        onHardwareClicked: function(key) { _policy.keyHardwareClicked(key) }
        onServerClicked: function(key) { _policy.keyServerClicked(key) }
        onBackupClicked: function(key) { _policy.keyBackupClicked(key) }
        onChangeClicked: function(key) { _policy.keyChangeClicked(key) }
    }
    QOnChainTimelockCard {
        width: parent.width
        dateText: _policy.primaryDateText
        subText: _policy.primarySubText
        hasError: _policy.primary.timelock_error === true
        onConfigureClicked: _policy.primaryTimelockClicked()
    }
    ErrorMessages {
        messages: _policy.primary.messages !== undefined ? _policy.primary.messages : []
    }
    QTextLink {
        width: paintedWidth
        height: 20
        text: _policy.isCustomized ? STR.STR_QML_849 : STR.STR_QML_2353
        onTextClicked: _policy.customizePrimaryClicked()
    }
    Rectangle {
        width: parent.width
        height: 1
        color: "#DEDEDE"
    }
    QLato {
        width: parent.width
        text: STR.STR_QML_2354
        font.weight: Font.Bold
        font.pixelSize: 20
        horizontalAlignment: Text.AlignLeft
    }
    QLato {
        width: parent.width
        text: STR.STR_QML_2355
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft
    }
    Repeater {
        model: _policy.fallbacks
        Column {
            id: _fallback
            property var fallback: modelData
            width: _policy.width
            spacing: 12
            Item {
                width: parent.width
                height: _fbTitle.height + _fbRequires.height + 4
                Column {
                    spacing: 4
                    QLato {
                        id: _fbTitle
                        text: STR.STR_QML_2356.arg(_fallback.fallback.index + 1)
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        id: _fbRequires
                        text: _policy.requiresText(_fallback.fallback.m, _fallback.fallback.n)
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignLeft
                    }
                }
                Row {
                    anchors {
                        right: parent.right
                        top: parent.top
                    }
                    spacing: 24
                    QTextLink {
                        width: paintedWidth
                        height: 20
                        text: STR.STR_QML_849
                        onTextClicked: _policy.editFallbackClicked(_fallback.fallback)
                    }
                    QTextLink {
                        width: paintedWidth
                        height: 20
                        text: STR.STR_QML_1134
                        onTextClicked: _policy.removeFallbackClicked(_fallback.fallback)
                    }
                }
            }
            QOnChainKeyList {
                isKeyHolderLimited: _policy.isKeyHolderLimited
                model: _policy.keysOf("FALLBACK", _fallback.fallback.index)
                onInheritanceClicked: function(key) { _policy.keyInheritanceClicked(key) }
                onHardwareClicked: function(key) { _policy.keyHardwareClicked(key) }
                onServerClicked: function(key) { _policy.keyServerClicked(key) }
                onBackupClicked: function(key) { _policy.keyBackupClicked(key) }
                onChangeClicked: function(key) { _policy.keyChangeClicked(key) }
            }
            QOnChainTimelockCard {
                width: parent.width
                dateText: _fallback.fallback.timelock && _fallback.fallback.timelock.display ? _fallback.fallback.timelock.display : ""
                subText: _fallback.fallback.timelock && _fallback.fallback.timelock.isHeightLock ? qsTr("%1 %2").arg(_fallback.fallback.timelock.block_height).arg(STR.STR_QML_188) : ""
                hasError: _fallback.fallback.timelock_error === true
                onConfigureClicked: _policy.fallbackTimelockClicked(_fallback.fallback)
            }
            ErrorMessages {
                messages: _fallback.fallback.messages !== undefined ? _fallback.fallback.messages : []
            }
        }
    }
    QIconTextButton {
        width: parent.width
        height: 48
        label: _policy.fallbacks.length > 0 ? STR.STR_QML_2372 : STR.STR_QML_2357
        icons: ["add-dark.svg", "add-dark.svg", "add-dark.svg", "add-dark.svg"]
        fontPixelSize: 16
        iconSize: 16
        type: eTypeB
        onButtonClicked: _policy.addFallbackClicked()
    }
    ErrorMessages {
        messages: _policy.generalErrors
    }
}
