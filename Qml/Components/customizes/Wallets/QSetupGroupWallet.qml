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
import QtQuick 2.12
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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: sandbox.url !== "" ? STR.STR_QML_1556 : STR.STR_QML_1664
    extraHeader: Item {}
    property var newWalletInfo: AppModel.newWalletInfo
    property var sandbox:       AppModel.newWalletInfo.sandbox
    onCloseClicked: {
        if (sandbox.url !== "") {
            closeTo(NUNCHUCKTYPE.WALLET_TAB)
        } else {
            _confirm.open()
        }
    }

    content: Item {
        anchors {
            top: parent.top
            topMargin: 8
        }
        Row {
            height: parent.height
            spacing: 24
            QGroupWalletInfo {
                onCopySandboxUrl: {
                    ClientController.copyMessage(sandbox.url)
                    AppModel.showToast(0, STR.STR_QML_1655, EWARNING.SUCCESS_MSG);
                }
                onShowQrSandboxUrl: {
                    qrcodeExportSandboxUrl.open()
                }
                onClickSetting: {
                    var _input = {
                        type: "setting-group-sandbox"
                    }
                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                }
            }
            Flickable {
                id: keylistflick
                clip: true
                width: 352
                height: 492
                contentHeight: keylist.implicitHeight
                interactive: contentHeight > height
                flickableDirection: Flickable.VerticalFlick
                contentY : contentHeight > height ? contentHeight - height : 0
                ScrollBar.vertical: ScrollBar { active: keylistflick.interactive }
                QGroupWalletKeys {
                    id: keylist
                }
            }
        }
    }

    bottomLeft: Item {}
    onPrevClicked: {
        QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK)
    }

    bottomRight: Row {
        spacing: 12
        Item {
            width: _txt.paintedWidth + 2*16
            height: 48
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    _confirm.open()
                }
            }
            QLato {
                id: _txt
                width: _txt.paintedWidth + 2*16
                height: 20
                anchors.centerIn: parent
                text: sandbox.url !== "" ? STR.STR_QML_1644 : STR.STR_QML_245
                color: "#CF4018"
                font.weight: Font.Bold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        QTextButton {
            width: 129
            height: 48
            label.text: sandbox.url !== "" ? STR.STR_QML_1249 : STR.STR_QML_1670
            label.font.pixelSize: 16
            type: eTypeE
            enabled: sandbox.enoughSigner
            onButtonClicked: {
                if (sandbox.checkWaitingOthers()) {
                    _waiting_for_other.open()
                } else {
                    var _input = {
                        type: "review-group-sandbox"
                    }
                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                }
            }
        }
    }

    QPopupEditBIP32Path {
        id: editBip32Path
        property int idx: -1
        property string xfp: ""
        onEnterText: {
            if (sandbox.editBIP32Path(idx, xfp, str)) {
                editBip32Path.close()
            } else {
                editBip32Path.showError()
            }
        }
    }

    QConfirmYesNoPopup {
        id: _confirm
        title: STR.STR_QML_334
        contentText: sandbox.url !== "" ? STR.STR_QML_1645 : STR.STR_QML_1666
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if (sandbox.url !== "") {
                sandbox.deleteGroup()
            } else {
                closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
        }
    }
    QConfirmYesNoPopup{
        id:_confirmRemoveKey
        property var idx
        title: STR.STR_QML_661
        contentText: STR.STR_QML_243
        onConfirmNo: close()
        onConfirmYes: {
            close()
            sandbox.removeKey(idx)
        }
    }

    QQrExportGroupWallet {
        id: qrcodeExportSandboxUrl
        label: STR.STR_QML_1636
        address: sandbox.url
    }
}
