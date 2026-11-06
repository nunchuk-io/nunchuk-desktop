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
import QtQuick
import QtQuick.Controls
import NUNCHUCKTYPE 1.0
import Features.Common.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2554
    onCloseClicked: vm.close()
    onPrevClicked: vm.close()
    content: Item {
        Row {
            spacing: 36
            QPicture {
                width: 346
                height: 512
                fillMode: Image.PreserveAspectFit
                source: "qrc:/Images/Images/clock_lock.svg"
            }
            Column {
                width: 346
                spacing: 16
                QLato {
                    width: parent.width
                    text: vm.description
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }
    bottomRight: QTextButton {
        width: label.paintedWidth + 16 * 2
        height: 48
        label.text: STR.STR_QML_777
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: vm.close()
    }
    BaseGuideViewModel {
        id: vm
    }
}
