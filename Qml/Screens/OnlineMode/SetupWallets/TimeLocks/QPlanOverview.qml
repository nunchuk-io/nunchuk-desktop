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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _plan
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    property var policy: ({})
    property int step: 0
    signal finished()

    readonly property var primary: policy && policy.primary ? policy.primary : ({})
    readonly property bool customized: primary.is_customized === true
    readonly property var fallbacks: policy && policy.fallbacks ? policy.fallbacks : []
    readonly property int fallbackCount: fallbacks.length

    function byCount(none, one, many) {
        return fallbackCount === 0 ? none : (fallbackCount === 1 ? one : many)
    }
    function keyBullets() {
        var ret = [customized ? STR.STR_QML_2467.arg(primary.m).arg(primary.n) : STR.STR_QML_2466]
        for (var i = 0; i < fallbacks.length; i++) {
            var fb = fallbacks[i]
            ret.push(fb.n === 1 ? STR.STR_QML_2468.arg(fb.index + 1) : STR.STR_QML_2469.arg(fb.index + 1).arg(fb.m).arg(fb.n))
        }
        return ret
    }

    label.text: step === 0 ? STR.STR_QML_2453 : step === 1 ? STR.STR_QML_2463 : STR.STR_QML_2475

    component BodyText: QLato {
        width: parent ? parent.width : 0
        lineHeightMode: Text.FixedHeight
        lineHeight: 20
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignLeft
        textFormat: Text.RichText
    }

    content: Item {
        Loader {
            anchors.fill: parent
            sourceComponent: _plan.step === 0 ? _overview : _illustrated
        }
        Component {
            id: _overview
            Column {
                spacing: 16
                BodyText { width: 680; text: STR.STR_QML_2454 }
                Repeater {
                    model: [STR.STR_QML_2455,
                            _plan.byCount(STR.STR_QML_2456, STR.STR_QML_2457, STR.STR_QML_2458),
                            _plan.byCount(STR.STR_QML_2459, STR.STR_QML_2460, STR.STR_QML_2461)]
                    Row {
                        spacing: 12
                        Rectangle {
                            width: 24
                            height: 24
                            radius: 12
                            color: "transparent"
                            border.width: 1.5
                            border.color: "#031F2B"
                            QLato {
                                anchors.centerIn: parent
                                text: index + 1
                                font.pixelSize: 12
                            }
                        }
                        QLato {
                            width: 680 - 36
                            text: modelData
                            wrapMode: Text.WordWrap
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                }
                BodyText { width: 680; text: STR.STR_QML_2462 }
            }
        }
        Component {
            id: _illustrated
            Row {
                spacing: 36
                Rectangle {
                    width: 346
                    height: 512
                    radius: 24
                    color: "#D0E2FF"
                    QPicture {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: _plan.step === 1 ? "qrc:/Images/Images/plan_inheritance_keys.svg" : "qrc:/Images/Images/plan_about_timelock.svg"
                    }
                }
                Column {
                    width: 346
                    spacing: 16
                    Column {
                        width: parent.width
                        spacing: 16
                        visible: _plan.step === 1
                        BodyText { text: _plan.fallbackCount > 0 ? STR.STR_QML_2465 : STR.STR_QML_2464 }
                        Repeater {
                            model: _plan.step === 1 ? _plan.keyBullets() : []
                            Row {
                                width: parent.width
                                spacing: 8
                                QLato {
                                    text: "•"
                                    anchors.top: parent.top
                                }
                                BodyText {
                                    width: parent.width - 16
                                    text: modelData
                                }
                            }
                        }
                        BodyText {
                            text: _plan.fallbackCount > 0 ? STR.STR_QML_2472 : (_plan.customized ? STR.STR_QML_2471 : STR.STR_QML_2470)
                        }
                        QWarningBgMulti {
                            width: parent.width
                            icon: "qrc:/Images/Images/info-60px.svg"
                            txt.text: (_plan.fallbackCount > 0 || _plan.customized) ? STR.STR_QML_2474 : STR.STR_QML_2473
                        }
                    }
                    Column {
                        width: parent.width
                        spacing: 16
                        visible: _plan.step === 2
                        BodyText { text: _plan.byCount(STR.STR_QML_2476, STR.STR_QML_2477, STR.STR_QML_2478) }
                        BodyText { text: STR.STR_QML_2479 }
                        BodyText { text: _plan.fallbackCount > 0 ? STR.STR_QML_2481 : STR.STR_QML_2480 }
                        QWarningBgMulti {
                            width: parent.width
                            icon: "qrc:/Images/Images/info-60px.svg"
                            txt.text: _plan.byCount(STR.STR_QML_2482, STR.STR_QML_2483, STR.STR_QML_2484)
                        }
                    }
                }
            }
        }
    }

    bottomLeft: QTextButton {
        width: 80
        height: 48
        label.text: STR.STR_QML_059
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: {
            if (_plan.step > 0) _plan.step--
            else _plan.prevClicked()
        }
    }
    bottomRight: QTextButton {
        width: label.paintedWidth + 2*20
        height: 48
        label.text: STR.STR_QML_265
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            if (_plan.step < 2) _plan.step++
            else _plan.finished()
        }
    }
}
