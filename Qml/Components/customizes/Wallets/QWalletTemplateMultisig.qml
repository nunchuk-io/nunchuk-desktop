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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1810
    property string templateMiniscript: "flexible-multisig"
    readonly property int maxN: 20

    property bool isMNEditting: true
    property bool isNewMNditting: false
    property bool isTimeEditting: false
    function clearEditing() {
        isMNEditting = false
        isNewMNditting = false
        isTimeEditting = false
    }
    function timeEditting() {
        console.log("timeEditting", newWalletInfo.timelockType, newWalletInfo.timelockType)
        if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.TIME_LOCK) {
            return qsTr("%1").arg(newWalletInfo.timeMini.absoluteTimestamp.valueDisplay)
        }
        else if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.TIME_LOCK) {
            console.log(newWalletInfo.timeMini.relativeTimestamp.valueDisplay)
            console.log(newWalletInfo.timeMini.relativeTimestamp.valueDay)
            console.log(newWalletInfo.timeMini.relativeTimestamp.valueHour)
            return qsTr("%1").arg(newWalletInfo.timeMini.relativeTimestamp.valueDisplay)
        }
        else if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK) {
            return qsTr("block %1").arg(newWalletInfo.timeMini.absoluteBlockheight)
        }
        else if(newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE && newWalletInfo.timeUnit === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK) {
            return qsTr("%1 blocks").arg(newWalletInfo.timeMini.relativeBlockheight)
        }
        return ""
    }
    content: Item {
        Row {
            spacing: 36
            Item {
                width: 350
                height: 512
                Column {
                    width: parent.width
                    spacing: 16
                    Column {
                        width: parent.width
                        spacing: 8
                        QLato {
                            width: parent.width
                            text: STR.STR_QML_1818
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        Loader {
                            id: templateLoader
                            width: parent.width
                            height: newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE ? 184 : 284
                            sourceComponent: templateMiniscript === "flexible-multisig" ? flexible_multisig :
                                            templateMiniscript === "expanding-multisig" ? expanding_multisig :
                                            templateMiniscript === "decaying-multisig" ? decaying_multisig : null


                        }
                    }
                    Row {
                        width: parent.width
                        height: 28
                        spacing: 4
                        QCheckBox {
                            id: selectAllBox
                            checked: newWalletInfo.reUseKeys
                            partiallyChecked: false
                            anchors.verticalCenter: parent.verticalCenter
                            onCheckboxClicked: {
                                newWalletInfo.reUseKeys = !newWalletInfo.reUseKeys
                            }
                        }
                        QLato {
                            width: parent.width - 24
                            text: STR.STR_QML_1819
                            font.pixelSize: 16
                            font.weight: Font.Normal
                            wrapMode: Text.WordWrap
                            verticalAlignment: Text.AlignVCenter
                            anchors.top: selectAllBox.top
                        }
                    }
                }                
            }
            Item {
                width: 352
                height: 512
                Loader {
                    id: configLoader
                    width: parent.width
                    height: 512
                    sourceComponent: if (isMNEditting) {
                        return oldConfig
                    } else if (isNewMNditting) {
                        return templateMiniscript === "flexible-multisig" ? newConfigFlexible :
                                    templateMiniscript === "expanding-multisig" ? newConfigExpanding :
                                    templateMiniscript === "decaying-multisig" ? newConfigDecaying : oldConfig
                    } else if (isTimeEditting) {
                        return editTimelock
                    } else {
                        return oldConfig
                    }
                }
            }
        }
    }
    onNextClicked: {
        if(newWalletInfo.miniscriptTemplateSelected(templateMiniscript)) {
            switchEnterMiniScript()
        }
    }

    Component {
        id: expanding_multisig
        Loader {
            sourceComponent: newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE ? expanding_multisig_absolute : expanding_multisig_relative
        }
    }
    Component {
        id: decaying_multisig
        Loader {
            sourceComponent: newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE ? decaying_multisig_absolute : decaying_multisig_relative
        }
    }
    Component {
        id: flexible_multisig
        Loader {
            sourceComponent: newWalletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_ABSOLUTE ? flexible_multisig_absolute : flexible_multisig_relative
        }
    }

    Component {
        id: oldConfig
        Column {
            width: parent.width
            spacing: 24
            QLato {
                width: parent.width
                text: STR.STR_QML_1817
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            QNumberChanged {
                headline: STR.STR_QML_1652
                description: STR.STR_QML_1826
                value: newWalletInfo.walletM
                minusEnabled: newWalletInfo.walletM > 1
                plusEnabled: newWalletInfo.walletM < maxN
                onValueUpdated: (newValue) => {
                                    newWalletInfo.walletM = newValue
                                    if (newWalletInfo.walletN < newWalletInfo.walletM) {
                                        newWalletInfo.walletN = newWalletInfo.walletM
                                    }
                                    if (templateMiniscript === "expanding-multisig") {
                                        const candidate = newWalletInfo.walletN + 1
                                        newWalletInfo.newWalletN = (candidate > maxN) ? maxN : candidate
                                    }
                                    else if (templateMiniscript === "decaying-multisig") {
                                        newWalletInfo.newWalletM = Math.max(newWalletInfo.walletM - 1, 1)
                                    }
                                    else{}
                                }
            }

            QNumberChanged {
                headline: STR.STR_QML_1827
                description: STR.STR_QML_1828
                value: newWalletInfo.walletN
                minusEnabled: newWalletInfo.walletN > newWalletInfo.walletM
                plusEnabled: newWalletInfo.walletN < maxN
                onValueUpdated: (newValue) => {
                                    newWalletInfo.walletN = newValue;
                                    if (newWalletInfo.walletM > newWalletInfo.walletN) {
                                        newWalletInfo.walletM = newWalletInfo.walletN;
                                    }
                                    if (templateMiniscript === "expanding-multisig") {
                                        const candidate = newWalletInfo.walletN + 1
                                        newWalletInfo.newWalletN = (candidate > maxN) ? maxN : candidate
                                    }
                                }
            }
        }
    }
    Component {
        id: newConfigExpanding
        Column {
            width: parent.width
            spacing: 24
            QLato {
                width: parent.width
                text: STR.STR_QML_1817
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QNumberChanged {
                headline: STR.STR_QML_1827
                description: STR.STR_QML_1828
                value: newWalletInfo.newWalletN
                minusEnabled: newWalletInfo.newWalletN > (newWalletInfo.walletN + 1)
                plusEnabled: newWalletInfo.newWalletN < maxN
                onValueUpdated: (newValue) => {
                    newWalletInfo.newWalletN = newValue;
                }
            }
        }
    }
    Component {
        id: newConfigDecaying
        Column {
            width: parent.width
            spacing: 24
            QLato {
                width: parent.width
                text: STR.STR_QML_1817
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QNumberChanged {
                headline: STR.STR_QML_1652
                description: STR.STR_QML_1826
                value: newWalletInfo.newWalletM

                minusEnabled: newWalletInfo.newWalletM > 1
                plusEnabled: newWalletInfo.newWalletM < newWalletInfo.walletM - 1

                onValueUpdated: (newValue) => {
                                    const clampedValue = Math.max(1, Math.min(newValue, newWalletInfo.walletM - 1))
                                    newWalletInfo.newWalletM = clampedValue

                                    if (clampedValue >= newWalletInfo.walletM) {
                                        newWalletInfo.walletM = clampedValue + 1
                                    }
                                }
            }
        }
    }
    Component {
        id: newConfigFlexible
        Column {
            width: parent.width
            spacing: 24
            QLato {
                width: parent.width
                text: STR.STR_QML_1817
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QNumberChanged {
                headline: STR.STR_QML_1652
                description: STR.STR_QML_1826
                value: newWalletInfo.newWalletM
                minusEnabled: newWalletInfo.newWalletM > 1
                plusEnabled: newWalletInfo.newWalletM < maxN
                onValueUpdated: (newValue) => {
                                    newWalletInfo.newWalletM = newValue;
                                    if (newWalletInfo.newWalletN < newWalletInfo.newWalletM) {
                                        newWalletInfo.newWalletN = newWalletInfo.newWalletM
                                    }
                                }
            }
            QNumberChanged {
                headline: STR.STR_QML_1827
                description: STR.STR_QML_1828
                value: newWalletInfo.newWalletN
                minusEnabled: newWalletInfo.newWalletN > newWalletInfo.newWalletM
                plusEnabled: newWalletInfo.newWalletN < maxN
                onValueUpdated: (newValue) => {
                                    newWalletInfo.newWalletN = newValue;
                                    if (newWalletInfo.newWalletM > newWalletInfo.newWalletN) {
                                        newWalletInfo.newWalletM = newWalletInfo.newWalletN;
                                    }
                                }
            }
        }
    }
    Component {
        id: editTimelock
        Flickable {
            id: flickerWalletList
            anchors.fill: parent
            clip: true
            flickableDirection: Flickable.VerticalFlick
            interactive: true
            contentHeight: timelockEditComponent.height
            ScrollBar.vertical: ScrollBar { active: true }
            QMiniscriptEditTimelock {
                id: timelockEditComponent
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Component {
        id: expanding_multisig_absolute
        Rectangle {
            width: parent.width
            height: 184
            color: "#F5F5F5"
            radius: 8
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "Until"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        height: parent.height
                        isEditting: isTimeEditting
                        label: timeEditting()
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isTimeEditting = isEditting
                                         }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "spending requires"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        height: parent.height
                        width: paintedWidth
                        text: qsTr("signature%1 from a").arg(newWalletInfo.walletN == 1 ? "" : "s")
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isMNEditting
                        height: parent.height
                        label: newWalletInfo.walletN == 1 ? "singlesig" : qsTr("%1-of-%2").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isMNEditting = isEditting
                                         }
                    }
                    QLato {
                        visible: newWalletInfo.walletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                QLato {
                    width: paintedWidth
                    height: 28
                    text: "After that, spending requires either the same"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row{
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: newWalletInfo.walletN == 1 ? qsTr("singlesig OR a %1 of").arg(newWalletInfo.walletM) : qsTr("%1-of-%2 multisig OR a %1 of").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN).arg(newWalletInfo.walletM)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isNewMNditting
                        label: qsTr("%1").arg(newWalletInfo.newWalletN)
                        anchors.verticalCenter: parent.verticalCenter
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isNewMNditting = isEditting
                                         }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
    Component {
        id: expanding_multisig_relative
        Rectangle {
            width: parent.width
            height: 284
            color: "#F5F5F5"
            radius: 8
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "First"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        height: parent.height
                        isEditting: isTimeEditting
                        label: timeEditting()
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isTimeEditting = isEditting
                                         }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "after a coin is received:"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                QLato {
                    height: 28
                    width: paintedWidth
                    text: qsTr("spending that coin requires signature%1").arg(newWalletInfo.walletN == 1 ? "" : "s")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        height: parent.height
                        width: paintedWidth
                        text: "from a"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isMNEditting
                        height: parent.height
                        label: newWalletInfo.walletN == 1 ? "singlesig" : qsTr("%1 of %2").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isMNEditting = isEditting
                                         }
                    }
                    QLato {
                        visible: newWalletInfo.walletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                QLato {
                    width: paintedWidth
                    height: 28
                    text: "After that, the same coin can be spent with"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: paintedWidth
                    height: 28
                    text: newWalletInfo.walletN == 1 ? qsTr("either the same singlesig OR a ") : qsTr("either the same %1-of‑%2 multisig OR a ").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row{
                    height: 28
                    width: parent.width
                    spacing: 4
                    QContractPolicyEdit{
                        isEditting: isNewMNditting
                        label: qsTr("%1").arg(newWalletInfo.newWalletN)
                        anchors.verticalCenter: parent.verticalCenter
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isNewMNditting = isEditting
                                         }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: qsTr("of %1 multisig.").arg(newWalletInfo.walletN)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
    Component {
        id: decaying_multisig_absolute
        Rectangle {
            width: parent.width
            height: 184
            color: "#F5F5F5"
            radius: 8
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "Until"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        height: parent.height
                        isEditting: isTimeEditting
                        label: timeEditting()
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isTimeEditting = isEditting
                        }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "spending requires"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        height: parent.height
                        width: paintedWidth
                        text: qsTr("signature%1 from a").arg(newWalletInfo.walletN == 1 ? "" : "s")
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isMNEditting
                        height: parent.height
                        label: newWalletInfo.walletN == 1 ? "singlesig" : qsTr("%1 of %2").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isMNEditting = isEditting
                        }
                    }
                    QLato {
                        visible: newWalletInfo.walletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                QLato {
                    width: paintedWidth
                    height: 28
                    text: "After that, spending requires either the same"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row{
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: newWalletInfo.walletN == 1 ? qsTr("singlesig OR a") : qsTr("%1 of %2 multisig OR a").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isNewMNditting
                        label: qsTr("%1").arg(newWalletInfo.newWalletM)
                        anchors.verticalCenter: parent.verticalCenter
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isNewMNditting = isEditting
                        }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: qsTr("of-%1 multisig.").arg(newWalletInfo.walletN)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
    Component {
        id: decaying_multisig_relative
        Rectangle {
            width: parent.width
            height: 284
            color: "#F5F5F5"
            radius: 8
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "First"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        height: parent.height
                        isEditting: isTimeEditting
                        label: timeEditting()
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isTimeEditting = isEditting
                                         }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "after a coin is received:"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                QLato {
                    height: 28
                    width: paintedWidth
                    text: qsTr("spending that coin requires signature%1").arg(newWalletInfo.walletN == 1 ? "" : "s")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        height: parent.height
                        width: paintedWidth
                        text: "from a"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isMNEditting
                        height: parent.height
                        label: newWalletInfo.walletN == 1 ? "singlesig" : qsTr("%1 of %2").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isMNEditting = isEditting
                                         }
                    }
                    QLato {
                        visible: newWalletInfo.walletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                QLato {
                    width: paintedWidth
                    height: 28
                    text: "After that, the same coin can be spent with"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: paintedWidth
                    height: 28
                    text: newWalletInfo.walletN == 1 ? qsTr("either the same singlesig OR a ") : qsTr("either the same %1-of‑%2 multisig OR a ").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row{
                    height: 28
                    width: parent.width
                    spacing: 4
                    QContractPolicyEdit{
                        isEditting: isNewMNditting
                        label: qsTr("%1").arg(newWalletInfo.newWalletM)
                        anchors.verticalCenter: parent.verticalCenter
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isNewMNditting = isEditting
                        }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: qsTr("of %1 multisig.").arg(newWalletInfo.walletN)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
    Component {
        id: flexible_multisig_absolute
        Rectangle {
            width: parent.width
            height: 184
            color: "#F5F5F5"
            radius: 8
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "Until"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        height: parent.height
                        isEditting: isTimeEditting
                        label: timeEditting()
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isTimeEditting = isEditting
                        }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "spending requires"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        height: parent.height
                        width: paintedWidth
                        text: qsTr("signature%1 from a").arg(newWalletInfo.walletN == 1 ? "" : "s")
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isMNEditting
                        height: parent.height
                        label: newWalletInfo.walletN == 1 ? "singlesig" : qsTr("%1-of-%2").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isMNEditting = isEditting
                        }
                    }
                    QLato {
                        visible: newWalletInfo.walletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                QLato {
                    width: paintedWidth
                    height: 28
                    text: "After that, spending requires either the same"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row{
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: newWalletInfo.walletN == 1 ? qsTr("singlesig OR a") : qsTr("%1-of-%2 multisig OR a").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isNewMNditting
                        height: parent.height
                        label: newWalletInfo.newWalletN == 1 ? "singlesig." : qsTr("%1-of-%2").arg(newWalletInfo.newWalletM).arg(newWalletInfo.newWalletN)
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isNewMNditting = isEditting
                        }
                    }
                    QLato {
                        visible: newWalletInfo.newWalletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
    Component {
        id: flexible_multisig_relative
        Rectangle {
            width: parent.width
            height: 284
            color: "#F5F5F5"
            radius: 8
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "First"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        height: parent.height
                        isEditting: isTimeEditting
                        label: timeEditting()
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isTimeEditting = isEditting
                                         }
                    }
                    QLato {
                        width: paintedWidth
                        height: parent.height
                        text: "after a coin is received:"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                QLato {
                    height: 28
                    width: paintedWidth
                    text: qsTr("spending that coin requires signature%1").arg(newWalletInfo.walletN == 1 ? "" : "s")
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row {
                    height: 28
                    width: parent.width
                    spacing: 4
                    QLato {
                        height: parent.height
                        width: paintedWidth
                        text: "from a"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QContractPolicyEdit{
                        isEditting: isMNEditting
                        height: parent.height
                        label: newWalletInfo.walletN == 1 ? "singlesig" : qsTr("%1-of-%2").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                        onEditingUpdate: (isEditting) => {
                                             clearEditing()
                                             isMNEditting = isEditting
                                         }
                    }
                    QLato {
                        visible: newWalletInfo.walletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: "multisig."
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                QLato {
                    width: paintedWidth
                    height: 28
                    text: "After that, the same coin can be spent with"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: paintedWidth
                    height: 28
                    text: newWalletInfo.walletN == 1 ? qsTr("either the same singlesig OR a ") : qsTr("either the same %1-of‑%2 multisig OR a ").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN)
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Row{
                    height: 28
                    width: parent.width
                    spacing: 4
                    QContractPolicyEdit{
                        isEditting: isNewMNditting
                        height: parent.height
                        label: newWalletInfo.newWalletN == 1 ? "singlesig": qsTr("%1-of-%2").arg(newWalletInfo.newWalletM).arg(newWalletInfo.newWalletN)
                        onEditingUpdate: (isEditting) => {
                            clearEditing()
                            isNewMNditting = isEditting
                        }
                    }
                    QLato {
                        visible: newWalletInfo.newWalletN > 1
                        width: paintedWidth
                        height: parent.height
                        text: qsTr("multisig.")
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
