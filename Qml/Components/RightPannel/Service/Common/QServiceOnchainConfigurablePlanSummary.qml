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
import HMIEVENTS 1.0
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Rectangle {
    id: _summary
    property var planInfo: ({})
    property string walletName: ""
    property bool showShareSecrets: true
    readonly property var policy: planInfo && planInfo.onchain_inheritance_policy ? planInfo.onchain_inheritance_policy : ({})
    readonly property var unlockTimes: planInfo && planInfo.onchain_unlock_times ? planInfo.onchain_unlock_times : []
    readonly property var inheritanceKeys: planInfo && planInfo.inheritance_keys ? planInfo.inheritance_keys : []
    readonly property bool customized: policy.primary !== undefined && policy.primary.is_customized === true
    readonly property bool hasFallbacks: unlockTimes.length > 1
    readonly property int cardWidth: (width - 48 - 12) / 2

    radius: 24
    color: "#2F466C"
    height: _content.implicitHeight + 48

    function requiresText(m, n) {
        if (n === 1) return STR.STR_QML_2493
        if (m === n) return STR.STR_QML_2494.arg(m).arg(n)
        return STR.STR_QML_2495.arg(m).arg(n)
    }
    function keysOf(fallbackIndex) {
        var ret = []
        for (var i = 0; i < inheritanceKeys.length; i++) {
            var k = inheritanceKeys[i]
            if (fallbackIndex < 0 && k.section === "PRIMARY") ret.push(k)
            else if (fallbackIndex >= 0 && k.section === "FALLBACK" && k.fallback_index === fallbackIndex) ret.push(k)
        }
        return ret
    }
    function keyCards(route) {
        var cards = []
        var keys = keysOf(route.fallback_index)
        for (var i = 0; i < keys.length; i++) {
            cards.push({ title: STR.STR_QML_2496.arg(keys[i].name).arg(String(keys[i].xfp).toUpperCase()), subtitle: STR.STR_QML_2497 })
        }
        if (route.fallback_index < 0 && !customized) {
            cards.push({ title: STR.STR_QML_2498.arg(route.n - 1), subtitle: STR.STR_QML_2499 })
        }
        return cards
    }

    component WhiteCard: Rectangle {
        property string titleText: ""
        property string subtitleText: ""
        property string icon: "qrc:/Images/Images/key-dark.svg"
        height: 64
        radius: 12
        color: "#FFFFFF"
        Row {
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
                leftMargin: 12
                rightMargin: 12
            }
            spacing: 12
            QIcon {
                iconSize: 24
                source: parent.parent.icon
            }
            Column {
                width: parent.width - 36
                spacing: 4
                QLato {
                    width: parent.width
                    text: parent.parent.parent.titleText
                    textFormat: Text.RichText
                    color: "#1C1C1C"
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: parent.width
                    text: parent.parent.parent.subtitleText
                    font.pixelSize: 12
                    color: "#1C1C1C"
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }

    Column {
        id: _content
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 24
        }
        spacing: 16
        Item {
            width: parent.width
            height: 60
            Row {
                spacing: 12
                anchors.verticalCenter: parent.verticalCenter
                QIcon {
                    iconSize: 60
                    source: "qrc:/Images/Images/wallet-brand-icon.svg"
                }
                Column {
                    width: 304
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: parent.width
                        font.weight: Font.Bold
                        color: "#FFFFFF"
                        text: STR.STR_QML_845
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        width: parent.width
                        color: "#FFFFFF"
                        text: _summary.walletName
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
            Row {
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                spacing: 12
                QTextLink {
                    width: paintedWidth
                    height: 48
                    font.weight: Font.Bold
                    color: "#FFFFFF"
                    font.underline: false
                    verticalAlignment: Text.AlignVCenter
                    text: STR.STR_QML_846
                    onTextClicked: Qt.openUrlExternally("https://nunchuk.io/howtoclaim")
                }
                QTextButton {
                    visible: _summary.showShareSecrets
                    width: label.paintedWidth + 32
                    height: 48
                    label.text: STR.STR_QML_847
                    label.font.pixelSize: 16
                    type: eTypeC
                    onButtonClicked: QMLHandle.sendEvent(EVT.EVT_SHARE_YOUR_SECRET_REQUEST)
                }
            }
        }
        QLato {
            width: parent.width
            color: "#FFFFFF"
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            text: (_summary.hasFallbacks || _summary.customized) ? STR.STR_QML_2490 : STR.STR_QML_1983
        }
        WhiteCard {
            width: parent.width
            icon: "qrc:/Images/Images/star-dark.png"
            titleText: "<b>" + STR.STR_QML_749 + "</b>"
            subtitleText: _summary.planInfo && _summary.planInfo.magic ? _summary.planInfo.magic : ""
        }
        Repeater {
            model: _summary.unlockTimes
            Column {
                width: _content.width
                spacing: 12
                QLato {
                    font.weight: Font.Bold
                    color: "#FFFFFF"
                    horizontalAlignment: Text.AlignLeft
                    text: modelData.fallback_index < 0 ? STR.STR_QML_2491.arg(_summary.requiresText(modelData.m, modelData.n))
                                                       : STR.STR_QML_2492.arg(modelData.fallback_index + 1).arg(_summary.requiresText(modelData.m, modelData.n))
                }
                Flow {
                    width: parent.width
                    spacing: 12
                    Repeater {
                        model: _summary.keyCards(modelData)
                        WhiteCard {
                            width: _summary.cardWidth
                            titleText: modelData.title
                            subtitleText: modelData.subtitle
                        }
                    }
                }
                QLato {
                    visible: modelData.fallback_index < 0 && !_summary.customized
                    color: "#FFFFFF"
                    horizontalAlignment: Text.AlignLeft
                    text: STR.STR_QML_2500.arg(modelData.n - 1)
                }
                QLato {
                    color: "#FFFFFF"
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    text: STR.STR_QML_2501
                }
                Rectangle {
                    width: parent.width
                    height: 64
                    radius: 12
                    color: "#2A3F61"
                    Row {
                        anchors {
                            left: parent.left
                            leftMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 12
                        QIcon {
                            iconSize: 24
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/calendar-light.svg"
                        }
                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 2
                            QLato {
                                color: "#FFFFFF"
                                text: modelData.is_height_lock ? qsTr("Est. %1").arg(modelData.display) : modelData.display
                            }
                            QLato {
                                font.pixelSize: 12
                                color: "#FFFFFF"
                                text: modelData.is_height_lock ? (modelData.timezone + " · " + STR.STR_QML_188 + " " + modelData.block_height) : modelData.timezone
                            }
                        }
                    }
                }
            }
        }
    }
}
