import QtQuick 2.4

TextInput {
    id: textEdit
    color: "Black"
    activeFocusOnTab : true
    activeFocusOnPress: true
    verticalAlignment: Text.AlignVCenter
    cursorVisible: !readOnly && textEdit.activeFocus
    selectByMouse: true
    renderType: Text.QtRendering
    signal typingFinished(var currentText)
    onTextChanged: if(initialized === true) inputIdentify.restart()

    property bool initialized: false
    Timer {
        id: inputIdentify
        interval: 250
        onTriggered: { if(textEdit.text !== "") typingFinished(textEdit.text) }
    }
    Component.onCompleted: initialized = true
}
