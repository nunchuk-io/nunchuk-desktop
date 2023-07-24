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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    function fclose() {
        QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_SHARE_YOUR_SECRETS)
    }
    property string option: STR.STR_QML_881
    property string selectedOption: ""

    function findObj(obj, _property) {
        if (obj.hasOwnProperty(_property)) return obj
        else return findObj(obj.parent, _property)
    }

    QOnScreenContentTypeB {
        id: _Option
        visible: selectedOption === ""
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_847
        onCloseClicked: fclose()
        content: Item {
            Column {
                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: -16
                spacing: 24
                QLato {
                    width: 539
                    height: 56
                    text: STR.STR_QML_880
                    anchors.left: parent.left
                    font.weight: Font.Normal
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Flickable {
                    width: 549
                    height: 440
                    contentWidth: width
                    contentHeight: _colum.childrenRect.height
                    ScrollBar.vertical: ScrollBar { active: true }
                    clip: true
                    Column {
                        id: _colum
                        anchors.fill: parent
                        spacing: 16
                        QRadioButtonTypeF {
                            width: 539
                            height: 116
                            selected: option === labelTop
                            labelTop: STR.STR_QML_881
                            labelBottom: STR.STR_QML_882
                            onButtonClicked: { option = labelTop; }
                        }
                        QRadioButtonTypeF {
                            width: 539
                            height: 144
                            selected: option === labelTop
                            labelTop: STR.STR_QML_883
                            labelBottom: STR.STR_QML_884
                            onButtonClicked: { option = labelTop; }
                        }
                        QRadioButtonTypeF {
                            width: 539
                            height: 172
                            selected: option === labelTop
                            labelTop: STR.STR_QML_885
                            labelBottom: STR.STR_QML_886
                            onButtonClicked: { option = labelTop; }
                        }
                    }
                }
            }
        }
        bottomRight: QTextButton {
            width: 98
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _Option.nextClicked()
            }
        }
        onPrevClicked: fclose()
        onNextClicked: {
            selectedOption = option
        }
    }

    QOnScreenContentTypeB {
        visible: selectedOption === label.text
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_881
        onCloseClicked: fclose()
        content: Item {
            Row {
                spacing: 36
                Rectangle {
                    width: 346
                    height: 512
                    radius: 24
                    color: "#D0E2FF"
                    QImage {
                        width: 153
                        height: 214
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/inheritance_backup_password.svg"
                    }
                }
                QInheritanceDetails {
                    width: 346
                    height: 512
                    title: STR.STR_QML_887
                    warning: STR.STR_QML_890
                }
            }
        }
        bottomRight: QTextButton {
            width: 71
            height: 48
            label.text: STR.STR_QML_777
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                findObj(parent, "nextClicked").nextClicked()
                fclose()
            }
        }
        bottomLeft: Item {}
        onPrevClicked: fclose()
        onNextClicked: {
            AppModel.qInheritanceSecret = selectedOption
        }
    }

    QOnScreenContentTypeB {
        visible: selectedOption === label.text
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_883
        onCloseClicked: fclose()
        content: Item {
            Row {
                spacing: 36
                Rectangle {
                    width: 346
                    height: 512
                    radius: 24
                    color: "#D0E2FF"
                    QImage {
                        width: 153
                        height: 214
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/inheritance_backup_password.svg"
                    }
                }
                QInheritanceDetails {
                    width: 346
                    height: 512
                    title: STR.STR_QML_891
                    warning: STR.STR_QML_893
                }
            }
        }
        bottomRight: QTextButton {
            width: 71
            height: 48
            label.text: STR.STR_QML_777
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                findObj(parent, "nextClicked").nextClicked()
                fclose()
            }
        }
        bottomLeft: Item {}
        onPrevClicked: fclose()
        onNextClicked: {
            AppModel.qInheritanceSecret = selectedOption
        }
    }

    QOnScreenContentTypeB {
        visible: selectedOption === label.text
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_885
        onCloseClicked: fclose()
        content: Item {
            Row {
                spacing: 36
                Rectangle {
                    width: 346
                    height: 512
                    radius: 24
                    color: "#D0E2FF"
                    QImage {
                        width: 153
                        height: 214
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/inheritance_backup_password.svg"
                    }
                }
                QInheritanceDetails {
                    width: 346
                    height: 512
                    title: STR.STR_QML_892
                    warning: STR.STR_QML_894
                }
            }
        }
        bottomRight: QTextButton {
            width: 71
            height: 48
            label.text: STR.STR_QML_777
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                findObj(parent, "nextClicked").nextClicked()
                fclose()
            }
        }
        bottomLeft: Item {}
        onPrevClicked: fclose()
        onNextClicked: {
            AppModel.qInheritanceSecret = selectedOption
        }
    }

}
