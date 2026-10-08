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
import Qt5Compat.GraphicalEffects
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Popups"
import "../../../../../localization/STR_QML.js" as STR

// NUN-10192 spec: "Removing encrypted selection auto-removes its pending backup/verification;
// confirm in app first." Shared by any Key Distribution Choice call site that lets the user
// switch AWAY from an already-uploaded encrypted backup (moved here from QVerifyBothBackups.qml,
// which used to show this unconditionally on link click instead of after re-choosing options).
QPopupOverlayScreen {
    id: _root
    property var pendingClaimOptions: []
    signal confirmed(var claimOptions)

    function openWith(claimOptions) {
        pendingClaimOptions = claimOptions
        open()
    }

    content: Component {
        QOnScreenContentTypeA {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_2283
            onCloseClicked: _root.close()
            onPrevClicked: _root.close()
            content: Item {
                Column {
                    width: 539
                    spacing: 16
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2284
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                    }
                    // BUGFIX: no explicit height - self-sizes now.
                    QWarningBgMulti {
                        width: 539
                        icon: "qrc:/Images/Images/info-60px.svg"
                        txt.text: STR.STR_QML_2285
                    }
                }
            }
            bottomRight: QTextButton {
                width: label.paintedWidth + 32
                height: 48
                type: eTypeD
                label.text: STR.STR_QML_2286
                label.font.pixelSize: 16
                onButtonClicked: {
                    _root.close()
                    _root.confirmed(_root.pendingClaimOptions)
                }
            }
        }
    }
}
