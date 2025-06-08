/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo                                         *
 * Copyright (C) 2022 Nunchuk                                             *
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
import "../../../Components/customizes/QRCodes"
import "../../OnlineMode/Inheritances"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _newTran
    property string destinationAddress: ""
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_786
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo
    property var claimInheritanceCustomAmount: ServiceSetting.servicesTag.claimInheritanceCustomAmount
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)

    content: Item {
        Column {
            id: destColumn
            spacing: 36
            Row {
                id: destinationRow
                spacing: 28
                Item {
                    width: 479
                    height: addressInput.height
                    QTextInputBoxTypeB {
                        id: addressInput
                        label: STR.STR_QML_1129
                        boxWidth: 479
                        boxHeight: 48
                        isValid: true
                        maxLength: 280
                        disabledColor: "#FFFFFF"
                        input.rightPadding: 36
                        textInputted: destinationAddress
                        onTextInputtedChanged: {
                            if(AppSetting.validateAddress(addressInput.textInputted)) {
                                addressInput.isValid = true
                                addressInput.errorText = ""
                                addressInput.showError = false;
                                destinationAddress = addressInput.textInputted
                            } else {
                                if (addressInput.isValid) {
                                    addressInput.isValid = false
                                    AppModel.showToast(-1, STR.STR_QML_1184, EWARNING.ERROR_MSG);
                                    destinationAddress = ""
                                }
                            }
                        }
                    }
                    QIcon {
                        iconSize: 36
                        id: qricoimport
                        source: "qrc:/Images/Images/QrSendButton.svg"
                        anchors {
                            bottom: parent.bottom
                            bottomMargin: 6
                            right: parent.right
                            rightMargin: 6
                        }
                        transformOrigin: Item.Center
                        scale: qrMouse.pressed ? 0.95 : 1
                        MouseArea {
                            id: qrMouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                qrscaner.open()
                            }
                        }
                    }
                }

                Item {
                    width: 221
                    height: 72
                    QTextInputBoxTypeB {
                        id: amountInput
                        label: STR.STR_QML_214
                        boxWidth: 221
                        boxHeight: 48
                        isValid: true
                        enabled: false
                        disabledColor: "#EAEAEA"
                        textInputted: if (claimInheritanceCustomAmount === 0) {
                            inheritanceInfo.balance
                        } else {
                            convertFromSatoshi(false, claimInheritanceCustomAmount)
                        }
                    }
                    QLato {
                        color: "#595959"
                        width: 23
                        height: 28
                        anchors {
                            bottom: parent.bottom
                            bottomMargin: 10
                            right: parent.right
                            rightMargin: 20
                        }
                        text: RoomWalletData.unitValue
                    }
                }
            }
        }

        QQrImportScanner {
            id: qrscaner
            onTagFound: {
                destinationAddress = tag
                qrscaner.close()
            }
        }
        function convertFromSatoshi(isCurrency, amount) {
            if(isCurrency) {
                return AppModel.qCurrencyFromAmount(amount)
            }
            else if (AppSetting.unit === NUNCHUCKTYPE.BTC){
                return AppModel.qBTCFromAmount(amount)
            } else {
                return amount
            }
        }
    }

    bottomRight: QTextButton {
        width: 164
        height: 48
        label.text: STR.STR_QML_257
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: _newTran.nextClicked()
    }

    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    onNextClicked: {
        if (isValid()) {
            AppModel.showToast(-1, STR.STR_QML_816, EWARNING.ERROR_MSG)
        } else {
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_NEW_TRANSACTION_REQ, destinationAddress)
        }
    }

    RegExpValidator { id: intValidator; regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: doubleValidator; regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }    
}


