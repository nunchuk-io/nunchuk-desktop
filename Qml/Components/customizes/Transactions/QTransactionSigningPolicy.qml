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
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Wallets"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: signingPolicy
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    background: Item{}

    property int currentIndex: 0
    property int keysetIndex: 0

    signal closeClicked()
    signal continueClicked(var use_script_path, var keyset_index)
    signal feeSettingClicked()

    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_1760
        extraHeader:Item {}
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 764
                height: 700
                radius: 24
            }
        }
        bottomLeft:  QButtonTextLink {
            height: 48
            label: STR.STR_QML_059
            onButtonClicked: {
                signingPolicy.close()
                signingPolicy.closeClicked()
            }
        }
        bottomRight: QTextButton {
            width: 166
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var use_script_path = (signingPolicy.currentIndex !== 0);
                var use_keyset_index = signingPolicy.keysetIndex;
                signingPolicy.continueClicked(use_script_path, use_keyset_index)
            }
        }
        onCloseClicked: {
            signingPolicy.close()
            signingPolicy.closeClicked()
        }
        content: Item {
            Item {
                anchors.fill: parent
                Flickable {
                    anchors.fill: parent
                    contentWidth: parent.width
                    contentHeight: policyContent.height
                    interactive: contentHeight > height
                    clip: true
                    flickableDirection: Flickable.VerticalFlick
                    ScrollBar.vertical: ScrollBar { active: true }
                    Column {
                        width: 500
                        id: policyContent
                        spacing: 16
                        Column {
                            width: parent.width
                            spacing: 8
                            QLato {
                                width: parent.width
                                font.pixelSize: 16
                                text: STR.STR_QML_1761
                                color: "#031F2B"
                                wrapMode: Text.WordWrap
                                lineHeight: 20
                                lineHeightMode: Text.FixedHeight
                                verticalAlignment: Text.AlignVCenter
                            }
                            Rectangle {
                                width: parent.width
                                height: 44
                                color: "#F5F5F5"
                                radius: 8
                                QLato {
                                    width: parent.width
                                    font.pixelSize: 16
                                    text: AppSetting.enableAutoFeeSelection ? STR.STR_QML_1764 : STR.STR_QML_1762
                                    color: "#031F2B"
                                    linkColor: "#031F2B"
                                    wrapMode: Text.WordWrap
                                    lineHeight: 20
                                    lineHeightMode: Text.FixedHeight
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.centerIn: parent
                                    onLinkActivated: {
                                        QMLHandle.sendEvent(EVT.EVT_GOTO_APP_SETTINGS_TAB, 3);
                                        feeSettingClicked()
                                    }
                                }
                            }
                        }

                        Column {
                            width: parent.width
                            spacing: 16
                            Repeater {
                                id: keysetRepeat
                                model: transactionInfo.keysetsCount
                                Rectangle {
                                    id: keysetItemIndex
                                    implicitWidth: keysetDelegate.width + 4
                                    implicitHeight: keysetDelegate.height + 4
                                    radius: 8
                                    border.width: (index == signingPolicy.currentIndex) ? 2 : 1
                                    border.color: (index == signingPolicy.currentIndex) ?"#031F2B" : "#DEDEDE"
                                    property int keysetIndex: index

                                    Column {
                                        id: keysetDelegate
                                        width: 500
                                        height: childrenRect.height
                                        anchors.centerIn: parent
                                        spacing: 12
                                        Loader {
                                            sourceComponent: keysetItemIndex.keysetIndex === 0 ? valueKeyset : otherKeyset
                                        }
                                        Column {
                                            width: parent.width
                                            spacing: 16
                                            Repeater {
                                                model: transactionInfo.singleSignersAssigned
                                                delegate: QSigningPolicyDelegateTaproot {
                                                    width: 500
                                                    visible: model.single_signer_keyset_index === keysetItemIndex.keysetIndex
                                                    signerData {
                                                        single_name: singleSigner_name
                                                        single_type: single_signer_type
                                                        single_tag: single_signer_tag
                                                        single_devicetype: single_signer_devicetype
                                                        single_masterFingerPrint: singleSigner_masterFingerPrint
                                                        single_account_index: single_signer_account_index
                                                        single_checked: single_signer_checked
                                                        single_is_local: single_signer_is_local
                                                        single_value_key: single_signer_value_key
                                                        single_derivationPath: singleSigner_derivationPath
                                                        single_device_cardid: single_signer_device_cardid
                                                    }
                                                }
                                            }
                                        }
                                        Rectangle {
                                            width: parent.width
                                            height: 56
                                            color: "#E9F1FF"
                                            radius: 8
                                            Rectangle {
                                                width: parent.width
                                                height: parent.height/2
                                                color: "#E9F1FF"
                                            }
                                            Rectangle {
                                                width: parent.width
                                                height: 1
                                                color: "#DEDEDE"
                                                anchors.top: parent.top
                                            }
                                            Row {
                                                spacing: 16
                                                anchors.centerIn: parent
                                                QLato {
                                                    text: "Estimated fee"
                                                    font.pixelSize: 12
                                                    color: "#031F2B"
                                                    horizontalAlignment: Text.AlignLeft
                                                    verticalAlignment: Text.AlignVCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                }
                                                Column {
                                                    width: 374
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    Item {
                                                        width: 374
                                                        height: 16
                                                        QText {
                                                            anchors.fill: parent
                                                            text: (((0 == index) ? transactionInfo.fee : transactionInfo.feeOtherKeyset) + RoomWalletData.unitValue)
                                                            color: "#031F2B"
                                                            font.weight: Font.DemiBold
                                                            font.pixelSize: 12
                                                            horizontalAlignment: Text.AlignRight
                                                            verticalAlignment: Text.AlignVCenter
                                                        }
                                                    }
                                                    Item {
                                                        width: 374
                                                        height: 16
                                                        QText {
                                                            anchors.fill: parent
                                                            text: qsTr("%1%2 %3").arg(AppSetting.currencySymbol).arg(((0 == index) ? transactionInfo.feeCurrency : transactionInfo.feeOtherKeysetCurrency)).arg(AppSetting.currency)
                                                            color: "#031F2B"
                                                            font.pixelSize: 12
                                                            horizontalAlignment: Text.AlignRight
                                                            verticalAlignment: Text.AlignVCenter
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Component {
                                        id: valueKeyset
                                        Item {
                                            width: 500
                                            height: 44
                                            Row {
                                                spacing: 8
                                                anchors.bottom: parent.bottom
                                                anchors.left: parent.left
                                                anchors.leftMargin: 16
                                                QImage {
                                                    width: 16
                                                    height: 16
                                                    source: "qrc:/Images/Images/ValueKeyset.png"
                                                }
                                                Column {
                                                    spacing: 4
                                                    QLato {
                                                        width: 76
                                                        height: 16
                                                        text: "Value Keyset"
                                                        font.pixelSize: 12
                                                        color: "#031F2B"
                                                        font.weight: Font.DemiBold
                                                        horizontalAlignment: Text.AlignLeft
                                                        verticalAlignment: Text.AlignVCenter
                                                    }
                                                    QLato {
                                                        height: 16
                                                        text: "Better privacy and lower fees"
                                                        font.pixelSize: 12
                                                        color: "#757575"
                                                        horizontalAlignment: Text.AlignLeft
                                                        verticalAlignment: Text.AlignVCenter
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Component {
                                        id: otherKeyset
                                        Item {
                                            width: 500
                                            height: 24
                                            QLato {
                                                anchors.bottom: parent.bottom
                                                anchors.left: parent.left
                                                anchors.leftMargin: 16
                                                height: 16
                                                text: qsTr("Keyset %1").arg(index+1)
                                                font.pixelSize: 12
                                                color: "#031F2B"
                                                font.weight: Font.DemiBold
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                        }
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            signingPolicy.currentIndex = index;
                                            signingPolicy.keysetIndex = keysetItemIndex.keysetIndex;
                                            console.log("signingPolicy.keysetIndex", signingPolicy.keysetIndex, keysetItemIndex.keysetIndex)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Rectangle {
                width: popupWidth
                height: 1
                color: "#DEDEDE"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
            }
        }
    }
}
