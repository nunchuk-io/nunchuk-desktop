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
import QtQuick 2.4
// Removed unused imports: QtQuick.Controls, QtGraphicalEffects, HMIEVENTS, EWARNING, and Chats
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Popups"
import "../../OnlineMode/AddHardwareKeys"
import "../../../../localization/STR_QML.js" as STR


QOnScreenContentTypeB {
    property var dashInfo: GroupWallet.dashboardInfo    
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_938
    onCloseClicked: closeScreen()
    content: Item {
        Row {
            anchors.fill: parent
            spacing: 36
            Item {
                width: 346
                height: parent.height
                Column {
                    anchors.fill: parent
                    spacing: 36
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_939.arg(dashInfo.mInfo).arg(dashInfo.nInfo).arg(dashInfo.mInfo === 2 ? STR.STR_QML_939_two : STR.STR_QML_939_three)
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if (dashInfo.allowInheritance)
                            {
                                var user = ClientController.user
                                if (dashInfo.mInfo === 2 && dashInfo.nInfo === 4) {
                                    if (dashInfo.isPremierGroup) {
                                        return STR.STR_QML_940_without
                                    } else {
                                        return STR.STR_QML_940_one
                                    }
                                }
                                else {
                                    if (dashInfo.isPremierGroup) {
                                        return STR.STR_QML_940_without
                                    } else {
                                        return STR.STR_QML_940_two
                                    }
                                }
                            }
                            else {
                                return STR.STR_QML_940_without
                            }
                        }
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Item {
                width: 346
                height: parent.height
                Item {
                    width: parent.width
                    height: 36
                    QLato {
                        width: 96
                        height: 36
                        text: STR.STR_QML_289
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QRefreshButtonA {
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        width: 70
                        height: 36
                        color: "transparent"
                        border.color: "transparent"
                        iconSize: 18
                        iconSpacing: 4
                        label: STR.STR_QML_652
                        fontPixelSize: 12
                        onButtonClicked: {
                            GroupWallet.refresh()
                            stopRefresh()
                        }
                    }
                }
                Rectangle {
                    anchors {
                        top: parent.top
                        topMargin: 36
                    }
                    color: "#FFFFFF"
                    border.color: "#FFEAEA"
                    radius: 12
                    width: 346
                    height: 456
                    Flickable {
                        anchors {
                            fill: parent
                            margins: 12
                        }
                        Column {
                            width: 322
                            spacing: 16
                            Repeater {
                                id: signers
                                model: GroupWallet.dashboardInfo.keys
                                QAddRequestKey {
                                    onTapsignerClicked: {
                                        // _info.contentText = STR.STR_QML_961
                                        // _info.open()
                                        _hardwareAddKey.key_index = modelData.key_index
                                        _hardwareAddKey.isInheritance = true
                                        _hardwareAddKey.open()
                                        dashInfo.startAddKeyAtIndex(index)
                                    }
                                    onHardwareClicked: {
                                        _hardwareAddKey.key_index = modelData.key_index
                                        _hardwareAddKey.isInheritance = false
                                        _hardwareAddKey.open()
                                        dashInfo.startAddKeyAtIndex(index)
                                    }
                                    onSerkeyClicked: {
                                        _info.contentText = STR.STR_QML_962
                                        _info.open()
                                    }
                                    onBackupClicked: {
                                        _importColdcardBackup.xfp = modelData.xfp
                                        _importColdcardBackup.open()
                                        var _input = {
                                            type: "open-import-encrypted-backup",
                                            fingerPrint: modelData.xfp,
                                        }
                                        dashInfo.requestBackupColdcard(_input)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: closeScreen()
    bottomRight: Item{}

    QPopupHardwareAddKey {
        id: _hardwareAddKey
        supportedModel: {
            var ls = []
            if (!isKeyHolderLimited) {
                if (!isInheritance) {
                    ls.push({add_type: NUNCHUCKTYPE.ADD_BITBOX,   txt: "BitBox"   , type: "bitbox02", tag: "BITBOX"  })
                }
                ls.push({add_type: NUNCHUCKTYPE.ADD_COLDCARD, txt: "COLDCARD" , type: "coldcard", tag: "COLDCARD"})
            }
            if (!isInheritance) {
                ls.push({add_type: NUNCHUCKTYPE.ADD_LEDGER,   txt: "Ledger"   , type: "ledger"  , tag: "LEDGER"  })
                ls.push({add_type: NUNCHUCKTYPE.ADD_TREZOR,   txt: "Trezor"   , type: "trezor"  , tag: "TREZOR"  })
            }
            if (isInheritance) {
                ls.push({add_type: NUNCHUCKTYPE.ADD_TAPSIGNER,   txt: "TAPSIGNER"   , type: "TAPSIGNER"  , tag: "INHERITANCE"  })
            }
            if (!isInheritance) {
                ls.push({add_type: NUNCHUCKTYPE.ADD_JADE,   txt: "Blockstream Jade"   , type: "JADE"  , tag: "JADE"  })
            }
            return ls
        }
        onNextClicked: {
            dashInfo.requestStartKeyCreate(hardware)
        }
    }

    QPopupInfo{
        id:_info
        contentText: STR.STR_QML_961
    }

    function importEncryptedBackup(xfp, file) {
        var _input = {
            type: "import-encrypted-backup",
            fingerPrint: xfp,
            currentFile: file
        }
        dashInfo.requestBackupColdcard(_input)
    }

    QPopupImportColdcardBackup {
        id: _importColdcardBackup
    }
}
