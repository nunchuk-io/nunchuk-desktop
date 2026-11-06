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
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _verify
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2445
    property var dashInfo
    signal verifyKeyClicked(var key)

    function keysOf(predicate) {
        var all = dashInfo ? dashInfo.keys : []
        var ret = []
        for (var i = 0; i < all.length; i++) {
            if (predicate(all[i])) ret.push(all[i])
        }
        return ret
    }
    readonly property var inheritanceKeys: keysOf(function(k) { return k.section === "PRIMARY" && k.has === true })
    readonly property var otherKeys: keysOf(function(k) { return k.section === "SPENDING" && k.has === true && k.type !== "SERVER" && k.signer_type !== NUNCHUCKTYPE.SERVER })

    component KeyRow: Item {
        id: _row
        property var key: ({})
        signal verifyClicked()
        width: parent ? parent.width : 0
        height: 64
        readonly property bool verified: key.verification_type !== undefined && key.verification_type !== "NONE"
        Row {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 12
            QCircleIcon {
                bgSize: 48
                icon.iconSize: 24
                icon.typeStr: _row.key.type
                icon.tag: _row.key.tag
                anchors.verticalCenter: parent.verticalCenter
                color: "#F5F5F5"
            }
            Column {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                QLato {
                    text: _row.key.name !== undefined ? _row.key.name : ""
                    horizontalAlignment: Text.AlignLeft
                }
                Row {
                    spacing: 4
                    Rectangle {
                        width: _typeText.width + 8
                        height: 16
                        radius: 20
                        color: "#EAEAEA"
                        QText {
                            id: _typeText
                            anchors.centerIn: parent
                            text: GlobalData.signers(_row.key.signer_type)
                            font.family: "Lato"
                            font.weight: Font.Bold
                            font.pixelSize: 10
                            color: "#031F2B"
                        }
                    }
                    QAccountIndexs {
                        height: 16
                        accountIndexs: _row.key.account_indexs !== undefined ? _row.key.account_indexs : []
                        walletType: _row.key.wallet_type
                    }
                }
                QLato {
                    text: "XFP: " + (_row.key.xfp !== undefined ? _row.key.xfp : "")
                    font.pixelSize: 12
                    font.capitalization: Font.AllUppercase
                    color: "#595959"
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
        QTextButton {
            visible: !_row.verified
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_1964
            label.font.pixelSize: 16
            onButtonClicked: _row.verifyClicked()
        }
        QBadge {
            visible: _row.verified
            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            width: 90
            height: 24
            iconSize: 24
            icon: "qrc:/Images/Images/check-circle-dark.svg"
            text: STR.STR_QML_2279
            color: "transparent"
        }
    }

    content: Item {
        Flickable {
            anchors.fill: parent
            contentHeight: _col.height
            clip: true
            ScrollBar.vertical: QScrollBar { }
            Column {
                id: _col
                width: parent.width - 12
                spacing: 12
                QLato {
                    width: parent.width
                    text: STR.STR_QML_2446
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: parent.width
                    text: STR.STR_QML_2447
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                }
                Repeater {
                    model: _verify.inheritanceKeys
                    KeyRow {
                        width: _col.width
                        key: modelData
                        onVerifyClicked: _verify.verifyKeyClicked(modelData)
                    }
                }
                QLato {
                    width: parent.width
                    text: STR.STR_QML_2448
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: parent.width
                    text: _verify.otherKeys.length > 0 ? STR.STR_QML_2449 : STR.STR_QML_2450
                    color: "#595959"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                Repeater {
                    model: _verify.otherKeys
                    KeyRow {
                        width: _col.width
                        key: modelData
                        onVerifyClicked: _verify.verifyKeyClicked(modelData)
                    }
                }
            }
        }
    }

    bottomLeft: QTextButton {
        width: 80
        height: 48
        label.text: STR.STR_QML_059
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: _verify.prevClicked()
    }
    bottomRight: QTextButton {
        width: label.paintedWidth + 2*20
        height: 48
        label.text: STR.STR_QML_777
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: _verify.nextClicked()
    }
}
