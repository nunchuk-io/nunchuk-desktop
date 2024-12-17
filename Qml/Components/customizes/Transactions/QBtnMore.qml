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
import QRCodeItem 1.0
import DataPool 1.0
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QButtonLargeTail {
    id: moreOptionsBtn
    width: 160
    height: 48
    type: eSECONDARY
    label: STR.STR_QML_1094
    optionVisible: othersContextMenu.visible
    layoutDirection: Qt.RightToLeft

    property bool  isAssisedWallet        : false
    property bool  isSharedWallet         : false
    property bool  enableShowInvoice      : false
    property bool  enableRequestSignature : false
    property bool  enableScheduleBroadcast: false
    property bool  enableCancelTransaction: false
    property bool  enableExportTransaction: false
    property bool  enableImportTransaction: false

    property string myRole: AppModel.walletInfo.myRole

    signal requestSignature()
    signal scheduleBroadcast()
    signal cancelTransaction()
    signal showInvoice()
    signal exportToPSBT()
    signal exportToQR()
    signal exportToBBQR()
    signal importViaPSBT()
    signal importViaQR()

    onButtonClicked: {
        othersContextMenu.x = 20
        othersContextMenu.y = 20 - othersContextMenu.height
        othersContextMenu.open()
    }

    QMultiContextMenu {
        id: othersContextMenu
        menuWidth: 300
        property var exportMenu: [
            {
                visible: true,
                label: STR.STR_QML_300,
                icon: "",
                iconRight: "",
                color: "#031F2B",
                enable: true,
                subMenu: null,
                action: function() {
                    exportToPSBT()
                    othersContextMenu.close()
                }
            },
            {
                visible: true,
                label: STR.STR_QML_1532,
                icon: "",
                iconRight: "",
                color: "#031F2B",
                enable: true,
                subMenu: null,
                action: function(){
                    exportToQR()
                    othersContextMenu.close()
                }
            },
            {
                visible: true,
                label: STR.STR_QML_1531,
                icon: "",
                iconRight: "",
                color: "#031F2B",
                enable: true,
                subMenu: null,
                action: function(){
                    exportToBBQR()
                    othersContextMenu.close()
                }
            },
        ]
        property var importMenu: [
            {
                visible: true,
                label: STR.STR_QML_1533,
                icon: "",
                iconRight: "",
                color: "#031F2B",
                enable: true,
                subMenu: null,
                action: function() {
                    importViaPSBT()
                    othersContextMenu.close()
                }
            },
            {
                visible: true,
                label: STR.STR_QML_302,
                icon: "",
                iconRight: "",
                color: "#031F2B",
                enable: true,
                subMenu: null,
                action: function(){
                    importViaQR()
                    othersContextMenu.close()
                }
            },
        ]

        mapMenu: [
            {
                visible: myRole !== "OBSERVER" && enableExportTransaction,
                label: "Export",
                icon: "qrc:/Images/Images/ExportFile.svg",
                iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                color: "#031F2B",
                enable: myRole !== "OBSERVER" && enableExportTransaction,
                subMenu: exportMenu,
                action: function(){

                }
            },
            {
                visible: myRole !== "OBSERVER" && enableImportTransaction,
                label: "Import",
                icon: "qrc:/Images/Images/importFile.svg",
                iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                color: "#031F2B",
                enable: myRole !== "OBSERVER" && enableImportTransaction,
                subMenu: importMenu,
                action: function(){

                }
            },
            {
                visible: enableRequestSignature,
                label: "Request signature",
                icon: "qrc:/Images/Images/OnlineMode/signature-dark.png",
                iconRight: "",
                color: "#031F2B",
                enable: enableRequestSignature,
                subMenu: null,
                action: function(){
                    requestSignature()
                }
            },
            {
                visible: enableScheduleBroadcast,
                label: "Schedule broadcast",
                icon:  "qrc:/Images/Images/OnlineMode/scheduling-dark.png",
                iconRight: "",
                color: "#031F2B",
                enable: enableScheduleBroadcast,
                subMenu: null,
                action: function(){
                    scheduleBroadcast()
                }
            },
            {
                visible: true,
                label: "Copy transaction ID",
                icon: "qrc:/Images/Images/Copy_031F2B.png",
                iconRight: "",
                color: "#031F2B",
                enable: true,
                subMenu: null,
                action: function(){
                    AppModel.transactionInfo.copyTransactionID()
                }
            },
            {
                visible: enableCancelTransaction,
                label: (isAssisedWallet || isSharedWallet) ? "Cancel transaction" : "Remove transaction",
                icon: "qrc:/Images/Images/OnlineMode/cancel_red_24dp.png",
                iconRight: "",
                color: "#CF4018",
                enable: enableCancelTransaction,
                subMenu: null,
                action: function(){
                    cancelTransaction()
                }
            },
            {
                visible: enableShowInvoice,
                label: "Show as an invoice",
                icon: "qrc:/Images/Images/OnlineMode/sticky_note_2.png",
                iconRight: "",
                color: "#031F2B",
                enable: enableShowInvoice,
                subMenu: null,
                action: function(){
                    showInvoice()
                }
            }
        ]
    }
}
