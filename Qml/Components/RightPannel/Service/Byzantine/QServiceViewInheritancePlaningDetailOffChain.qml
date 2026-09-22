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
import Qt.labs.platform 1.1
import Qt5Compat.GraphicalEffects
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import Features.Inheritance.OffChain.ViewModels 1.0
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../customizes/services"
import "../../../../Screens/OnlineMode/SetupWallets/TimeLocks"
import "../../../../features/inheritance/components"
import "../Common"

Item {
    id: viewByzantineinheritancePlanRoot
    property var    inheritancePlanInfo: ServiceSetting.walletInfo.inheritancePlanInfo
    property var    planInfo: inheritancePlanInfo.planInfo
    property string walletName: ServiceSetting.walletInfo.walletName
    property string myRole: ServiceSetting.walletInfo.myRole
    property bool   isEdit: myRole === "MASTER" || myRole === "ADMIN"

    // NUN-10192: inheritance_keys[] only carries xfp; claim_options lives on the wallet's own key
    // list, so cross-reference by xfp to know each key's sharing method(s).
    function keyClaimOptions(xfp) {
        var keys = ServiceSetting.walletInfo && ServiceSetting.walletInfo.dashboardInfo ? ServiceSetting.walletInfo.dashboardInfo.keys : []
        // Case-insensitive match: inheritance_keys[].xfp casing vs dashboardInfo.keys[].xfp casing is unverified.
        for (var i = 0; i < keys.length; i++) {
            if (String(keys[i].xfp).toUpperCase() === String(xfp).toUpperCase()) {
                return keys[i].claim_options !== undefined ? keys[i].claim_options : []
            }
        }
        return []
    }
    function keyCardIcon(options) {
        return options.indexOf("SEED_PHRASE") !== -1 ? "qrc:/Images/Images/key-dark.svg" : "qrc:/Images/Images/change-password-dark.svg"
    }
    // Seed-capable keys show "Inheritance key (XFP: ...)"; backup-only (or unclassified) keeps the legacy "Backup Password" title.
    function keyCardTitle(xfp, options, index, total) {
        if (options.indexOf("SEED_PHRASE") !== -1) {
            return total > 1 ? QSTR.STR_QML_2038.arg(index + 1).arg(xfp.toUpperCase()) : QSTR.STR_QML_1984.arg(xfp.toUpperCase())
        }
        return QSTR.STR_QML_727
    }
    function keyCardSubtitle(options, index, total) {
        var hasSeed = options.indexOf("SEED_PHRASE") !== -1
        var hasBackup = options.indexOf("ENCRYPTED_BACKUP") !== -1
        if (hasSeed && hasBackup) return QSTR.STR_QML_2325
        if (hasSeed) return total > 1 ? QSTR.STR_QML_2039.arg(index + 1) : QSTR.STR_QML_1986
        return QSTR.STR_QML_917
    }
    // BUGFIX: _img_bg was a fixed height sized for exactly 1 key card; grow it for extra stacked cards (mockup 04D).
    function extraKeyCardsHeight() {
        var n = planInfo.inheritance_keys ? planInfo.inheritance_keys.length : 0
        return n > 1 ? (n - 1) * 104 : 0
    }
    function keyInfoClicked(options) {
        var hasSeed = options.indexOf("SEED_PHRASE") !== -1
        var hasBackup = options.indexOf("ENCRYPTED_BACKUP") !== -1
        if (hasSeed && hasBackup) {
            _BackupPassword.isFinalStep = false
            _BackupPassword.open()
        } else if (hasSeed) {
            _SeedPhraseBackup.isJointVariant = false
            _SeedPhraseBackup.open()
        } else {
            _BackupPassword.isFinalStep = true
            _BackupPassword.open()
        }
    }
    QContextMenu {
        id: optionMenu
        menuWidth: 300
        icons: {
            var ls = []
            if(myRole === "MASTER" || myRole === "ADMIN"){
                ls.push("qrc:/Images/Images/close-24px.svg")
            }
            return ls
        }
        labels: {
            var ls = []
            if(myRole === "MASTER" || myRole === "ADMIN"){
                ls.push(QSTR.STR_QML_844)
            }
            return ls
        }
        colors: {
            var ls = []
            if(myRole === "MASTER" || myRole === "ADMIN"){
                ls.push("#CF4018")
            }
            return ls
        }
        onItemClicked: {
            switch(index){
            case 0:
                vm.cancelInheritancePlan()
                break;
            default:
                break;
            }
        }
    }

    Column {
        id: columnHeader
        anchors.fill: parent
        spacing: 0
        Rectangle {
            id: _img_bg
            width: parent.width
            // BUGFIX: the "Funds become claimable after" block used to be excluded from CUSTOMIZE's
            // layout (visible: false -> Column skips it); now it always shows, so CUSTOMIZE's fixed
            // budget needs +122 (label + 62px card + the Column spacing this block no longer skips).
            height: (vm.distribution_method == "CUSTOMIZE" ? 376 + 122 : 486) + extraKeyCardsHeight()
            color: "#D0E2FF"
            Column {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 24
                Item {
                    width: parent.width
                    height: 48
                    QLato {
                        font.pixelSize: 28
                        color: "#031F2B"
                        font.weight: Font.Bold
                        text: QSTR.STR_QML_843
                    }
                    QIconButton{
                        width: 48
                        height: 48
                        anchors.right: parent.right
                        // BUGFIX: optionMenu has 0 items for non-MASTER/ADMIN roles; hide the button too.
                        visible: inheritancePlanInfo.isActived && isEdit
                        bgColor: "#D0E2FF"
                        icon: "qrc:/Images/Images/more-horizontal-dark.svg"
                        onClicked: {
                            optionMenu.popup()
                        }
                    }
                }
                Loader {
                    sourceComponent: headerplanOffchain
                    width: parent.width
                    height: _img_bg.height - 24 - 48 - 24*2
                }
            }
        }

        Item {
            width: parent.width
            height: parent.height - _img_bg.height - normalRect.height
            Loader {
                sourceComponent: bodyplanOffchain
                anchors.fill: parent
            }
        }
    }

    Rectangle {
        id: normalRect
        height: 80
        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 4
            samples: radius * 2
            source: normalRect
            color: Qt.rgba(0, 0, 0, 0.5)
        }
        Row {
            anchors{
                right: parent.right
                rightMargin: 24
                bottom: parent.bottom
                bottomMargin: 16
            }
            spacing: 12
            layoutDirection: Qt.RightToLeft
            QTextButton {
                id: _save
                width: 66
                height: 48
                label.text: QSTR.STR_QML_835
                label.font.pixelSize: 16
                type: eTypeE
                enabled: planInfo.edit_isChanged || vm.isDataChanged
                onButtonClicked: {
                    vm.finalizeChanges()
                }
            }
            QTextButton {
                width: 148
                height: 48
                label.text: QSTR.STR_QML_805
                label.font.pixelSize: 16
                type: eTypeF
                enabled: planInfo.edit_isChanged || vm.isDataChanged
                onButtonClicked: {
                    vm.discardChanges()
                }
            }
        }
    }
    Component {
        id: headerplanOffchain
        Rectangle {
            radius: 24
            color: "#2F466C"
            Column {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 24
                Item {
                    width: parent.width
                    height: 60
                    Row {
                        width: parent.width
                        spacing: 12
                        QIcon {
                            iconSize: 60
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/wallet-brand-icon.svg"
                        }
                        Column {
                            width: 304
                            height: 44
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4
                            QLato {
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                color: "#FFFFFF"
                                text: QSTR.STR_QML_845
                            }
                            QLato {
                                color: "#FFFFFF"
                                text: walletName
                            }
                        }
                    }
                    Row {
                        width: parent.width
                        layoutDirection: Qt.RightToLeft
                        spacing: 12
                        QTextButton {
                            width: 162
                            height: 48
                            label.text: QSTR.STR_QML_847
                            label.font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            type: eTypeC
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_SHARE_YOUR_SECRET_REQUEST)
                            }
                        }
                        QTextLink {
                            width: 220
                            height: 48
                            font.weight: Font.Bold
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            text: QSTR.STR_QML_846
                            anchors.verticalCenter: parent.verticalCenter
                            font.underline: false
                            onTextClicked: {
                                Qt.openUrlExternally("https://nunchuk.io/howtoclaim")
                            }
                        }
                    }
                }
                Column {
                    width: parent.width
                    spacing: 12
                    QLato {
                        font.pixelSize: 16
                        color: "#FFFFFF"
                        text: QSTR.STR_QML_1983
                    }
                    Row {
                        id: row_key
                        spacing: 12
                        QInheritancePlanMagicPhrases {
                            magicPhrases: vm.assetAllocation
                            magic: planInfo.magic
                        }
                        // NUN-10192: 1 card per inheritance key, content driven by that key's claim_options (mockup 01D-04D).
                        Column {
                            spacing: 12
                            Repeater {
                                model: planInfo.inheritance_keys ? planInfo.inheritance_keys.length : 0
                                Rectangle {
                                    property string _xfp: planInfo.inheritance_keys[index].xfp
                                    property var _claimOptions: viewByzantineinheritancePlanRoot.keyClaimOptions(_xfp)
                                    width: 393
                                    height: 92
                                    color: "#FFFFFF"
                                    radius: 12
                                    Row {
                                        spacing: 12
                                        anchors.fill: parent
                                        anchors.margins: 12
                                        QIcon {
                                            iconSize: 24
                                            source: viewByzantineinheritancePlanRoot.keyCardIcon(_claimOptions)
                                        }
                                        Column {
                                            spacing: 12
                                            QLato {
                                                width: 333
                                                font.pixelSize: 16
                                                color: "#1C1C1C"
                                                text: viewByzantineinheritancePlanRoot.keyCardTitle(_xfp, _claimOptions, index, planInfo.inheritance_keys.length)
                                                textFormat: Text.RichText
                                                font.weight: Font.Bold
                                                QLato {
                                                    anchors.right: parent.right
                                                    font.pixelSize: 16
                                                    color: "#1C1C1C"
                                                    text:  "Info"
                                                    font.underline: true
                                                    font.weight: Font.Bold
                                                    MouseArea {
                                                        anchors.fill: parent
                                                        hoverEnabled: true
                                                        cursorShape: Qt.PointingHandCursor
                                                        onClicked: {
                                                            viewByzantineinheritancePlanRoot.keyInfoClicked(_claimOptions)
                                                        }
                                                    }
                                                }
                                            }
                                            QLato {
                                                font.pixelSize: 16
                                                color: "#1C1C1C"
                                                text: viewByzantineinheritancePlanRoot.keyCardSubtitle(_claimOptions, index, planInfo.inheritance_keys.length)
                                                width: 333
                                                wrapMode: Text.WordWrap
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Column {
                        spacing: 12
                        // BUGFIX: off-chain timelock value is method-agnostic (NUN-10192); must always show.
                        QLato {
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            text: QSTR.STR_QML_2101
                        }
                        Rectangle {
                            width: 393
                            height: 62
                            color: "#2A3F61"
                            radius: 12
                            Row {
                                spacing: 12
                                anchors.fill: parent
                                anchors.margins: 12
                                QImage {
                                    width: 24
                                    height: 24
                                    source: "qrc:/Images/Images/calendar-dark.png"
                                }
                                Column {
                                    width: 292
                                    spacing: 2
                                    QLato {
                                        font.pixelSize: 16
                                        color: "#FFFFFF"
                                        text: vm.valueDate
                                    }
                                    QLato {
                                        font.pixelSize: 12
                                        color: "#FFFFFF"
                                        text: vm.valueTimezone
                                    }
                                }
                                QLato {
                                    width:  29
                                    color: "#FFFFFF"
                                    text:  "Edit"
                                    font.underline: true
                                    font.weight: Font.Bold
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            vm.timeLockEditClicked()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    Component {
        id: bodyplanOffchain
        Item {
            anchors.fill: parent
            Flickable {
                anchors.top: parent.top
                anchors.topMargin: vm.distribution_method == "CUSTOMIZE" ? 0 : 24
                clip: true
                width: parent.width
                height: parent.height
                contentWidth: width
                contentHeight: _colum.childrenRect.height + 100
                ScrollBar.vertical: QScrollBar { }
                Column {
                    id: _colum
                    anchors.top: parent.top
                    width: parent.width - 8
                    spacing: 24
                    QInheritanceOverview {
                        visible: vm.distribution_method == "CUSTOMIZE"
                        onAssetAllocationEditClicked: vm.onAssetAllocationEditClicked()
                        onReleaseMethodEditClicked: vm.onReleaseMethodEditClicked()
                        onReleaseScheduleEditClicked: vm.onReleaseScheduleEditClicked()
                        onTimezoneEditClicked: vm.onTimezoneEditClicked()
                        onFallbackSettingsEditClicked: vm.onFallbackSettingsEditClicked()
                        onBeneficiarySchedulesEditClicked: vm.onBeneficiarySchedulesEditClicked()
                    }
                    QTextAreaBoxTypeB {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        width: 651
                        height: 128
                        label.text: QSTR.STR_QML_850
                        input.text: planInfo.note
                        input.backgroundColor: "#F5F5F5"
                        input.verticalAlignment: Text.AlignTop
                        input.height: 96
                        input.readOnly: true
                        textColor: "#031F2B"
                        onTextEditClicked: {
                            if(isEdit) {
                                QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_LEAVE_MESSAGE)
                            }
                        }
                    }
                    Rectangle {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        height: 1
                        width: 651
                        color: "#EAEAEA"
                        visible: vm.distribution_method != "CUSTOMIZE" && (vm.beneficiary_mode === "SINGLE" || (vm.beneficiary_mode !== "SINGLE" && vm.release_method === "INDIVIDUAL"))
                    }
                    QTextInputBoxTypeE {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        visible: vm.distribution_method != "CUSTOMIZE" && (vm.beneficiary_mode === "SINGLE" || (vm.beneficiary_mode !== "SINGLE" && vm.release_method === "INDIVIDUAL"))
                        width: 651
                        height: 84
                        label.text: QSTR.STR_QML_851
                        input.text: planInfo.buffer_period.id === "" ? QSTR.STR_QML_921 : planInfo.buffer_period.display_name
                        input.backgroundColor: "#F5F5F5"
                        input.height: 52
                        input.readOnly: true
                        textColor: "#031F2B"
                        onTextEditClicked: {
                            if(isEdit) {
                                QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_BUFFER_PERIOD)
                            }
                        }
                    }
                    Rectangle {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        height: 1
                        width: 651
                        color: "#EAEAEA"
                    }
                    QServiceInheritanceNotificationPreferencesOffchain {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        width: 651
                        inheritance: planInfo
                        onTextEditClicked: {
                            if(isEdit) {
                                QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_NOTIFICATION)
                            }
                        }
                    }
                    Item {
                        width: 651
                        height: 80
                    }
                }
            }
        }
    }
    
    Connections {
        target: inheritancePlanInfo
        // BUGFIX: modernize deprecated implicit onFoo Connections syntax (Qt warning), no behavior change.
        function onSecurityQuestionClosed() {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                _Security.close()
            }
        }
        function onInheritanceDummyTransactionAlert() {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                QMLHandle.sendEvent(EVT.EVT_HEALTH_CHECK_STARTING_REQUEST)
            }
        }
    }
    ViewInheritancePlanViewModel {
        id: vm
    }
}
