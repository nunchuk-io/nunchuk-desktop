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
import NUNCHUCKTYPE 1.0
import Features.Signers.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2088
    onCloseClicked: vm.close()
    content: Item {
        anchors.fill: parent
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 728
                text: STR.STR_QML_2090
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                verticalAlignment: Text.AlignVCenter
            }
            QTextInputBoxTypeB {
                id: xprvInput
                label: STR.STR_QML_2091
                boxWidth: 728
                boxHeight: 150
                isValid: true
                showError: true
                maxLength: 280
                input.verticalAlignment: TextInput.AlignTop
                onTypingFinished:  {
                    vm.xprv = xprvInput.textInputted
                    xprvInput.isValid = true
                    xprvInput.errorText = ""
                }
            }
        }
    }

    onPrevClicked: vm.back()
    onNextClicked: {
        if(vm.validateXPRV(vm.xprv)){
            vm.checkSignerExist()
        }        
    }
    QPopupInfoTwoButtons {
        id: _info
        title: STR.STR_QML_661
        labels: [STR.STR_QML_433,STR.STR_QML_432]
        funcs: [
            function() { vm.requestCreateSigner() },
            function() {}
        ]
    }
    function showPopupInfo(isSoftware, fingerPrint){
        if (isSoftware) {
            _info.contentText = STR.STR_QML_1283.arg(fingerPrint.toUpperCase())
            _info.contentTextTwo = STR.STR_QML_1284
            _info.open()
        }
        else {
            _info.contentText = STR.STR_QML_1283.arg(fingerPrint.toUpperCase())
            _info.contentTextTwo = ""
            _info.open()
        }
    }
    RecoverViaXPRVViewModel {
        id: vm
    }
    Connections {
        target: vm
        onNotifySignerExist: {
            showPopupInfo(isSoftware, fingerPrint)
        }
    }
    
}
