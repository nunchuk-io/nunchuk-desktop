/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2026 Nunchuk                                            *
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
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

QPopup {
    id: root

    property string title: ""
    property string description: ""
    property url imageUrl: ""
    property var actions: []

    property int panelWidth: 420
    property int maximumBodyHeight: 560
    readonly property int bodyHeightLimit: Math.max(
        120,
        Math.min(maximumBodyHeight, root.height - root.offset * 2 - 48)
    )

    signal actionTriggered(var action)

    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: MouseArea {
        acceptedButtons: Qt.LeftButton
        onClicked: (mouse) => {
            const cardWidth = (root.itemInfo ? root.itemInfo.implicitWidth : root.panelWidth)
                              + root.offset * 2
            const cardHeight = (root.itemInfo ? root.itemInfo.implicitHeight : root.bodyHeightLimit)
                               + root.offset * 2
            const cardLeft = (root.width - cardWidth) / 2
            const cardTop = (root.height - cardHeight) / 2
            const outsideCard = mouse.x < cardLeft || mouse.x > cardLeft + cardWidth
                                || mouse.y < cardTop || mouse.y > cardTop + cardHeight
            if (outsideCard) {
                root.close()
            }
        }
    }

    function actionLabel(action) {
        if (action === undefined || action === null) {
            return ""
        }
        if (typeof action === "string") {
            return action
        }
        return action.label === undefined || action.label === null
                ? ""
                : String(action.label)
    }

    function actionSnapshot(action) {
        if (action === undefined || action === null || typeof action === "string") {
            return ({})
        }
        return {
            "label": action.label === undefined || action.label === null ? "" : String(action.label),
            "type": action.type === undefined || action.type === null ? "" : String(action.type),
            "target": action.target === undefined || action.target === null ? "" : String(action.target),
            "primary": action.primary === true
        }
    }

    content: Item {
        implicitWidth: root.panelWidth
        implicitHeight: Math.min(reminderContent.implicitHeight, root.bodyHeightLimit)
        width: implicitWidth
        height: implicitHeight

        Connections {
            target: root
            function onOpened() {
                reminderFlickable.contentY = 0
            }
        }

        Flickable {
            id: reminderFlickable
            anchors.fill: parent
            clip: true
            contentWidth: width
            contentHeight: reminderContent.implicitHeight
            interactive: contentHeight > height
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: QScrollBar {
                topPadding: 8
                bottomPadding: 8
            }

            Column {
                id: reminderContent
                width: reminderFlickable.width - 12
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 16

                QImage {
                    id: reminderImage
                    width: parent.width
                    height: visible ? 80 : 0
                    visible: root.imageUrl.toString().length > 0
                             && status !== Image.Error
                    source: root.imageUrl
                    asynchronous: true
                    cache: true
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: width * 2
                    sourceSize.height: height * 2
                }

                QLato {
                    width: parent.width
                    visible: text.length > 0
                    text: root.title
                    textFormat: Text.PlainText
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }

                QLato {
                    width: parent.width
                    visible: text.length > 0
                    text: root.description
                    textFormat: Text.PlainText
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 24
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }

                Column {
                    id: actionColumn
                    width: parent.width
                    spacing: 12
                    visible: actionRepeater.count > 0

                    Repeater {
                        id: actionRepeater
                        model: root.actions ? root.actions : []

                        delegate: QTextButton {
                            required property var modelData

                            width: actionColumn.width
                            height: 48
                            visible: label.text.length > 0
                            label.text: root.actionLabel(modelData)
                            label.font.pixelSize: 16
                            label.elide: Text.ElideRight
                            type: modelData.primary === true ? eTypeE : eTypeF

                            onButtonClicked: {
                                const selectedAction = root.actionSnapshot(modelData)
                                root.close()
                                root.actionTriggered(selectedAction)
                            }
                        }
                    }
                }
            }
        }
    }
}
