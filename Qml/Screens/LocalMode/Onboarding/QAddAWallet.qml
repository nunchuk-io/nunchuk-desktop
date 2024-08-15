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
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1249
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        Column {
            anchors.left: parent.left
            width: 539
            spacing: 24
            QLato {
                anchors.left: parent.left
                width: parent.width
                text: STR.STR_QML_1250
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QCreateHotWallet {
                width: 539
                height: 92
                anchors.left: parent.left
                color: "#FFFFFF"
                title: STR.STR_QML_1262
                description: STR.STR_QML_1405
                desImg: "qrc:/Images/Images/wallet.svg"
                onItemclicked: {
                    var _input = {
                        type: "create-new-wallet"
                    }
                    QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                }
            }
            QCreateHotWallet {
                width: 539
                height: 120
                anchors.left: parent.left
                color: "#FFFFFF"
                title: STR.STR_QML_1251
                description: STR.STR_QML_1252
                desImg: "qrc:/Images/Images/person-add-24px.svg"
                onItemclicked: {
                    OnBoarding.state = "hotWallet"
                }
            }
        }
    }

    bottomLeft: Item {}
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 198
            height: 48
            label.text: STR.STR_QML_1253
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                optionMenu.x = 20
                optionMenu.y = 20 - optionMenu.height
                optionMenu.open()

            }
            QContextMenu {
                id: optionMenu
                menuWidth: 350
                labels: [
                    STR.STR_QML_038,
                    STR.STR_QML_037,
                    STR.STR_QML_040,
                    STR.STR_QML_1254
                ]
                icons: [
                    "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
                    "qrc:/Images/Images/import.png",
                    "qrc:/Images/Images/import.png",
                    "qrc:/Images/Images/import.png",
                ]
                onItemClicked: {
                    switch(index){
                    case 0:
                        qrscaner.open()
                        break;
                    case 1:
                        fileDialog.recoverType = "recover-via-bsms-config-file"
                        fileDialog.open()
                        break;
                    case 2:
                        fileDialog.recoverType = "recover-via-coldcard"
                        fileDialog.open()
                        break;
                    case 3:
                        OnBoarding.state = "recoverHotWallet"
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    QQrImportScanner {
        id: qrscaner
        onTagFound: {
            if(OnBoarding.importQrHotWallet(qrscaner.tags)){
                qrscaner.close()
                closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
        }
    }
    FileDialog {
        id: fileDialog
        property string recoverType: ""
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var _input = {
                type: "recover-existing-wallet",
                filePath: fileDialog.file,
                recoverType: fileDialog.recoverType
            }
            QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
        }
    }
}
