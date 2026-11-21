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
 **************************************************************************/
import QtQuick
import QtQuick.Controls
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Popups"

QPopup {
    id: _popup
    property var signersModel: null
    property int addressType: NUNCHUCKTYPE.NATIVE_SEGWIT
    property int walletType: NUNCHUCKTYPE.MULTI_SIG
    property bool singleKey: false
    property bool canRemove: true
    signal removeNonStandardKeys()
    signal createAnyway()

    content: Item {
        width: 480
        height: _col.implicitHeight

        Column {
            id: _col
            width: parent.width
            spacing: 16

            Row {
                width: parent.width
                QLato {
                    width: parent.width - 48
                    text: qsTr("Review key compatibility")
                    font.weight: Font.Bold
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QCloseButton {
                    anchors.verticalCenter: parent.verticalCenter
                    onButtonClicked: _popup.close()
                }
            }

            QLato {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 14
                text: AppSetting.bip32PathWarningCopy(addressType, walletType, singleKey)
            }

            Rectangle {
                width: parent.width
                height: Math.min(signerlist.contentHeight, 360)
                color: "transparent"
                clip: true
                ListView {
                    id: signerlist
                    anchors.fill: parent
                    model: signersModel
                    ScrollBar.vertical: QScrollBar { }
                    delegate: QDraftWalletKeyDeletage {
                        width: signerlist.width - 8
                        signerData {
                            single_name: singleSigner_name
                            single_type: single_signer_type
                            single_tag: single_signer_tag
                            single_devicetype: single_signer_devicetype
                            single_masterFingerPrint: singleSigner_masterFingerPrint
                            single_account_index: single_signer_account_index
                            single_checked: single_signer_checked
                            single_is_local: single_signer_is_local
                            single_value_key: single_signer_value_key
                            single_derivationPath: singleSigner_derivationPath
                            single_device_cardid: single_signer_device_cardid
                            single_need_Topup_Xpub: false
                            single_platformkeyPolicyType: single_signer_platformkeyPolicyType
                        }
                        visible: isNonStandardPath
                        height: isNonStandardPath ? implicitHeight : 0
                    }
                }
            }

            QLato {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 14
                text: qsTr("If you create it anyway, some signing devices may not be able to sign this wallet's transactions, and other wallet apps may not import it.")
            }

            Item {
                width: parent.width
                height: 48
                QTextButton {
                    anchors.left: parent.left
                    width: label.paintedWidth + 16*2
                    height: 48
                    label.text: qsTr("Create anyway")
                    label.font.pixelSize: 16
                    type: eTypeF
                    onButtonClicked: {
                        _popup.close()
                        createAnyway()
                    }
                }
                QTextButton {
                    anchors.right: parent.right
                    width: label.paintedWidth + 16*2
                    height: 48
                    label.text: qsTr("Remove non-standard key%1").arg(singleKey ? "" : "s")
                    label.font.pixelSize: 16
                    type: eTypeE
                    visible: canRemove
                    onButtonClicked: {
                        _popup.close()
                        removeNonStandardKeys()
                    }
                }
            }
        }
    }
}
