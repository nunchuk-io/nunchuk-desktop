import QtQuick 2.4
import "../origins"

Rectangle {
    id: textipbox
    width: 344
    property int heightMin: 56
    height: heightMin + (textInputted.contentHeight - 20 )
    radius: 4

    readonly property bool textActiveFocus: textInputted.activeFocus
    property alias texOutputValignment: textInputted.verticalAlignment
    property alias textOutput: textInputted.text
    property alias validator: textInputted.validator
    property alias placeholder: placeholder_text
    property alias errorText: errorText
    property alias echoMode: textInputted.echoMode
    property alias length: textInputted.length
    property alias textOutputCapitalization: textInputted.font.capitalization
    property alias botomLineVisible: indicator_bottom.visible
    property string fontFamily: "Lato"
    property int fontPixelSize: 16
    property int rightPading: 16
    property int leftPading: 16
    property alias  maximumLength : textInputted.maximumLength
    readonly property int eREADONLY_MODE: 0
    readonly property int ePREVIEW_MODE: 1
    readonly property int eEDIT_MODE: 2
    property int mode: eEDIT_MODE
    property bool validInput: true
    signal typingFinished(var currentText)

    property bool isEditing: ("" != textInputted.text) || (true == textInputted.activeFocus)

    Rectangle {
        id: indicator_bottom
        width: parent.width-2
        height: 2
        color: !validInput ? "#E02247" : (true == textInputted.activeFocus) ? "#F6D65D" : "#C9DEF1"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: isEditing
    }

    QText {
        id: placeholder_text
        font.family: fontFamily
        anchors {
            left: parent.left
            leftMargin: leftPading
            top: parent.top
            topMargin: isEditing ? 8 : 16
        }
        font.weight: isEditing ? Font.Bold : Font.Normal
        color: !validInput ? "#E02247" : isEditing ? "#031F2B" : "#323E4A"
        font.pixelSize: isEditing ? 10 : 16
        font.capitalization: isEditing ?  Font.AllUppercase : Font.MixedCase
    }

    QTextEdit {
        id: textInputted
        anchors {
            fill: parent
            leftMargin: leftPading
            rightMargin: rightPading
            topMargin: 24
            bottomMargin: 6
        }
        font.family: fontFamily
        readOnly: (mode == eREADONLY_MODE)
        activeFocusOnPress: (mode == eEDIT_MODE)
        color: "#031F2B"
        font.pixelSize: fontPixelSize
        wrapMode: Text.WrapAnywhere
        onTypingFinished: textipbox.typingFinished(currentText)
        clip: true
    }

    Image {
        id: icoEdit
        source: "qrc:/Images/Images/edit.png"
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.verticalCenter: parent.verticalCenter
        visible: (mode == ePREVIEW_MODE) && (false == textInputted.activeFocus)
        enabled: (mode == ePREVIEW_MODE)
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                textipbox.focus = true
                textInputted.forceActiveFocus()
            }
        }
    }

    QText {
        id: errorText
        anchors.top: parent.bottom
        anchors.left: parent.left
        font.family: "Lato"
        font.pixelSize: 12
        color: "#E02247"
        visible: !validInput
    }
}
