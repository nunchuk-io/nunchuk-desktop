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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Transactions"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _context
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1456
    property bool isEdit: false
    property bool isCreateTag: false
    property var coinTags: walletInfo.coinTags
    property var coinCollections: walletInfo.coinCollections
    property int radioSelect: _DESCENDING
    readonly property int _DESCENDING: 0
    readonly property int _ASCENDING: 1
    property string minAmount: ""
    property string maxAmount: ""
    property bool hasBack: false
    property bool includeLockedCoins: true
    property string typeApply: "apply-filter-coin-collection"
    property string typeClear: "clear-all-filter-coin-collection"
    property bool isCurrency: false
    property var filterData
    RegularExpressionValidator { id: satValidator;   regularExpression: /^[1-9][0-9]*$/ }
    RegularExpressionValidator { id: currencyValidator;   regularExpression: /^(?:0|[1-9][0-9]*)(\.\d{1,2})?$/ }
    RegularExpressionValidator { id: btcValidator;   regularExpression: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }
    property string amountUnit: (isCurrency ? AppSetting.currency : RoomWalletData.unitValue).replace(" ", "")

    function getBalanceUnit() {
        var balance = ""
        if (isCurrency) {
            balance = "0.00"
        } else if (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) {
            balance = "0"
        } else {
            balance = "0.00"
        }
        return qsTr("%1 %2").arg(balance).arg(amountUnit)
    }
    function getValidator() {
        if (isCurrency) {
            return currencyValidator
        } else if (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) {
            return satValidator
        } else {
            return btcValidator
        }
    }

    extraHeader: Item{}
    content: Flickable {
        id: _item
        clip: true
        contentWidth: _item.width
        contentHeight: _column.childrenRect.height
        ScrollBar.vertical: QScrollBar { }
        interactive: true
        Column {
            id: _column
            width: _item.width - 8
            spacing: 24
            QFilterCoinTextBox {
                boxWidth: _column.width
                boxHeight: 48
                label: STR.STR_QML_1417
                input.placeholderText: coinTags.selectedCount === 0 ? STR.STR_QML_1457 : ""
                textInputted: coinTags.selectedCount === 0 ? "" : qsTr("%1 tag%2 selected").arg(coinTags.selectedCount).arg(coinTags.selectedCount <= 1 ? "" : "s")
                enabled: true
                showArrow: true
                input.readOnly: true
                onArrowClicked: {
                    isCleared = false
                    coinTags.storeSelected()
                    _tags.open()
                }
            }

            QLine {
                width: _column.width
            }

            QFilterCoinTextBox {
                boxWidth: _column.width
                boxHeight: 48
                label: STR.STR_QML_1418
                input.placeholderText: coinCollections.selectedCount === 0 ? STR.STR_QML_1457 : ""
                textInputted: coinCollections.selectedCount === 0 ? "" : qsTr("%1 collection%2 selected").arg(coinCollections.selectedCount).arg(coinCollections.selectedCount <= 1 ? "" : "s")
                enabled: true
                showArrow: true
                onArrowClicked: {
                    isCleared = false
                    coinCollections.storeSelected()
                    _collections.open()
                }
            }

            QLine {
                width: _column.width
            }
            Column {
                width: parent.width
                spacing: 16
                QLato {
                    width: _column.width
                    height: 20
                    font.weight: Font.Bold
                    text: STR.STR_QML_214
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                QFilterAmountCoinTextBox {
                    boxWidth: _column.width
                    boxHeight: 48
                    label: STR.STR_QML_1458
                    input.placeholderText: getBalanceUnit()
                    textInputted: minAmount
                    validator: getValidator()
                    onTypingFinished: (currentText) => {
                        isCleared = false
                        minAmount = currentText
                    }
                }

                QFilterAmountCoinTextBox {
                    boxWidth: _column.width
                    boxHeight: 48
                    label: STR.STR_QML_1459
                    input.placeholderText: getBalanceUnit()
                    textInputted: maxAmount
                    validator: getValidator()
                    onTypingFinished: (currentText) => {
                        isCleared = false
                        maxAmount = currentText
                    }
                }
            }

            QLine {
                width: _column.width
            }

            Column {
                width: parent.width
                spacing: 16
                QLato {
                    width: _column.width
                    height: 20
                    font.weight: Font.Bold
                    text: STR.STR_QML_1463
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Item {
                    width: _column.width
                    height: 72
                    QTextInputBoxTypeB {
                        id: _from
                        label: STR.STR_QML_1461
                        boxWidth: _column.width
                        boxHeight: 48
                        textInputted: _calendarFrom.dateString
                        input.placeholderText: "MM/DD/YYYY"
                        onTextInputtedChanged: {
                            if(!_from.isValid){
                                _from.isValid = true
                                _from.errorText = ""
                            }
                            _from.showError = false;
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        propagateComposedEvents: true
                        onClicked: {
                            isCleared = false
                            _calendarFrom.open()
                        }
                    }
                }
                Item {
                    width: _column.width
                    height: 72
                    QTextInputBoxTypeB {
                        id: _to
                        label: STR.STR_QML_1462
                        boxWidth: _column.width
                        boxHeight: 48
                        textInputted: _calendarTo.dateString
                        input.placeholderText: "MM/DD/YYYY"
                        onTextInputtedChanged: {
                            if(!_to.isValid){
                                _to.isValid = true
                                _to.errorText = ""
                            }
                            _to.showError = false;
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        propagateComposedEvents: true
                        onClicked: {
                            isCleared = false
                            _calendarTo.open()
                        }
                    }
                }
            }

            QLine {
                width: _column.width
                visible: includeLockedCoins
            }

            Column {
                width: parent.width
                spacing: 16
                visible: includeLockedCoins
                QLato {
                    width: _column.width
                    height: 20
                    font.weight: Font.Bold
                    text: STR.STR_QML_1464
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Column {
                    width: parent.width
                    QCheckBoxButton{
                        id: _locked
                        width: _column.width
                        height: 48
                        label: STR.STR_QML_1465
                        onButtonClicked: {
                            isCleared = false
                        }
                    }
                    QCheckBoxButton{
                        id: _unlocked
                        width: _column.width
                        height: 48
                        label: STR.STR_QML_1466
                        onButtonClicked: {
                            isCleared = false
                        }
                    }
                }
            }

            QLine {
                width: _column.width
            }

            Column {
                width: parent.width
                spacing: 16
                QLato {
                    width: _column.width
                    height: 20
                    font.weight: Font.Bold
                    text: STR.STR_QML_1137
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Column {
                    width: parent.width
                    QRadioButtonTypeA {
                        width: _column.width
                        height: 48
                        layoutDirection: Qt.LeftToRight
                        label: STR.STR_QML_1467
                        selected: radioSelect === _DESCENDING
                        onButtonClicked: {
                            isCleared = false
                            radioSelect = _DESCENDING
                        }
                    }
                    QRadioButtonTypeA {
                        width: _column.width
                        height: 48
                        layoutDirection: Qt.LeftToRight
                        label: STR.STR_QML_1468
                        selected: radioSelect === _ASCENDING
                        onButtonClicked: {
                            isCleared = false
                            radioSelect = _ASCENDING
                        }
                    }
                    Row {
                        visible: (walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT)
                        spacing: 0
                        QLato {
                            width: _column.width - 64
                            text: STR.STR_QML_1880
                            color: "#031F2B"
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QSwitchTypeB {
                            id: ageSortSwitch
                            width: 64
                            height: 36
                            switchOn: walletInfo.sortByCoinAge
                            anchors.verticalCenter: parent.verticalCenter
                            onButtonClicked: {
                                isCleared = false
                            }
                        }
                    }
                }
                Item {
                    width: _column.width
                    height: 24
                }
            }
        }

        function apply() {
            var input = {}
            if (isCleared) {
                isCleared = false
                input = {
                    type: typeClear,
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
                return
            } else {
                input = {
                    type: typeApply,
                    "minAmount": minAmount,
                    "maxAmount": maxAmount,
                    "isCurrency": isCurrency,
                    "fromDate": _calendarFrom.dateString,
                    "toDate": _calendarTo.dateString,
                    "isLocked": _locked.checked,
                    "isUnLocked": _unlocked.checked,
                    "sortOrder": radioSelect,
                    "sortByCoinAge": ageSortSwitch.switchOn
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            }
        }
        function clear() {
            isCleared = true
            _locked.checked = false
            _unlocked.checked = false
            radioSelect = _DESCENDING
            minAmount = ""
            maxAmount = ""
            isCurrency = false
            coinTags.selectAll(false)
            coinCollections.selectAll(false)
            _calendarFrom.dateString = ""
            _calendarTo.dateString = ""
        }
        function init() {
            _locked.checked = filterData.isLocked
            _unlocked.checked = filterData.isUnLocked
            radioSelect = filterData.sortOrder
            minAmount = filterData.minAmount
            maxAmount = filterData.maxAmount
            _calendarFrom.dateString = filterData.fromDate
            _calendarTo.dateString = filterData.toDate
            isCurrency = filterData.isCurrency
        }
        Component.onCompleted: {
            init()
        }
    }
    property bool isCleared: true
    onIsCurrencyChanged: {
        isCleared = false
    }

    isShowLine: true
    signal backScreen()
    bottomLeft: Row {
        spacing: 8
        QButtonTextLink {
            width: 97
            height: 48
            label: STR.STR_QML_059
            visible: hasBack
            onButtonClicked: {
                _context.backScreen()
            }
        }
        QTextButton {
            width: 80
            height: 48
            label.text: STR.STR_QML_245
            label.font.pixelSize: 16
            type: eTypeF
            visible: !hasBack
            onButtonClicked: {
                _context.closeClicked()
            }
        }
        QTextButton {
            width: 89
            height: 48
            label.text: STR.STR_QML_1469
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                _context.contentItem.clear()
            }
        }
    }

    signal applyClicked()
    bottomRight: QTextButton {
        width: 74
        height: 48
        label.text: STR.STR_QML_194
        type: eTypeE
        onButtonClicked: {
            _context.contentItem.apply()
             _context.applyClicked()
        }
    }

    QPopupCalendar {
        id: _calendarFrom
    }
    QPopupCalendar {
        id: _calendarTo
    }

    QSearchFilterTags {
        id: _tags
    }

    QSearchFilterCollections {
        id: _collections
    }
}
