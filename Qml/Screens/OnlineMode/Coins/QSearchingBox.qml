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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"


Rectangle {
    id: _searchBar
    width: 500
    height: 48
    border.width: 1
    border.color: "#EAEAEA"
    enabled: true
    radius: 44
    signal filterClicked()

    Item {
        width: 468
        height: 24
        anchors {
            left: parent.left
            leftMargin: 16
            verticalCenter: parent.verticalCenter
        }
        Row {
            id:_row
            spacing: 12
            anchors.fill: parent
            QIconButton {
                height: 24
                width: 24
                iconSize: 24
                bgColor: "transparent"
                icon: "qrc:/Images/Images/search_24px.svg"
                onClicked: {
                    walletInfo.searchText = _search.text
                }
            }

            TextField {
                id: _search
                height: 48
                width: 396 - ((_close.visible ? 24 : 0) + (_close.visible ? 12 : 0))
                anchors.verticalCenter: parent.verticalCenter
                background: Item {}
                leftPadding: 0
                rightPadding: 0
                color: "#031F2B"
                font.family: "Lato"
                font.pixelSize: 16
                selectByMouse: true
                onTextChanged: {
                    walletInfo.searchText = text
                }
            }

            QIconButton {
                id: _close
                height: 24
                width: 24
                iconSize: 24
                visible: _search.text !== ""
                bgColor: "transparent"
                icon: "qrc:/Images/Images/close-dark.svg"
                onClicked: {
                    _search.text = ""
                }
            }
            QIconButton {
                id: _filter
                height: 24
                width: 24
                iconSize: 24
                bgColor: "transparent"
                icon: "qrc:/Images/Images/filter-dark.svg"
                onClicked: {
                    filterClicked()
                }
                Rectangle {
                    width: 16
                    height: 16
                    color: "#CF4018"
                    radius: 16
                    visible: walletInfo.utxoList.isFiltered
                    anchors {
                        top: parent.top
                        topMargin: -8
                        right: parent.right
                        rightMargin: -8
                    }
                }
            }
        }
    }
}
