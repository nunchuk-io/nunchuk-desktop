import QtQuick
import QtQuick.Controls
import EWARNING 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _root
    anchors.fill: parent
    property bool pendingKeep: false

    function itemsText(wallets, keys) {
        var parts = []
        if (wallets > 0) {
            parts.push(wallets === 1 ? STR.STR_QML_2595 : STR.STR_QML_2596.arg(wallets))
        }
        if (keys > 0) {
            parts.push(keys === 1 ? STR.STR_QML_2597 : STR.STR_QML_2598.arg(keys))
        }
        if (parts.length === 2) {
            return STR.STR_QML_2599.arg(parts[0]).arg(parts[1])
        }
        return parts.length === 1 ? parts[0] : ""
    }

    function startMigrate(keep) {
        _root.pendingKeep = keep
        GuestMigration.migrate(keep, "")
    }

    QPopupInfoThreeButtons {
        id: _prompt
        title: STR.STR_QML_2590
        contentText: ""
        closePolicy: Popup.NoAutoClose
        labels: [STR.STR_QML_2592, STR.STR_QML_2593, STR.STR_QML_2594]
        funcs: [
            function() { _root.startMigrate(false) },
            function() { _root.startMigrate(true) },
            function() { GuestMigration.dismissPrompt() }
        ]
    }

    QPopupBusyLoading {
        id: _busy
        closePolicy: Popup.NoAutoClose
        warningText1: STR.STR_QML_2605
        warningText2: STR.STR_QML_2606
    }

    QPopup {
        id: _passphrase
        closePolicy: Popup.NoAutoClose
        onOpened: {
            if (itemInfo) itemInfo.reset()
        }
        content: Item {
            width: 300
            height: _col.implicitHeight
            function reset() {
                _input.textInputted = ""
                _input.showError = false
                _input.isValid = true
                _input.errorText = ""
            }
            function showInvalid() {
                _input.isValid = false
                _input.showError = true
                _input.errorText = STR.STR_QML_238
            }
            Column {
                id: _col
                width: parent.width
                spacing: 16
                QLato {
                    width: parent.width
                    font.weight: Font.Bold
                    text: STR.STR_QML_162
                    horizontalAlignment: Text.AlignHCenter
                }
                QLato {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    text: STR.STR_QML_2603
                    horizontalAlignment: Text.AlignHCenter
                }
                QTextInputBoxTypeB {
                    id: _input
                    label: ""
                    boxWidth: 300
                    boxHeight: 48
                    isPassword: true
                    onTextInputtedChanged: {
                        if (!_input.isValid) {
                            _input.isValid = true
                        }
                        _input.showError = false
                    }
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12
                    QTextButton {
                        width: 120
                        height: 48
                        label.text: STR.STR_QML_035
                        label.font.pixelSize: 16
                        type: eTypeF
                        enabled: !GuestMigration.busy
                        onButtonClicked: {
                            _passphrase.close()
                            GuestMigration.dismissPrompt()
                        }
                    }
                    QTextButton {
                        width: 120
                        height: 48
                        label.text: STR.STR_QML_097
                        label.font.pixelSize: 16
                        type: eTypeE
                        enabled: _input.textInputted !== "" && !GuestMigration.busy
                        onButtonClicked: {
                            var pass = _input.textInputted
                            _input.textInputted = ""
                            GuestMigration.migrate(_root.pendingKeep, pass)
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: GuestMigration
        function onBusyChanged() {
            if (GuestMigration.busy) {
                _busy.open()
            } else {
                _busy.close()
            }
        }
        function onPromptRequested(wallets, keys) {
            _prompt.contentText = STR.STR_QML_2591.arg(_root.itemsText(wallets, keys))
            _prompt.open()
        }
        function onPassphraseRequired(keep) {
            _root.pendingKeep = keep
            _passphrase.open()
        }
        function onPassphraseInvalid(keep) {
            _root.pendingKeep = keep
            if (!_passphrase.opened) {
                _passphrase.open()
            }
            if (_passphrase.itemInfo) _passphrase.itemInfo.showInvalid()
        }
        function onMigrateFinished(success, keep, wallets, keys, reason, message) {
            _passphrase.close()
            if (success) {
                var items = _root.itemsText(wallets, keys)
                var text = keep ? STR.STR_QML_2600.arg(items) : STR.STR_QML_2601.arg(items)
                AppModel.showToast(0, text, EWARNING.SUCCESS_MSG)
                return
            }
            var errorText = ""
            if (reason === "ACCOUNT_HAS_DATA") {
                errorText = STR.STR_QML_2607
            } else if (reason === "INVALID_ACCOUNT") {
                errorText = STR.STR_QML_2608
            } else if (reason === "GENERIC") {
                errorText = STR.STR_QML_2604.arg(message)
            }
            if (errorText !== "") {
                AppModel.showToast(-1, errorText, EWARNING.EXCEPTION_MSG)
            }
        }
    }
}
