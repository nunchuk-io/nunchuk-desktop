/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2026 Nunchuk                                            *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *************************************************************************/
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import DataPool 1.0

Item {
    id: root
    width: 40
    height: 40

    property var targetInput: null
    property int currentCategory: 1
    property bool replacingEmoticon: false
    readonly property var emoticonMappings: [
        { text: ":'(", emoji: "😢" },
        { text: ":-D", emoji: "😄" },
        { text: ":-P", emoji: "😛" },
        { text: ":-p", emoji: "😛" },
        { text: ":-)", emoji: "🙂" },
        { text: ":-(", emoji: "🙁" },
        { text: ";-)", emoji: "😉" },
        { text: ":-O", emoji: "😮" },
        { text: ":-o", emoji: "😮" },
        { text: ":-/", emoji: "😕" },
        { text: ":-|", emoji: "😐" },
        { text: "^_^", emoji: "😊" },
        { text: "-_-", emoji: "😑" },
        { text: ":D", emoji: "😄" },
        { text: "=D", emoji: "😄" },
        { text: "XD", emoji: "😆" },
        { text: "xD", emoji: "😆" },
        { text: ":P", emoji: "😛" },
        { text: ":p", emoji: "😛" },
        { text: ":)", emoji: "🙂" },
        { text: "=)", emoji: "🙂" },
        { text: ":(", emoji: "🙁" },
        { text: ";)", emoji: "😉" },
        { text: ":O", emoji: "😮" },
        { text: ":o", emoji: "😮" },
        { text: ":/", emoji: "😕" },
        { text: ":|", emoji: "😐" },
        { text: "8)", emoji: "😎" },
        { text: "<3", emoji: "❤️" }
    ]
    readonly property var categories: [
        {
            name: qsTr("Recent"),
            icon: "🕘",
            emojis: GlobalData.chatRecentEmojis
        },
        {
            name: qsTr("Smileys"),
            icon: "😀",
            emojis: ["😀", "😃", "😄", "😁", "😆", "😅", "😂", "🤣",
                     "😊", "😇", "🙂", "🙃", "😉", "😌", "😍", "🥰",
                     "😘", "😋", "😜", "🤪", "🤓", "😎", "🤩", "🥳",
                     "😏", "😒", "😔", "😢", "😭", "😤", "😱", "🤗"]
        },
        {
            name: qsTr("Gestures"),
            icon: "👍",
            emojis: ["👍", "👍🏽", "👎", "👌", "✌️", "🤞", "🤟", "🤘",
                     "🤙", "👈", "👉", "👆", "👇", "☝️", "✋", "🤚",
                     "🖐️", "🖖", "👋", "👏", "🙌", "👐", "🤝", "🙏",
                     "💪", "🫶", "🫰", "✍️", "👩🏽‍💻", "👨‍💻", "🧑‍💻", "💅"]
        },
        {
            name: qsTr("Animals"),
            icon: "🐻",
            emojis: ["🐶", "🐱", "🐭", "🐹", "🐰", "🦊", "🐻", "🐼",
                     "🐨", "🐯", "🦁", "🐮", "🐷", "🐸", "🐵", "🐔",
                     "🐧", "🐦", "🦄", "🐝", "🦋", "🐌", "🐞", "🐠"]
        },
        {
            name: qsTr("Food"),
            icon: "🍜",
            emojis: ["🍎", "🍊", "🍋", "🍉", "🍇", "🍓", "🍒", "🥭",
                     "🥑", "🍅", "🥕", "🌽", "🍞", "🧀", "🍔", "🍕",
                     "🍜", "🍣", "🍪", "🍩", "🎂", "☕", "🍺", "🥂"]
        },
        {
            name: qsTr("Activities"),
            icon: "⚽",
            emojis: ["⚽", "🏀", "🏈", "⚾", "🎾", "🏐", "🏓", "🏸",
                     "🥊", "🎯", "🎮", "🎲", "🎸", "🎹", "🎨", "🎬",
                     "🚴", "🏊", "🏆", "🥇", "🎉", "🎊", "🎁", "🎈"]
        },
        {
            name: qsTr("Travel"),
            icon: "✈️",
            emojis: ["🚗", "🚕", "🚌", "🚎", "🏎️", "🚓", "🚑", "🚒",
                     "🚲", "✈️", "🚀", "🚁", "⛵", "🚢", "🏠", "🏢",
                     "🏖️", "🏔️", "🌋", "🗼", "🗽", "🌍", "🌙", "⭐"]
        },
        {
            name: qsTr("Symbols"),
            icon: "❤️",
            emojis: ["❤️", "🧡", "💛", "💚", "💙", "💜", "🖤", "🤍",
                     "🤎", "💔", "❣️", "💕", "💞", "💓", "💗", "💖",
                     "💯", "✅", "❌", "⚠️", "❓", "❗", "♻️", "🔒"]
        },
        {
            name: qsTr("Flags"),
            icon: "🇻🇳",
            emojis: ["🇻🇳", "🇺🇸", "🇬🇧", "🇦🇺", "🇨🇦", "🇯🇵", "🇰🇷", "🇸🇬",
                     "🇹🇭", "🇮🇩", "🇵🇭", "🇲🇾", "🇫🇷", "🇩🇪", "🇮🇹", "🇪🇸",
                     "🇳🇱", "🇨🇭", "🇸🇪", "🇳🇴", "🇩🇰", "🇫🇮", "🇮🇳", "🇧🇷"]
        }
    ]
    readonly property var currentEmojis: currentCategory === 0
                                          ? GlobalData.chatRecentEmojis
                                          : categories[currentCategory].emojis

    function closePicker() {
        picker.close()
    }

    function isBoundaryBefore(value, index) {
        return index === 0 || /\s/.test(value.charAt(index - 1))
    }

    function isBoundaryAfter(value, index) {
        if (index >= value.length) {
            return true
        }
        var nextCharacter = value.charAt(index)
        if (/[\s,!?]/.test(nextCharacter)) {
            return true
        }
        return nextCharacter === "."
                && (index + 1 >= value.length || !/[0-9]/.test(value.charAt(index + 1)))
    }

    function convertEmoticons(value) {
        if (!value) {
            return value
        }

        var result = ""
        var index = 0
        while (index < value.length) {
            var matched = false
            for (var i = 0; i < emoticonMappings.length; ++i) {
                var mapping = emoticonMappings[i]
                var end = index + mapping.text.length
                if (value.substr(index, mapping.text.length) === mapping.text
                        && isBoundaryBefore(value, index)
                        && isBoundaryAfter(value, end)) {
                    result += mapping.emoji
                    index = end
                    matched = true
                    break
                }
            }
            if (!matched) {
                result += value.charAt(index)
                ++index
            }
        }
        return result
    }

    function replaceCompletedEmoticon() {
        if (!targetInput || replacingEmoticon || targetInput.inputMethodComposing) {
            return
        }

        var cursor = targetInput.cursorPosition
        if (cursor < 1 || !/\s/.test(targetInput.text.charAt(cursor - 1))) {
            return
        }

        var tokenEnd = cursor - 1
        for (var i = 0; i < emoticonMappings.length; ++i) {
            var mapping = emoticonMappings[i]
            var tokenStart = tokenEnd - mapping.text.length
            if (tokenStart >= 0
                    && targetInput.text.substring(tokenStart, tokenEnd) === mapping.text
                    && isBoundaryBefore(targetInput.text, tokenStart)) {
                replacingEmoticon = true
                targetInput.remove(tokenStart, tokenEnd)
                targetInput.insert(tokenStart, mapping.emoji)
                targetInput.cursorPosition = tokenStart + mapping.emoji.length + 1
                replacingEmoticon = false
                GlobalData.rememberChatEmoji(mapping.emoji)
                return
            }
        }
    }

    function insertEmoji(emoji) {
        if (!targetInput || emoji === "") {
            return
        }

        targetInput.forceActiveFocus()
        Qt.inputMethod.commit()

        var start = Math.min(targetInput.selectionStart, targetInput.selectionEnd)
        var end = Math.max(targetInput.selectionStart, targetInput.selectionEnd)
        if (start < 0) {
            start = targetInput.cursorPosition
        }
        if (end < start) {
            end = start
        }
        start = Math.max(0, Math.min(start, targetInput.text.length))
        end = Math.max(start, Math.min(end, targetInput.text.length))

        if (end > start) {
            targetInput.remove(start, end)
        }
        targetInput.insert(start, emoji)
        targetInput.cursorPosition = start + emoji.length
        GlobalData.rememberChatEmoji(emoji)
        picker.close()
        targetInput.forceActiveFocus()
    }

    Button {
        id: trigger
        anchors.fill: parent
        hoverEnabled: true
        focusPolicy: Qt.TabFocus
        padding: 0
        Accessible.name: qsTr("Emoji")

        background: Rectangle {
            radius: 8
            color: trigger.down ? "#EAEAEA" : (trigger.hovered ? "#F5F5F5" : "#FFFFFF")
            border.color: "#DEDEDE"
        }
        contentItem: Text {
            text: "😀"
            font.pixelSize: 22
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ToolTip.visible: hovered
        ToolTip.delay: 500
        ToolTip.text: qsTr("Emoji")

        onClicked: {
            if (!root.targetInput) {
                return
            }
            root.targetInput.forceActiveFocus()
            Qt.inputMethod.commit()
            root.currentCategory = GlobalData.chatRecentEmojis.length > 0 ? 0 : 1
            picker.open()
        }
    }

    Popup {
        id: picker
        x: root.width - width
        y: -height - 8
        width: 336
        height: 330
        padding: 8
        modal: false
        dim: false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        onAboutToShow: {
            var overlay = Overlay.overlay
            if (!overlay || !parent) {
                return
            }

            var origin = overlay.mapToGlobal(0, 0)
            var left = parent.mapToGlobal(x, 0).x - origin.x
            var right = parent.mapToGlobal(x + width, 0).x - origin.x
            if (left < 8) {
                x += 8 - left
            } else if (right > overlay.width - 8) {
                x -= right - overlay.width + 8
            }

            var top = parent.mapToGlobal(0, y).y - origin.y
            if (top < 8) {
                y = parent.height + 8
            }
            var bottom = parent.mapToGlobal(0, y + height).y - origin.y
            if (bottom > overlay.height - 8) {
                y -= bottom - overlay.height + 8
            }
        }

        background: Rectangle {
            radius: 10
            color: "#FFFFFF"
            border.color: "#DEDEDE"
        }

        contentItem: Column {
            spacing: 8

            Text {
                width: parent.width
                height: 24
                text: root.categories[root.currentCategory].name
                color: "#031F2B"
                font.family: "Lato"
                font.pixelSize: 15
                font.weight: Font.DemiBold
                verticalAlignment: Text.AlignVCenter
            }

            ListView {
                id: categoryList
                width: parent.width
                height: 36
                orientation: ListView.Horizontal
                spacing: 4
                clip: true
                model: root.categories

                delegate: Button {
                    id: categoryButton
                    required property var modelData
                    required property int index
                    width: 32
                    height: 32
                    focusPolicy: Qt.NoFocus
                    padding: 0
                    Accessible.name: categoryButton.modelData.name

                    background: Rectangle {
                        radius: 6
                        color: root.currentCategory === categoryButton.index
                               ? "#F6D65D"
                               : (categoryButton.hovered ? "#F5F5F5" : "transparent")
                    }
                    contentItem: Text {
                        text: categoryButton.modelData.icon
                        font.pixelSize: 18
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    ToolTip.visible: hovered
                    ToolTip.delay: 500
                    ToolTip.text: categoryButton.modelData.name
                    onClicked: root.currentCategory = categoryButton.index
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#EAEAEA"
            }

            Item {
                width: parent.width
                height: 237

                GridView {
                    id: emojiGrid
                    anchors.fill: parent
                    clip: true
                    cellWidth: 40
                    cellHeight: 40
                    model: root.currentEmojis

                    delegate: Item {
                        id: emojiDelegate
                        required property string modelData
                        width: emojiGrid.cellWidth
                        height: emojiGrid.cellHeight

                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 2
                            radius: 6
                            color: emojiMouse.containsMouse ? "#F5F5F5" : "transparent"
                        }
                        Text {
                            anchors.centerIn: parent
                            text: emojiDelegate.modelData
                            font.pixelSize: 24
                            renderType: Text.NativeRendering
                        }
                        MouseArea {
                            id: emojiMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: root.insertEmoji(emojiDelegate.modelData)
                        }
                    }
                }

                Text {
                    anchors.centerIn: parent
                    visible: root.currentCategory === 0 && root.currentEmojis.length === 0
                    text: qsTr("No recent emoji")
                    color: "#757575"
                    font.family: "Lato"
                    font.pixelSize: 14
                }
            }
        }
    }
}
