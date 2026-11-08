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
    readonly property var policy: dashInfo ? dashInfo.replacePolicy : ({})
    readonly property var primary: policy && policy.primary ? policy.primary : ({})
    readonly property bool isCustomized: primary.is_customized === true
    readonly property var fallbacks: policy && policy.fallbacks ? policy.fallbacks : []
    readonly property var generalErrors: policy && policy.general_errors ? policy.general_errors : []

    signal keyInheritanceClicked(var key)
    signal keyHardwareClicked(var key)
    signal keyServerClicked(var key)
    signal keyBackupClicked(var key)
    signal keyRemoveClicked(var key)
    signal refreshClicked()
    signal timelockClicked(int fallbackIndex)

    spacing: 12

    component ErrorLines: Column {
        id: _errLines
        property var messages: []
        width: _policy.width
        spacing: 4
        visible: messages.length > 0
        Repeater {
            model: _errLines.messages
            QLato {
                width: _errLines.width
                text: modelData
                color: "#CF4018"
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
        }
    }

    function keysOf(section, fallbackIndex) {
        var all = dashInfo ? dashInfo.replaceKeys : []
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
        if (m === n) return STR.STR_QML_2361.arg(m).arg(n)
        return STR.STR_QML_2351.arg(m).arg(n)
    }
    function afterTimelockText() {
        if (!isCustomized) return STR.STR_QML_2347
        if (primary.m === 1) return STR.STR_QML_2349.arg(primary.n)
        var words = STR.STR_QML_2374.split(",")
        var word = primary.m >= 1 && primary.m <= words.length ? words[primary.m - 1] : primary.m
        return STR.STR_QML_2348.arg(word).arg(primary.m).arg(primary.n)
    }

    QLato {
        width: parent.width
        text: STR.STR_QML_2570
        lineHeightMode: Text.FixedHeight
        lineHeight: 20
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft
    }
    QOnChainReplaceKeyList {
        isKeyHolderLimited: _policy.isKeyHolderLimited
        onInheritanceClicked: function(key) { _policy.keyInheritanceClicked(key) }
        onHardwareClicked: function(key) { _policy.keyHardwareClicked(key) }
        onServerClicked: function(key) { _policy.keyServerClicked(key) }
        onBackupClicked: function(key) { _policy.keyBackupClicked(key) }
        onRemoveClicked: function(key) { _policy.keyRemoveClicked(key) }
        model: _policy.keysOf("SPENDING", -1)
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
                font.pixelSize: 12
                color: "#595959"
                horizontalAlignment: Text.AlignLeft
            }
        }
        Rectangle {
            anchors {
                right: parent.right
                top: parent.top
            }
            width: _badgeText.paintedWidth + 12
            height: 20
            radius: 4
            color: "#CF4018"
            QLato {
                id: _badgeText
                anchors.centerIn: parent
                text: STR.STR_QML_1600
                color: "#FFFFFF"
                font.pixelSize: 12
                font.weight: Font.Bold
            }
        }
    }
    QOnChainReplaceKeyList {
        isKeyHolderLimited: _policy.isKeyHolderLimited
        onInheritanceClicked: function(key) { _policy.keyInheritanceClicked(key) }
        onHardwareClicked: function(key) { _policy.keyHardwareClicked(key) }
        onServerClicked: function(key) { _policy.keyServerClicked(key) }
        onBackupClicked: function(key) { _policy.keyBackupClicked(key) }
        onRemoveClicked: function(key) { _policy.keyRemoveClicked(key) }
        model: _policy.keysOf("PRIMARY", -1)
    }
    ErrorLines {
        messages: _policy.primary.key_messages !== undefined ? _policy.primary.key_messages : []
    }
    QOnChainReplaceTimelockCard {
        width: parent.width
        lockState: _policy.primary
        onChangeClicked: _policy.timelockClicked(-1)
    }
    Rectangle {
        width: parent.width
        height: 1
        color: "#DEDEDE"
        visible: _policy.fallbacks.length > 0
    }
    QLato {
        width: parent.width
        visible: _policy.fallbacks.length > 0
        text: STR.STR_QML_2354
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
    }
    Repeater {
        model: _policy.fallbacks
        Column {
            id: _fallback
            property var fallback: modelData
            width: _policy.width
            spacing: 12
            Column {
                spacing: 4
                QLato {
                    text: STR.STR_QML_2356.arg(_fallback.fallback.index + 1)
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    text: _policy.requiresText(_fallback.fallback.m, _fallback.fallback.n)
                    font.pixelSize: 12
                    color: "#595959"
                    horizontalAlignment: Text.AlignLeft
                }
            }
            QOnChainReplaceKeyList {
                isKeyHolderLimited: _policy.isKeyHolderLimited
                onInheritanceClicked: function(key) { _policy.keyInheritanceClicked(key) }
                onHardwareClicked: function(key) { _policy.keyHardwareClicked(key) }
                onServerClicked: function(key) { _policy.keyServerClicked(key) }
                onBackupClicked: function(key) { _policy.keyBackupClicked(key) }
                onRemoveClicked: function(key) { _policy.keyRemoveClicked(key) }
                model: _policy.keysOf("FALLBACK", _fallback.fallback.index)
            }
            ErrorLines {
                messages: _fallback.fallback.key_messages !== undefined ? _fallback.fallback.key_messages : []
            }
            QOnChainReplaceTimelockCard {
                width: parent.width
                lockState: _fallback.fallback
                onChangeClicked: _policy.timelockClicked(_fallback.fallback.index)
            }
        }
    }
    Repeater {
        model: _policy.generalErrors
        QLato {
            width: _policy.width
            text: modelData
            color: "#CF4018"
            font.pixelSize: 12
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
        }
    }
}
