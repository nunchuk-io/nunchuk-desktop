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
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    height: 84 + (model.single_signer_isReplaced ? (16 + 8) : 0)
    signal replaceClicked()
    Column {
        id: _col
        width: parent.width
        spacing: 8
        Rectangle {
            height: 84
            width: parent.width
            color: model.single_signer_isReplaced ? "#A7F0BA" : "#FDEBD2"
            radius: 12
            QSignerDetailDelegate {
                height: 60
                width: parent.width
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 12
                }
                typeInt: model.single_signer_type
                tag: model.single_signer_tag
                signerName: model.singleSigner_name
                accountIndex: model.single_signer_account_index
                card_id_or_xfp: model.single_signer_type === NUNCHUCKTYPE.NFC ? model.single_signer_device_cardid : model.singleSigner_masterFingerPrint
                key_color: "#F5F5F5"
            }
            QTextButton {
                visible: !model.single_signer_isReplaced
                width: 78
                height: 36
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                label.text: STR.STR_QML_1368
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    replaceClicked()
                }
            }
            Row {
                visible: model.single_signer_isReplaced
                width: 75
                height: 24
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                spacing: 4
                QImage {
                    width: 24
                    height: 24
                    source: "qrc:/Images/Images/check-circle-dark.svg"
                }
                QLato {
                    text: STR.STR_QML_104
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        Item {
            height: 16
            width: parent.width
            visible: model.single_signer_isReplaced
            anchors {
                left: parent.left
                leftMargin: 16
            }
            Row {
                spacing: 4
                QImage {
                    width: 16
                    height: 16
                    source: "qrc:/Images/Images/replace.svg"
                }
                QLato {
                    height: 16
                    text: {
                        if (model.single_signer_isReplaced === false) {
                            return ""
                        }
                        var type = model.single_signer_keyReplaced.signerType
                        var id = ""
                        if (type === NUNCHUCKTYPE.NFC) {
                            var card_id = model.single_signer_keyReplaced.cardId
                            var textR = card_id.substring(card_id.length - 5,card_id.length).toUpperCase()
                            id = "CARD ID: ••" + textR
                        } else {
                            id = "XFP: " + model.single_signer_keyReplaced.signerMasterFingerPrint.toUpperCase()
                        }
                        return STR.STR_QML_1380.arg(model.single_signer_keyReplaced.signerName).arg(id)
                    }
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
}

