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
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Popups"
import "../../OnlineMode/AddHardwareKeys"
import "../../../../localization/STR_QML.js" as STR

Item {
    property var dashInfo: GroupWallet.dashboardInfo
    property var alert: dashInfo.alert
    property var inviter: dashInfo.inviter.user
    property bool isRead: false
    property int key_index: -1
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"
    function closeScreen() {
        closeTo(NUNCHUCKTYPE.WALLET_TAB)
        dashInfo.markRead()
    }

    QOnScreenContentTypeB {
        visible: alert.status === "UNREAD"
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: alert.title
        extraHeader: Item {}
        onCloseClicked: closeScreen()
        content: Item {
            Column {
                anchors.fill: parent
                spacing: 24
                QLato {
                    width: 656
                    height: 56
                    text: STR.STR_QML_937.arg(inviter.name)
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        onPrevClicked: closeScreen()
        onNextClicked: {
            isRead = true
            dashInfo.markRead()
        }
    }

    QOnScreenContentTypeB {
        visible: alert.status === "READ" || isRead
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_938
        onCloseClicked: closeScreen()
        content: Item {
            Row {
                anchors.fill: parent
                spacing: 36
                Item {
                    width: 346
                    height: parent.height
                    Column {
                        anchors.fill: parent
                        spacing: 36
                        QLato {
                            width: parent.width
                            text: STR.STR_QML_939.arg(dashInfo.mInfo).arg(dashInfo.nInfo).arg(dashInfo.mInfo === 2 ? STR.STR_QML_939_two : STR.STR_QML_939_three)
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        QLato {
                            width: parent.width
                            text: {
                                if (dashInfo.allowInheritance)
                                {
                                    var user = ClientController.user
                                    if (dashInfo.mInfo === 2 && dashInfo.nInfo === 4) {
                                        if (dashInfo.isPremierGroup) {
                                            return STR.STR_QML_940_without
                                        } else {
                                            return STR.STR_QML_940_one
                                        }
                                    }
                                    else {
                                        if (dashInfo.isPremierGroup) {
                                            return STR.STR_QML_940_without
                                        } else {
                                            return STR.STR_QML_940_two
                                        }
                                    }
                                }
                                else {
                                    return STR.STR_QML_940_without
                                }
                            }

                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                Item {
                    width: 346
                    height: parent.height
                    Item {
                        width: parent.width
                        height: 36
                        QLato {
                            width: 96
                            height: 36
                            text: STR.STR_QML_289
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QRefreshButtonA {
                            anchors {
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                            }
                            width: 70
                            height: 36
                            color: "transparent"
                            border.color: "transparent"
                            iconSize: 18
                            iconSpacing: 4
                            label: STR.STR_QML_652
                            fontPixelSize: 12
                            onButtonClicked: {
                                GroupWallet.refresh()
                                stopRefresh()
                            }
                        }
                    }
                    Rectangle {
                        anchors {
                            top: parent.top
                            topMargin: 36
                        }
                        color: "#FFFFFF"
                        border.color: "#FFEAEA"
                        radius: 12
                        width: 346
                        height: 456
                        Flickable {
                            anchors {
                                fill: parent
                                margins: 12
                            }
                            Column {
                                width: 322
                                spacing: 16

                                Repeater {
                                    id: signers
                                    model: GroupWallet.dashboardInfo.keys
                                    QAddRequestKey {
                                        onTapsignerClicked: {
                                            // _info.contentText = STR.STR_QML_961
                                            // _info.open()
                                            key_index = modelData.key_index
                                            _hardwareAddKey.isInheritance = true
                                            _hardwareAddKey.open()
                                        }
                                        onHardwareClicked: {
                                            key_index = modelData.key_index
                                            _hardwareAddKey.isInheritance = false
                                            _hardwareAddKey.open()
                                        }
                                        onSerkeyClicked: {
                                            _info.contentText = STR.STR_QML_962
                                            _info.open()
                                        }
                                        onBackupClicked: {
                                            _importColdcardBackup.xfp = modelData.xfp
                                            _importColdcardBackup.open()
                                            var _input = {
                                                type: "open-import-encrypted-backup",
                                                fingerPrint: modelData.xfp,
                                            }
                                            QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        onPrevClicked: closeScreen()
        bottomRight: Item{}
    }


    QPopupHardwareAddKey {
        id: _hardwareAddKey
    }

    QPopupInfo{
        id:_info
        contentText: STR.STR_QML_961
    }

    QPopupImportColdcardBackup {
        id: _importColdcardBackup
    }
}
