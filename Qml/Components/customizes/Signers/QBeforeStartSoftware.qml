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
import "../../../Components/customizes/Lists"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1724
    extraHeader: Item {}
    onCloseClicked: SignerManagement.backScreen()
    property string optionSelected: "create-and-back-up-key-now"
    property var optionList: {
        var ls = []
        ls.push({ id: "create-and-back-up-key-now"  ,  label: STR.STR_QML_1721 })
        if (!isPrimaryKeyFlow) {
            ls.push({ id: "create-key-now-back-up_later" , label: STR.STR_QML_1722 })
        }
        ls.push({ id: "recover-exist-seed" ,           label: STR.STR_QML_1351 })
        ls.push({ id: "recover-key-via-xprv",          label: STR.STR_QML_1723 })
        return ls
    }
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QPictureSmooth {
                    width: 346
                    height: 512
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/create-soft-key.svg"
                }
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 24
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_1720
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QRadioList {
                        width: 350
                        height: optionList.length * 60
                        model: optionList
                        spacing: 12
                        delegateType: _RadioTypeB
                        function radioSelect() {
                            return optionSelected
                        }
                        function setRadioSelect(id, data) {
                            optionSelected = id
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: SignerManagement.backScreen()
}

