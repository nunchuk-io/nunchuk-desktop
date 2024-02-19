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
import Qt.labs.platform 1.1
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
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/services"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1068
    extraHeader: Item {}
    property string option: ""
    property var recurringPayment: AppModel.walletInfo.recurringPayment
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var formats: [
        {id: "bsms",            description: STR.STR_QML_1087},
        {id: "coldcard-nfc",    description: STR.STR_QML_1088},
        {id: "qr_code",         description: STR.STR_QML_1089},
        {id: "existing-wallet", description: STR.STR_QML_1158},
    ]
    content: Item {
        Column {
            anchors {
                top: parent.top
                topMargin: -8
            }
            spacing: 24
            QLato {
                width: 539
                height: 28
                text: STR.STR_QML_1090
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Column {
                id: _colum
                spacing: 16
                Repeater {
                    model: formats
                    Rectangle {
                        width: 573
                        height: 60
                        border.width: 2
                        border.color: _radio.selected ? "#000000" : "#DEDEDE"
                        radius: 12
                        QRadioButtonTypeA {
                            anchors {
                                left: parent.left
                                leftMargin: 18
                            }
                            id: _radio
                            width: 573
                            height: 60
                            layoutDirection: Qt.RightToLeft
                            label: modelData.description
                            selected: option === modelData.id
                            onButtonClicked: { option = modelData.id; }
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: requestBack()
    nextEnable: option !== ""
    onNextClicked: {
        if (option === "qr_code") {
            qrscaner.open()
        }
        else if (option === "existing-wallet") {
            _select_wallet.open()
        }
        else {
            fileDialog.open()
        }
    }
    QQrImportScanner {
        id: qrscaner
        onTagFound: {
            if(recurringPayment.parseQRSuccess(qrscaner.tags)){
                if(recurringPayment.importQRWallet(qrscaner.tags)){
                    qrscaner.close()
                    var _input = {
                        type: "select-import-format",
                        format: option
                    }
                    QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
                }
            }
        }
        percent: recurringPayment.scan_percent
    }
    FileDialog {
        id: fileDialog
        property int iType: Popup_t.IMPORT_WALLET_DB
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var _input = {
                type: "select-import-format",
                format: option,
                filePath: fileDialog.file
            }
            QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
        }
    }
    QPopupEmpty {
        id: _select_wallet
        content: QOnScreenContentTypeB {
            width: 600
            height: 516
            anchors.centerIn: parent
            label.text: STR.STR_QML_837
            extraHeader: Item {}
            onCloseClicked: { _select_wallet.close() }
            content: Flickable {
                width: parent.width
                height: parent.height
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    spacing: 16
                    Repeater {
                        model: AppModel.walletList
                        QAssistedWalletDelegate {
                            id: dele
                            width: 528
                            height: dele.visible ? 92 : 0
                            walletName: model.wallet_name
                            walletBalance: model.wallet_Balance
                            walletCurrency: model.wallet_Balance_Currency
                            walletM: model.wallet_M
                            walletN: model.wallet_N
                            isLocked: model.wallet_dashboard ? model.wallet_dashboard.isLocked : false
                            onButtonClicked: {
                                var _input = {
                                    type: "select-import-format",
                                    format: option,
                                    wallet_id: model.wallet_id
                                }
                                _select_wallet.close()
                                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
                            }
                        }
                    }
                }
            }

            isShowLine: true
            onPrevClicked: { _select_wallet.close() }
            bottomRight: Item {}
        }
    }
}
