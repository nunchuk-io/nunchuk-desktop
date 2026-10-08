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
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1187
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var walletInfo: AppModel.walletInfo
    property string wallet_name: eFlow === eWALLET_CHANGE_ALIAS ?  walletInfo.walletAliasName  : ""
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 539
                height: paintedHeight
                text: STR.STR_QML_1186
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            QWalletAliasTextInput {
                id: _name
                label: STR.STR_QML_1187
                editLabel: eFlow === eWALLET_SET_ALIAS ?  "" : STR.STR_QML_1189
                boxWidth: 539
                boxHeight: 48
                textInputted: wallet_name
                onTextInputtedChanged: {
                    if(!_name.isValid){
                        _name.isValid = true
                        _name.errorText = ""
                    }
                    _name.showError = false;
                    wallet_name = _name.textInputted
                }
                onTextEditClicked: {
                    remove_alias.open()
                }

                maxLength: 20
                showWarning: true
            }
            QTextInputBoxTypeB {
                label: STR.STR_QML_1198
                boxWidth: 539
                boxHeight: 48
                textInputted: walletInfo.walletOriginName
                enabled: false
                visible: eFlow === eWALLET_CHANGE_ALIAS
            }
            Rectangle {
                width: 343
                height: 1
                color: "#EAEAEA"
                visible: eFlow === eWALLET_SET_ALIAS && list.count > 0
            }
            Item {
                width: 539
                height: 196
                visible: list.count > 0
                Column {
                    anchors.fill: parent
                    spacing: 16
                    QLato {
                        width: parent.width
                        height: paintedHeight
                        text: STR.STR_QML_1188
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.weight: Font.Bold
                    }
                    QListView {
                        id: list
                        width: parent.width
                        height: 150
                        spacing: 16
                        clip: true
                        ScrollBar.vertical: ScrollBar { active: true }
                        model: walletInfo.aliasMembers
                        delegate: QTextInputBoxTypeB {
                            label: modelData.user.name
                            boxWidth: 539
                            boxHeight: 48
                            textInputted: modelData.alias
                            enabled: false
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: eFlow = eWALLET_CONFIG
    bottomRight: QTextButton {
        width: 100
        height: 48
        label.text: STR.STR_QML_835
        label.font.pixelSize: 16
        type: eTypeE
        enabled: wallet_name !== ""
        onButtonClicked: {
            if (walletInfo.updateWalletAlias(wallet_name)) {
                eFlow = eWALLET_CONFIG
                AppModel.showToast(0, STR.STR_QML_1197, EWARNING.SUCCESS_MSG);
            }
        }
    }
    QConfirmYesNoPopup {
        id: remove_alias
        title: STR.STR_QML_334
        contentText: STR.STR_QML_1192
        onConfirmNo: close()
        onConfirmYes: {
            if (walletInfo.deleteWalletAlias()) {
                close()
                eFlow = eWALLET_CONFIG
                AppModel.showToast(0, STR.STR_QML_1191, EWARNING.SUCCESS_MSG);
            }
        }
    }
}
