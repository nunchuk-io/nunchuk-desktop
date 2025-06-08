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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1244
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property string email_address: {
        if (ClientController.isNunchukLoggedIn) {
            return ClientController.user.email
        } else {
            return ""
        }
    }

    property string note: ""
    property string your_country: ""
    content: Item {
        Column {
            spacing: 24
            QLato {
                width: parent.width
                text: STR.STR_QML_1260
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Item {
                width: 539
                height: 72
                QTextInputBoxTypeB {
                    label: STR.STR_QML_1264
                    width: 539
                    height: 72
                    boxWidth: 539
                    boxHeight: 48
                    isValid: true
                    textInputted: _your_country.countryName
                    input.placeholderText: "Select your country"

                }
                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/chevron-right-dark.svg"
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        bottom: parent.bottom
                        bottomMargin: 12
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    propagateComposedEvents: true
                    onClicked: {
                        _your_country.open()
                    }
                }
            }
            QTextInputBoxTypeB {
                id: _address
                label: STR.STR_QML_1263
                boxWidth: 539
                boxHeight: 48
                textInputted: email_address
                onTextInputtedChanged: {
                    if(!_address.isValid){
                        _address.isValid = true
                        _address.errorText = ""
                    }
                    _address.showError = false;
                    email_address = _address.textInputted
                }
            }
            QTextAreaBoxTypeA  {
                id: _note
                boxWidth: 539
                boxHeight: 120
                label: STR.STR_QML_860
                optional: " (optional)"
                textInputted: note
                onTextInputtedChanged: {
                    note = textInputted
                }
                enableLengthLimit:true
                isValid: true
                maxLength: 280
                input.verticalAlignment: Text.AlignTop
                input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }

    QSelectACountry {
        id: _your_country
    }

    onPrevClicked: OnBoarding.screenFlow = "assistedWallet"
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 349
            height: 48
            label.text: STR.STR_QML_1258
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                Qt.openUrlExternally("https://nunchuk.io/individuals")
            }
        }
        QTextButton {
            width: 113
            height: 48
            label.text: STR.STR_QML_1259
            label.font.pixelSize: 16
            type: eTypeE
            enabled: email_address !== "" && _your_country.countryCode !== ""
            onButtonClicked: {
                var _input = {
                    type: "send-query",
                    country_code: _your_country.countryCode,
                    email: email_address,
                    note: note
                }
                QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
            }
        }
    }
}
