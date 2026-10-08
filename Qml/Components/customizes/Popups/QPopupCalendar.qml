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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: _calendar
    property string dateString: ""
    property date   minimumDate
    property alias  selectedDate: inputCalendar.selectedDate

    width: inputCalendar.width
    height: inputCalendar.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    anchors.centerIn: parent
    Calendar {
        id: inputCalendar
        anchors.centerIn: parent
        minimumDate: _calendar.minimumDate
        onClicked: {
            dateString = Qt.formatDateTime(date, "MM/dd/yyyy")
            _calendar.close()
        }
        style: CalendarStyle {
            gridVisible: false
            navigationBar: Rectangle {
              height: 48
              RowLayout {
                anchors.fill: parent
                anchors.margins: 1
                Button {
                  iconSource: "qrc:/Images/Images/chevron-left-dark.svg"
                  style: ButtonStyle {
                      background: Item{}
                  }
                  Layout.fillHeight: true
                  Layout.preferredWidth: 48
                  onClicked: inputCalendar.showPreviousMonth()
                }
                QText {
                  Layout.fillWidth: true
                  height: 40
                  text: styleData.title
                  font.pixelSize: 20
                  font.weight: Font.Normal
                  horizontalAlignment: Text.AlignHCenter
                  verticalAlignment: Text.AlignVCenter
                }
                Button {
                  iconSource: "qrc:/Images/Images/chevron-right-dark.svg"
                  style: ButtonStyle {
                      background: Item{}
                  }
                  Layout.fillHeight: true
                  Layout.preferredWidth: 48
                  onClicked: inputCalendar.showNextMonth()
                }
              }
            }
            dayOfWeekDelegate: QText {
              height: 40
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter

              text: dayOfWeekString(styleData.dayOfWeek)
              function dayOfWeekString(id){
                var str;
                switch(id){
                case Locale.Sunday:   str="Sun"; break;
                case Locale.Monday:   str="Mon"; break;
                case Locale.Tuesday:  str="Tue"; break;
                case Locale.Wednesday:str="Wed"; break;
                case Locale.Thursday: str="Thu"; break;
                case Locale.Friday:   str="Fri"; break;
                case Locale.Saturday: str="Sat"; break;
                default:              str="";   break;
                }
                return str
              }
            }
            dayDelegate: Rectangle {
                gradient: Gradient {
                    GradientStop {
                        position: 0.00
                        color: styleData.selected ? "#111" : (styleData.visibleMonth && styleData.valid ? "#444" : "#666");
                    }
                    GradientStop {
                        position: 1.00
                        color: styleData.selected ? "#444" : (styleData.visibleMonth && styleData.valid ? "#111" : "#666");
                    }
                    GradientStop {
                        position: 1.00
                        color: styleData.selected ? "#777" : (styleData.visibleMonth && styleData.valid ? "#111" : "#666");
                    }
                }

                Label {
                    text: styleData.date.getDate()
                    anchors.centerIn: parent
                    color: styleData.valid ? "white" : "grey"
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#555"
                    anchors.bottom: parent.bottom
                }

                Rectangle {
                    width: 1
                    height: parent.height
                    color: "#555"
                    anchors.right: parent.right
                }
            }
        }
    }
}
