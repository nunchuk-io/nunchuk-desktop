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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "./../../customizes/Wallets/miniscript"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: _walletConfig
    spacing: 8
    property bool isEnabled: true
    property int addressType: 0
    property int customizeM: sandbox.groupM
    property int customizeN: sandbox.groupN
    property bool isSetting: false
    property var types: [
        {id: "2_3_multisig",  walletM: 2, walletN: 3, displayName: STR.STR_QML_1550 },
        {id: "3_5_multisig",  walletM: 3, walletN: 5, displayName: STR.STR_QML_1551 },
        {id: "customzie",     walletM: 0, walletN: 0, displayName: STR.STR_QML_1498 },
        {id: "miniscript",    walletM: 0, walletN: 0, displayName: STR.STR_QML_1801 },
    ]
    property var custom: types.find(function(e) { if (e.walletM === customizeM && e.walletN === customizeN) return true; else return false })
    property string option: {
        if (sandbox.walletType === NUNCHUCKTYPE.MINISCRIPT) {
            return "miniscript"
        } else {
            return !isSetting ? "2_3_multisig" : (custom !== undefined ? custom.id : "customzie")
        }
    }

    signal selectMiniscriptTemplate(var templateName)
    signal enterCustomMiniscript(var option)
    signal miniscriptEdit()
    signal miniscriptDelete()
    QLato {
        font.weight: Font.Bold
        font.pixelSize: 12
        text: STR.STR_QML_437
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    Flickable {
        id: flick
        width: _walletConfig.width
        height: 200
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }
        contentHeight: _colWalletConfig.childrenRect.height + 50
        contentWidth: _walletConfig.width
        clip: true
        Column {
            id: _colWalletConfig
            width: _walletConfig.width
            spacing: 8
            Repeater {
                id: repeater
                model: types
                Loader {
                    sourceComponent: {
                        if (index == 2) {
                            return option == "customzie" ? _radioConfigCustomize : _radioConfig
                        } else if (index == 3) {
                            return option === "miniscript" ? _radioConfigCustomizeMiniscript : _radioConfig
                        } else {
                            return _radioConfig
                        }
                    }
                    Component {
                        id: _radioConfig
                        QRadioButtonTypeG {
                            id: btn
                            border.width: 0
                            width: _walletConfig.width
                            height: 40
                            radius: 8
                            label: modelData.displayName
                            fontPixelSize: 16
                            fontWeight: Font.Normal
                            selected: option === modelData.id
                            color: btn.selected ? "#66D0E2FF" : "#FFFFFF"
                            opacity: btn.enabled ? 1.0 : (btn.selected ? 1.0 : 0.4)
                            enabled: {
                                var tmp = isEnabled
                                if (modelData.id === "miniscript") {
                                    tmp = tmp && !(addressType === NUNCHUCKTYPE.NESTED_SEGWIT || addressType === NUNCHUCKTYPE.LEGACY)
                                }
                                return tmp

                            }
                            textBadge: modelData.id === "miniscript" ? STR.STR_QML_1548 : ""
                            onButtonClicked: {
                                option = modelData.id
                                if (modelData.id === "miniscript") {
                                    var lastItem = repeater.itemAt(repeater.count - 1)
                                    var pos = lastItem.mapToItem(flick.contentItem, 0, 0)
                                    flick.contentY = pos.y
                                }
                            }
                        }
                    }
                    Component {
                        id: _radioConfigCustomize
                        QWalletConfigCustomize {
                            id: btn
                            width: _walletConfig.width
                            height: 192
                            radius: 8
                            selected: option === modelData.id
                            opacity: isEnabled ? 1.0 : (btn.selected ? 1.0 : 0.4)
                            enabled: isEnabled
                        }
                    }
                    Component {
                        id: _radioConfigCustomizeMiniscript
                        QWalletConfigCustomizeMiniscript {
                            id: btn
                            width: _walletConfig.width
                            height: 192
                            radius: 8
                            selected: option === modelData.id
                            opacity: btn.enabled ? 1.0 : (btn.selected ? 1.0 : 0.4)
                            enabled: isEnabled
                            miniscript: newWalletInfo.customizeMiniscript
                            onSelectMiniscriptTemplate: (templateName) => {
                                _walletConfig.selectMiniscriptTemplate(templateName)
                            }
                            onEnterCustomMiniscript: (option) => {
                                _walletConfig.enterCustomMiniscript(option)
                            }
                            onMiniscriptEdit: () => {
                                _walletConfig.miniscriptEdit()
                            }
                            onMiniscriptDelete: () => {
                                _walletConfig.miniscriptDelete()
                            }
                        }
                    }
                }
            }
        }
    }

    function findOpt() {
        if (option == "customzie") {
            return {id: "customzie",     walletM: customizeM, walletN: customizeN, displayName: STR.STR_QML_1498 }
        } else {
            return types.find(function(e) { if (e.id === option) return true; else return false })
        }
    }
}
