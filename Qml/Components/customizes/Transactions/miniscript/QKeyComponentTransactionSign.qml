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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Signers"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

QKeyComponentTransaction {
    id: keyroot
    signal signRequest()
    signal scanRequest()
    signal exportRequest()
    signal importRequest()
    property bool alreadySigned : miniscript.alreadySigned
    property bool hasSignOrScan : miniscript.hasSignOrScan
    property int  tx_status     : transactionInfo.status
    property bool signerReadyToSign: miniscript.signerReadyToSign
    property int  signerType    : miniscript.signerType

    Loader {
        anchors {
            top: parent.top
            topMargin: 8
            right: parent.right
            rightMargin: 12
        }
        height: 36
        sourceComponent: {
            if(alreadySigned) return signedComp
            else{
                if(tx_status !== NUNCHUCKTYPE.PENDING_SIGNATURES && tx_status !== NUNCHUCKTYPE.PENDING_NONCE && tx_status !== NUNCHUCKTYPE.READY_TO_BROADCAST) {
                    return null;
                } else {
                    if(signerType === NUNCHUCKTYPE.AIRGAP || signerType === NUNCHUCKTYPE.UNKNOWN ) {
                        return hasSignOrScan ? keysignOption : null;
                    } else {
                        if(signerReadyToSign){
                            return hasSignOrScan ? requiredSignature : null;
                        } else {
                            return hasSignOrScan ? requiredScan : null;
                        }
                    }
                }
            }
        }
    }
    Component {
        id: signedComp
        Row {
            spacing: 4
            layoutDirection: Qt.RightToLeft
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/check_circle_24px_n.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                anchors.verticalCenter: parent.verticalCenter
                text: (miniscript.keysetStatus === NUNCHUCKTYPE.PENDING_NONCE) ? "Committed" : "Signed"
                font.pixelSize: 12
                color: "#1C1C1C"
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    Component {
        id: requiredSignature
        QTextButton {
            width: label.width + 36
            height: 36
            label.text: (miniscript.keysetStatus === NUNCHUCKTYPE.PENDING_NONCE) ? "Commit" : STR.STR_QML_509
            label.font.pixelSize: 12
            label.font.family: "Lato"
            type: eTypeE
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            onButtonClicked: { signRequest() }
        }
    }
    Component {
        id: requiredScan
        Item {
            QRefreshButton {
                width: 70
                height: 36
                iconSize: 18
                iconSpacing: 4
                label: STR.STR_QML_510
                fontPixelSize: 12
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onButtonClicked: { scanRequest() }
            }
        }
    }
    Component {
        id: keysignOption
        Item {
            QTextButton {
                width: label.width + 36
                height: 36
                label.text: STR.STR_QML_509
                label.font.pixelSize: 12
                label.font.family: "Lato"
                type: eTypeE
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onButtonClicked: {
                    signOptionMenu.x = 20
                    signOptionMenu.y = 20 - signOptionMenu.height
                    signOptionMenu.open()
                }
            }
            QContextMenu {
                id: signOptionMenu
                menuWidth: 250
                labels: [
                    STR.STR_QML_294,
                    STR.STR_QML_252,
                ]
                icons: [
                    "qrc:/Images/Images/ExportFile.svg",
                    "qrc:/Images/Images/importFile.svg",
                ]
                onItemClicked: {
                    switch(index){
                    case 0: // Export transaction
                        exportRequest()
                        break;
                    case 1: // Import signature
                        importRequest()
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}
