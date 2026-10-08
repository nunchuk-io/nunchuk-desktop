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
    property alias signerData: dataSingle
    QSingleSignerData {
        id: dataSingle
    }
    height: 84 + (dataSingle.single_isReplaced ? (16 + 8) : 0)
    signal replaceClicked()
    signal removeClicked(var idx)
    Column {
        id: _col
        width: parent.width
        spacing: 8
        Rectangle {
            height: 84
            width: parent.width
            color: dataSingle.single_isReplaced ? "#A7F0BA" : "#FDEBD2"
            radius: 12
            QSignerDetailDelegate {
                height: 60
                width: parent.width
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 12
                }
                typeInt: dataSingle.single_type
                tag: dataSingle.single_tag
                signerName: dataSingle.single_name
                accountIndex: dataSingle.single_account_index
                card_id_or_xfp: dataSingle.single_type === NUNCHUCKTYPE.NFC ? dataSingle.single_device_cardid : dataSingle.single_masterFingerPrint
                key_color: "#F5F5F5"
            }
            QTextButton {
                width: 79
                height: 36
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                label.text: dataSingle.single_isReplaced ? STR.STR_QML_1134 : STR.STR_QML_1368
                label.font.pixelSize: 12
                type: eTypeB
                onButtonClicked: {
                    if (dataSingle.single_isReplaced) {
                        removeClicked(index)
                    } else {
                        replaceClicked()
                    }
                }
            }
        }
        Item {
            height: 16
            width: parent.width
            visible: dataSingle.single_isReplaced
            anchors {
                left: parent.left
                leftMargin: 16
            }
            Row {
                spacing: 4
                QIcon {
                    iconSize: 16
                    source: "qrc:/Images/Images/replace.svg"
                }
                QLato {
                    height: 16
                    text: {
                        if (dataSingle.single_isReplaced === false) {
                            return ""
                        }
                        var keyReplaced = dataSingle.single_keyReplaced
                        var type = keyReplaced.single_signer_type
                        var id = ""
                        if (type === NUNCHUCKTYPE.NFC) {
                            var card_id = keyReplaced.single_signer_device_cardid
                            var textR = card_id.substring(card_id.length - 5,card_id.length).toUpperCase()
                            id = "CARD ID: ••" + textR
                        } else {
                            id = "XFP: " + keyReplaced.singleSigner_masterFingerPrint.toUpperCase()
                        }
                        return STR.STR_QML_1380.arg(keyReplaced.singleSigner_name).arg(id)
                    }
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
}

