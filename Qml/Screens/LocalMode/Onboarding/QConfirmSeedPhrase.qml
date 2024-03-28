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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1266
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property string mnemonicstr: ""
    content: Item {
        height: 496
        Column {
            spacing: 24
            QLato {
                width: 646
                text: STR.STR_QML_1269
                anchors.left: parent.left
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            Column {
                spacing: 48
                QSeedSelectionItem {
                    id: random1
                    width: 560
                    height: 60
                }
                QSeedSelectionItem {
                    id: random2
                    width: 560
                    height: 60
                }
                QSeedSelectionItem {
                    id: random3
                    width: 560
                    height: 60
                }
            }
        }
        function isValid() {
            return random1.correct && random2.correct && random3.correct
        }
        function randomSeedSelection() {
            var arr = [];
            var mnemomics = textMnemonic.split(' ');
            var numWords = mnemomics.length;
            while(arr.length < 9 && mnemomics.length > 1){
                var r = Math.floor(Math.random() * (numWords - 1)) + 1;
                if(arr.indexOf(r) === -1) arr.push(r);
            }
            var numsIndexSelection = GlobalData.swapPositions()
            console.log(numsIndexSelection)
            // #1
            random1.indexArray = [arr[0], arr[1], arr[2]];
            random1.indexNeeded = random1.indexArray[numsIndexSelection[0]];
            random1.phraseNeeded = mnemomics[random1.indexNeeded];
            random1.phraseArray = [mnemomics[random1.indexArray[0]], mnemomics[random1.indexArray[1]], mnemomics[random1.indexArray[2]]];
            // #2
            random2.indexArray = [arr[3], arr[4], arr[5]];
            random2.indexNeeded = random2.indexArray[numsIndexSelection[1]];
            random2.phraseNeeded = mnemomics[random2.indexNeeded];
            random2.phraseArray = [mnemomics[random2.indexArray[0]], mnemomics[random2.indexArray[1]], mnemomics[random2.indexArray[2]]];
            // #3
            random3.indexArray = [arr[6], arr[7], arr[8]];
            random3.indexNeeded = random3.indexArray[numsIndexSelection[2]];
            random3.phraseNeeded = mnemomics[random3.indexNeeded];
            random3.phraseArray = [mnemomics[random3.indexArray[0]], mnemomics[random3.indexArray[1]], mnemomics[random3.indexArray[2]]];
        }

    }
    Connections {
        target: AppModel
        onMnemonicChanged: {
            _content.contentItem.randomSeedSelection()
        }
    }
    Component.onCompleted: {
        _content.contentItem.randomSeedSelection()
    }

    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: (textMnemonic !== "")
            onButtonClicked: {
                var valid = _content.contentItem.isValid()
                if(valid){
                    _info2.open()
                }
                else{
                    AppModel.showToast(-1, STR.STR_QML_207, EWARNING.EXCEPTION_MSG);
                }
            }
        }
    }
    property string textMnemonic: AppModel.mnemonic
    QPopupInfoTwoButtons {
        id: _info2
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1273
        labels: [STR.STR_QML_1274,STR.STR_QML_1275]
        isVertical: true
        funcs: [
            function() {
                OnBoarding.state = "seedPhrase"
            },
            function() {
                OnBoarding.requestBackupHotWallet()
                closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
        ]
    }
}
