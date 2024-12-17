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
import HMIEVENTS 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _newTag
    height: 48
    property string coinColor:findUnuseColor()
    property var coinColors: ["#9EC063", "#2F466C", "#FAA077", "#1C652D",
                              "#B4DCFF", "#7E519B", "#FDD95C", "#595959",
                              "#D38FFF", "#CF4018", "#FFFFFF", "#A66800"]

    signal doneClicked()

    function findUnuseColor() {
        var unuseColor = ""
        for (var i = 0; i < coinColors.length; i++) {
            if (!coinTags.containColor(coinColors[i])) {
                unuseColor = coinColors[i]
                break
            }
        }
        return unuseColor === "" ? coinColors[0] : unuseColor
    }

    function refreshColor() {
        inputTag.textInputted = ""
        coinColor = findUnuseColor()
    }

    QPopup {
        id: colorEdit
        x: 36
        y: 90
        offset: 12
        offsetRadius: 8
        width: 264 - 12*2
        height: 228 - 12*2
        content: Rectangle {
            id: boxmask
            width: 264 - 12*2
            height: 228 - 12*2
            radius: 8
            color: "#FFFFFF"
            Column {
                anchors.fill: parent
                spacing: 16
                QLato {
                    text: STR.STR_QML_1448
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                Item {
                    width: 240
                    height: 168
                    Grid {
                        anchors.fill: parent
                        columns: 4
                        rowSpacing: 12
                        columnSpacing: 16
                        Repeater {
                            model: coinColors
                            QBadge {
                                width: 48
                                height: 48
                                radius: 48
                                color: modelData
                                border.width: coinColor === modelData ? 6 : 1
                                border.color: coinColor === modelData ? "#031F2B" : "#DEDEDE"
                                useMouse: true
                                onBadgeClicked: {
                                    coinColor = modelData
                                    colorEdit.close()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Row {
        id: row
        anchors.fill: parent
        spacing: 12
        QBadge {
            width: 48
            height: 48
            radius: 48
            iconSize: 24
            border.width: 1
            border.color: coinColor == "#FFFFFF" ? "#DEDEDE": coinColor
            color: coinColor
            icon: "qrc:/Images/Images/select-color-dark.svg"
            font.weight: Font.Medium
            anchors.verticalCenter: parent.verticalCenter
            useMouse: true
            onBadgeClicked: {
                colorEdit.open()
            }
        }

        QTextInputBoxTypeB {
            id: inputTag
            label: ""
            boxWidth: 610
            boxHeight: 48
            isValid: true
            anchors.verticalCenter: parent.verticalCenter
            enabled: true
            maxLength: 40
            validator: RegExpValidator {
                regExp: /^[^\s]*$/  // Allows letters, numbers, and symbols, no spaces
            }
            textInputted: "#"
            onTextInputtedChanged: {
                if (!textInputted.startsWith("#")) {
                    textInputted = "#" + textInputted.replace("#", "")
                    input.cursorPosition = textInputted.length
                }
            }
        }

        QTextLink {
            id: _done
            width: _done.paintedWidth
            height: 20
            text: STR.STR_QML_777
            anchors.verticalCenter: parent.verticalCenter
            onTextClicked: {
                var input = {
                    type: "add-coin-tag-to-wallet",
                    coinName: inputTag.textInputted,
                    coinColor: _newTag.coinColor
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            }
        }
    }
    Connections {
        target: walletInfo
        onUpdateTagNameChanged: {
            if (isError) {
                inputTag.isValid = false
                inputTag.showError = true
                inputTag.errorText = STR.STR_QML_1513
            } else {
                inputTag.isValid = true
                inputTag.showError = false
                inputTag.errorText = ""
                doneClicked()
            }
        }
    }
}
