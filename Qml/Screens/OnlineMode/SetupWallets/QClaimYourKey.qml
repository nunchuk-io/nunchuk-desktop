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
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    property var alert: GroupWallet.dashboardInfo.alert
    property string fingerPrint: ""
    property int currentSelect: -1
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1017
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            spacing: 24
            QLato {
                width: 539
                height: paintedHeight
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                text: STR.STR_QML_1018
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QListView {
                width: 550
                height: 464
                clip: true
                model: GroupWallet.dashboardInfo.health.healthStatuses
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QSignerDetailRadioDelegate {
                    width: ListView.view.width
                    height: modelData.keyinfo.type !== "SERVER" ? 92 : 0
                    visible: modelData.keyinfo.type !== "SERVER"
                    typeStr: modelData.keyinfo.type
                    tag: modelData.keyinfo.tag
                    signerName: modelData.keyinfo.name
                    card_id_or_xfp: modelData.keyinfo.tapsigner ? modelData.keyinfo.tapsigner.card_id : modelData.keyinfo.xfp
                    key_color: "#F5F5F5"
                    selected: currentSelect === index
                    key_isPrimaryKey: modelData.keyinfo.signer_is_primary
                    key_signerType: modelData.keyinfo.signer_type
                    key_accountIndex: modelData.keyinfo.account_index
                    onButtonClicked: {
                        if (modelData.keyinfo.type === "NFC") {
                            _info.open()
                            return
                        }
                        currentSelect = index
                        fingerPrint = modelData.keyinfo.xfp
                    }
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    nextEnable: fingerPrint !== ""
    onNextClicked: {
        if (fingerPrint !== "") {
            var _input = {
                xfp: fingerPrint,
            }
            QMLHandle.sendEvent(EVT.EVT_HEALTH_CHECK_STARTING_REQUEST, _input)
        }
    }
    QPopupInfoTwoButtons {
        id: _info
        property string link:"https://nunchuk.io/"
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1000
        labels: [STR.STR_QML_341,STR.STR_QML_1001]
        funcs: [
            function() {},
            function() {
                Qt.openUrlExternally(link)
            }
        ]
    }
}
