import QtQuick
import QtQuick.Controls
import "../Texts"
import "../Buttons"

QPopup {
    id: root
    required property var pairing
    objectName: "devicePairingPopup"
    closePolicy: Popup.CloseOnEscape
    z: 1001

    readonly property bool enterCode: pairing.stage === "enter_code"
    readonly property bool compareCode: pairing.stage === "confirm_code"

    Connections {
        target: root.pairing
        function onStateChanged() {
            if (root.pairing.active && root.pairing.stage !== "canceling") root.open()
            else root.close()
        }
    }
    onOpened: if (itemInfo) itemInfo.resetInput()
    onClosed: if (pairing.active && pairing.stage !== "canceling") pairing.cancel()
    Component.onDestruction: if (pairing && pairing.active) pairing.cancel()

    content: Column {
        id: panel
        width: 400
        spacing: 24
        function resetInput() {
            codeInput.textInputted = ""
            if (root.enterCode) codeInput.input.forceActiveFocus()
        }
        function submit() {
            if (confirmButton.enabled) root.pairing.confirm(codeInput.textInputted)
        }

        QLato {
            width: parent.width
            text: qsTr("Pair %1").arg(root.pairing.deviceName)
            font.pixelSize: 24
            font.weight: Font.Bold
            textFormat: Text.PlainText
        }
        QLato {
            width: parent.width
            wrapMode: Text.WordWrap
            textFormat: Text.PlainText
            text: root.enterCode
                ? qsTr("Enter the six-digit code shown on your Trezor.")
                : root.compareCode
                    ? qsTr("Compare this code with your BitBox02. Confirm only if both codes match, and also approve on the device.")
                    : root.pairing.stage === "confirming"
                        ? qsTr("Complete the confirmation on your hardware device.")
                        : root.pairing.stage === "canceling"
                            ? qsTr("Canceling pairing…")
                            : qsTr("Unlock your device and approve the connection to continue.")
        }
        QTextInputBoxTypeB {
            id: codeInput
            objectName: "pairingCodeInput"
            width: parent.width
            boxWidth: width - 2
            label: qsTr("Pairing code")
            maxLength: 6
            visible: root.enterCode
            validator: RegularExpressionValidator { regularExpression: /^[0-9]{6}$/ }
            input.inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText | Qt.ImhSensitiveData
            onVisibleChanged: if (visible) input.forceActiveFocus()
            onEnterKeyRequest: panel.submit()
        }
        QLato {
            objectName: "pairingDisplayCode"
            width: parent.width
            text: root.pairing.code
            textFormat: Text.PlainText
            visible: root.compareCode
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WrapAnywhere
            font.family: "monospace"
            font.pixelSize: 20
        }
        Row {
            spacing: 12
            QTextButton {
                objectName: "pairingCancelButton"
                width: 194
                type: eTypeF
                label.text: qsTr("Cancel")
                onButtonClicked: root.pairing.cancel()
            }
            QTextButton {
                id: confirmButton
                objectName: "pairingConfirmButton"
                width: 194
                type: eTypeE
                label.text: root.compareCode ? qsTr("Codes match") : qsTr("Pair")
                enabled: root.compareCode || (root.enterCode && codeInput.acceptableInput)
                onButtonClicked: panel.submit()
            }
        }
    }
}
