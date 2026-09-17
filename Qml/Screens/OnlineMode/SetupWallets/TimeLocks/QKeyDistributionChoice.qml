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
import Qt5Compat.GraphicalEffects
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

// "How will you pass the inheritance key to your Beneficiary?"
// Choice of claim_options (SEED_PHRASE/ENCRYPTED_BACKUP/both), backend-driven per NUN-10192.
QOnScreenContentTypeA {
    id: _root
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2256
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    // BUGFIX: Next used to stay enabled even when selected_option wasn't in availableOptions.
    nextEnable: {
        var item = maps.find(function(e) { return e.id === selected_option })
        return item ? isOptionAvailable(item.options) : false
    }

    // Tag of the signer being added (COLDCARD/LEDGER/JADE/KEYSTONE/...)
    property string signerTag: ""
    property var availableOptions: []
    property string claimNote: ""
    property string selected_option: "SEED_PHRASE"

    signal distributionChosen(var claimOptions)

    readonly property var maps: [
        {id: "SEED_PHRASE",      title: STR.STR_QML_2258, desc: STR.STR_QML_2259, options: ["SEED_PHRASE"]},
        {id: "ENCRYPTED_BACKUP", title: STR.STR_QML_2260, desc: STR.STR_QML_2261, options: ["ENCRYPTED_BACKUP"]},
        {id: "BOTH",             title: STR.STR_QML_2262, desc: STR.STR_QML_2263, options: ["SEED_PHRASE", "ENCRYPTED_BACKUP"]},
    ]

    function isOptionAvailable(opts) {
        for (var i = 0; i < opts.length; i++) {
            if (availableOptions.indexOf(opts[i]) === -1) return false
        }
        return true
    }

    function refresh(tag) {
        signerTag = tag
        // BUGFIX: pass wallet_type so claim_options/claim_note match the right supported_signers[] entry.
        var walletType = SignerManagement.currentSigner.wallet_type !== undefined ? SignerManagement.currentSigner.wallet_type : ""
        availableOptions = SignerManagement.claimOptionsForTag(tag, walletType) || []
        claimNote = SignerManagement.claimNoteForTag(tag, walletType)
        if (!isOptionAvailable(["SEED_PHRASE"])) {
            for (var i = 0; i < maps.length; i++) {
                if (isOptionAvailable(maps[i].options)) {
                    selected_option = maps[i].id
                    break
                }
            }
        } else {
            selected_option = "SEED_PHRASE"
        }
    }

    onNextClicked: {
        var item = maps.find(function(e) { return e.id === selected_option })
        if (item) {
            distributionChosen(item.options)
        }
    }

    content: Item {
        Column {
            width: 539
            spacing: 16
            QLato {
                width: parent.width
                text: STR.STR_QML_2257
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Column {
                width: parent.width
                spacing: 8
                Repeater {
                    model: _root.maps
                    delegate: Rectangle {
                        width: 539
                        height: _descText.paintedHeight + 56
                        radius: 12
                        property bool isAvailable: _root.isOptionAvailable(modelData.options)
                        opacity: isAvailable ? 1.0 : 0.4
                        border.width: 2
                        border.color: _root.selected_option === modelData.id ? "#000000" : "#DEDEDE"
                        Row {
                            anchors {
                                fill: parent
                                margins: 20
                            }
                            spacing: 12
                            QIcon {
                                iconSize: 24
                                anchors.top: parent.top
                                source: _root.selected_option === modelData.id ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                            }
                            Column {
                                width: 467
                                spacing: 4
                                QLato {
                                    width: parent.width
                                    text: modelData.title
                                    font.weight: Font.ExtraBold
                                    font.pixelSize: 16
                                    horizontalAlignment: Text.AlignLeft
                                }
                                QLato {
                                    id: _descText
                                    width: parent.width
                                    text: modelData.desc
                                    font.pixelSize: 13
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                }
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            enabled: parent.isAvailable
                            cursorShape: parent.isAvailable ? Qt.PointingHandCursor : Qt.ArrowCursor
                            onClicked: _root.selected_option = modelData.id
                        }
                    }
                }
            }
            QWarningBgMulti {
                width: 539
                height: 64
                visible: _root.claimNote !== ""
                icon: "qrc:/Images/Images/info-60px.svg"
                txt.text: _root.claimNote
            }
        }
    }
}
