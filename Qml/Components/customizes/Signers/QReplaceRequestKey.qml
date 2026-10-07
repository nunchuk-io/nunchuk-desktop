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
import "../../../../localization/STR_QML.js" as STR

Item {
    width: 322
    height: childrenRect.height
    // BUGFIX: same as QAddRequestKey.qml - the Setup 20D-20gD caption line(s) (File/Seed or "Sharing
    // method not set") need extra row height, or they overflow into the row below.
    readonly property int captionLines: {
        // BUGFIX: these captions are off-chain (MULTI_SIG, NUN-10192) only - never show them for an
        // on-chain MINISCRIPT key even if is_inheritance/claim_options data exists for it.
        if (!modelData.is_inheritance || !modelData.has || modelData.wallet_type !== "MULTI_SIG") return 0
        if (inheritanceRowState() === "SET_UP") return 1
        var n = 0
        if (hasClaimOption("ENCRYPTED_BACKUP")) n++
        if (hasClaimOption("SEED_PHRASE")) n++
        return n
    }
    signal inheritanceKeyClicked()
    signal serkeyClicked()
    signal hardwareClicked()
    signal backupClicked()

    property var  key_index: modelData.wallet_type === "MULTI_SIG" ? (modelData.key_index + 1) : modelData.key_index
    property var  walletType: modelData.wallet_type
    property int  slot_index: index + 1
    property bool isBeforeSlot: modelData.has && !modelData.hasSecond

    function getComponent() {
        if(modelData.is_inheritance) {
            return modelData.has ? inheritanceAdded : inheritanceEmpty
        }
        else if (modelData.type === "SERVER") {
            return modelData.has ? serverAdded : serverEmpty
        }
        else {
            return modelData.has ? hardwareAdded : hardwareEmpty
        }
    }

    function inheritance(replace, add, backup, added) {
        if(modelData.replacements.length === 0) {
            return replace
        }
        else if(modelData.replacements.length === 1) {
            return add
        }
        else {
            if (modelData.wallet_type === "MULTI_SIG") {
                return modelData.user_key === null ? backup : added
            }
            var needVerifyBackup = modelData.verification_type === "NONE" ? backup : added
            if (isBeforeSlot) {
                return modelData.hasSecond ? needVerifyBackup : add
            }
            return needVerifyBackup
        }
    }

    function normal(replace, add, added) {
        if(modelData.replacements.length === 0) {
            return replace
        }
        else if(modelData.replacements.length === 1) {
            return add
        }
        else {
            if (modelData.wallet_type === "MULTI_SIG") {
                return added
            }
            if (isBeforeSlot) {
                return modelData.hasSecond ? added : add
            }
            return added
        }
    }

    function getModelData(data, replacements) {
        if(modelData.replacements.length === 0){
            return data
        }
        else {
            return replacements[0]
        }
    }

    Column {
        spacing: 4
        Loader {
            id: _source
            width: 346
            height: 72 + captionLines * 16
            sourceComponent: getComponent()
        }
        Row {
            spacing: 4
            visible: modelData.replacements.length > 0
            QSvgImage {
                width: 16
                height: 16
                source: "qrc:/Images/Images/replace.svg"
            }
            QLato {
                text: STR.STR_QML_2052.arg(modelData.old_singer.name).arg(modelData.old_singer.xfp.toUpperCase())
                font.pixelSize: 12
                color: "#031F2B"
            }
        }
    }

    FastBlur {
        anchors.fill: _source
        source: _source
        radius: 32
        visible: modelData.type === "SERVER" ? isKeyHolderLimited : (isKeyHolderLimited && modelData.has && !modelData.ourAccount)
    }
    Component {
        id: replaceButton
        QTextButton {
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_1368
            label.font.pixelSize: 16
            onButtonClicked: {
                if (modelData.is_inheritance) {
                    inheritanceKeyClicked()
                } else {
                    hardwareClicked()
                }
            }
        }
    }
    Component {
        id: backupButton
        QTextButton {
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: modelData.wallet_type === "MULTI_SIG" ? STR.STR_QML_342 : STR.STR_QML_1964
            label.font.pixelSize: 16
            onButtonClicked: {
                backupClicked()
            }
        }
    }
    Component {
        id: addButton
        QTextButton {
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_941
            label.font.pixelSize: 16
            onButtonClicked: {
                if (modelData.is_inheritance) {
                    inheritanceKeyClicked()
                } else {
                    hardwareClicked()
                }
            }
        }
    }
    Component {
        id: addedCheck
        QBadge {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 12
            }
            width: 75
            height: 24
            iconSize: 24
            icon: "qrc:/Images/Images/check-circle-dark.svg"
            text: STR.STR_QML_104
            color: "#A7F0BA"
        }
    }
    // Setup 20bD-20eD/20dD (NUN-10192): mirrors QAddRequestKey.qml's claim_options/verifications model.
    Component {
        id: verifyBackupButton
        QTextButton {
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_2309 // "Verify backup"
            label.font.pixelSize: 16
            onButtonClicked: backupClicked()
        }
    }
    Component {
        id: setUpButton
        QTextButton {
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_2310 // "Set up"
            label.font.pixelSize: 16
            onButtonClicked: backupClicked()
        }
    }
    // claim_options/verifications (NUN-10192), applies once replacements.length >= 2 and MULTI_SIG.
    // BUGFIX: guard here (the single source feeding hasClaimOption()/captionLines/inheritanceRowState())
    // so no caller can leak off-chain claim_options onto an on-chain MINISCRIPT key.
    function claimOptions() {
        if (modelData.wallet_type !== "MULTI_SIG") return []
        return modelData.claim_options !== undefined ? modelData.claim_options : []
    }
    function hasClaimOption(method) {
        return claimOptions().indexOf(method) !== -1
    }
    function verificationFor(method) {
        var list = modelData.verifications !== undefined ? modelData.verifications : []
        for (var i = 0; i < list.length; i++) {
            if (list[i].verification_method === method) return list[i]
        }
        return null
    }
    function fileStatusText() {
        var v = verificationFor("ENCRYPTED_BACKUP")
        if (!v || v.verification_type === "NONE") return STR.STR_QML_2288 // Not uploaded
        if (v.verification_type === "SKIPPED_VERIFICATION") return STR.STR_QML_2289 // Skipped
        return STR.STR_QML_2279 // Verified
    }
    function seedStatusText() {
        var v = verificationFor("SEED_PHRASE")
        if (!v || v.verification_type === "NONE") return STR.STR_QML_2287 // Pending
        if (v.verification_type === "SKIPPED_VERIFICATION") return STR.STR_QML_2289 // Skipped
        return STR.STR_QML_2279 // Verified
    }
    function methodState(method) {
        var v = verificationFor(method)
        if (method === "ENCRYPTED_BACKUP" && !v) return "NOT_UPLOADED"
        if (!v || v.verification_type === "NONE") return "PENDING"
        if (v.verification_type === "SKIPPED_VERIFICATION") return "SKIPPED"
        return "VERIFIED"
    }
    // Single source of truth for row state; adds "REPLACE"/"ADD_NEW" tiers before the claim_options tiers.
    function inheritanceRowState() {
        if (modelData.wallet_type !== "MULTI_SIG") {
            // Outside NUN-10192 scope - unchanged legacy behavior.
            return inheritance("REPLACE", "ADD_NEW", "BACKUP", "ADDED")
        }
        if (modelData.replacements.length === 0) return "REPLACE"
        if (modelData.replacements.length === 1) return "ADD_NEW"
        var opts = claimOptions()
        if (opts.length === 0) {
            // Setup 20dD: "[] means legacy/not configured" - "Set up opens the sharing-method selection".
            return "SET_UP"
        }
        if (hasClaimOption("ENCRYPTED_BACKUP") && methodState("ENCRYPTED_BACKUP") === "NOT_UPLOADED") {
            return "BACKUP" // Setup 20fD
        }
        var allVerified = opts.every(function(m) { return methodState(m) === "VERIFIED" })
        return allVerified ? "ADDED" : "VERIFY_BACKUP" // Setup 20D (Added) vs 20bD/20cD/20eD (Verify backup)
    }
    function inheritanceActionComponent() {
        switch (inheritanceRowState()) {
        case "REPLACE": return replaceButton
        case "ADD_NEW": return addButton
        case "SET_UP": return setUpButton
        case "BACKUP": return backupButton
        case "VERIFY_BACKUP": return verifyBackupButton
        default: return addedCheck
        }
    }
    Component {
        id: inheritanceEmpty
        QDashRectangle {
            anchors.fill: parent
            radius: 8
            borderWitdh: 2
            borderColor: "#031F2B"

            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 36
                    height: 36
                    iconSize: 18
                    icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    height: childrenRect.height
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: 150
                        height: 28
                        text: STR.STR_QML_954.arg(slot_index)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QAccountIndexs {
                        height: 16
                        visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER  && modelData.signer_type !== NUNCHUCKTYPE.PLATFORM
                        accountIndexs: modelData.account_indexs
                        walletType: modelData.wallet_type
                    }
                }
            }
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: addButton
            }
            // Setup 01aD: "Inheritance" corner ribbon, per Figma asset Inheritance_badge.svg (81x14 Hug).
            // Shared design for on-chain and off-chain inheritance keys alike - no wallet_type gate.
            QImage {
                anchors {
                    top: parent.top
                    right: parent.right
                    topMargin: 0
                    rightMargin: 1
                }
                width: 81
                height: 14
                source: "qrc:/Images/Images/Inheritance_badge.svg"
            }
        }
    }
    Component {
        id: inheritanceAdded
        QDashRectangle {
            anchors.fill: parent
            // BUGFIX: color/button both now use inheritanceRowState(), matching QAddRequestKey.qml.
            color: {
                switch (inheritanceRowState()) {
                case "REPLACE": return "#FFFFFF"
                case "ADD_NEW": return "#66A7F0BA"
                case "SET_UP": return "#FDEBD2"
                case "BACKUP": return "#FDEBD2"
                case "VERIFY_BACKUP": return "#FDEBD2"
                default: return "#A7F0BA"
                }
            }
            isDashed: inheritanceRowState() === "ADD_NEW"
            radius: 8
            borderWitdh: isDashed ? 2 : 1
            borderColor: isDashed ? "#031F2B" : "#DEDEDE"
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QCircleIcon {
                    bgSize: 36
                    icon.iconSize: 24
                    icon.typeStr: modelData.type
                    icon.tag: modelData.tag
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    height: childrenRect.height
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: parent.width
                        text: modelData.name
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        spacing: 4
                        Rectangle {
                            width: signerTypeText.width + 8
                            height: 16
                            color: "#EAEAEA"
                            radius: 20
                            visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER  && modelData.signer_type !== NUNCHUCKTYPE.PLATFORM
                            QText {
                                id: signerTypeText
                                text: GlobalData.signers(modelData.signer_type)
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                        
                        QAccountIndexs {
                            height: 16
                            visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER && modelData.signer_type !== NUNCHUCKTYPE.PLATFORM
                            accountIndexs: modelData.account_indexs
                            walletType: modelData.wallet_type
                        }
                    }
                    Item {
                        width: parent.width
                        height: 16
                        QLato {
                            visible: modelData.card_id !== ""
                            width: parent.width
                            text: {
                                var card_id_text = modelData.card_id
                                var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                return "Card ID: ••" + textR
                            }
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.capitalization: Font.AllUppercase
                            font.pixelSize: 12
                        }
                        QLato {
                            visible: modelData.card_id === ""
                            width: parent.width
                            text: "XFP: " + modelData.xfp
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.capitalization: Font.AllUppercase
                            font.pixelSize: 12
                        }
                    }
                    // Setup 20D-20gD: File/Seed captions, matching QAddRequestKey.qml.
                    Column {
                        width: parent.width
                        spacing: 2
                        // BUGFIX: is_inheritance alone is wallet-type-agnostic (also true for on-chain
                        // timelock inheritance keys) - gate the whole off-chain caption block by MULTI_SIG too.
                        visible: modelData.is_inheritance && modelData.wallet_type === "MULTI_SIG"
                        // Setup 20dD: "Sharing method not set" caption for the SET_UP state.
                        QLato {
                            width: parent.width
                            visible: inheritanceRowState() === "SET_UP"
                            text: STR.STR_QML_2311
                            font.pixelSize: 11
                            color: "#5B6268"
                            horizontalAlignment: Text.AlignLeft
                        }
                        QLato {
                            width: parent.width
                            visible: hasClaimOption("ENCRYPTED_BACKUP")
                            text: STR.STR_QML_2307.arg(fileStatusText())
                            font.pixelSize: 11
                            color: "#5B6268"
                            horizontalAlignment: Text.AlignLeft
                        }
                        QLato {
                            width: parent.width
                            visible: hasClaimOption("SEED_PHRASE")
                            text: STR.STR_QML_2308.arg(seedStatusText())
                            font.pixelSize: 11
                            color: "#5B6268"
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                }
            }
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: inheritanceActionComponent()
            }
        }
    }
    Component {
        id: hardwareEmpty
        QDashRectangle {
            anchors.fill: parent
            radius: 8
            borderWitdh: 2
            borderColor: "#031F2B"
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 36
                    height: 36
                    iconSize: 24
                    icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: 150
                        text: STR.STR_QML_954.arg(slot_index)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QAccountIndexs {
                        height: 16
                        visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER && modelData.signer_type !== NUNCHUCKTYPE.PLATFORM
                        accountIndexs: modelData.account_indexs
                        walletType: modelData.wallet_type
                    }
                }
            }
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: addButton
            }
        }
    }
    Component {
        id: hardwareAdded
        QDashRectangle {
            anchors.fill: parent
            color: normal("#FFFFFF", "#66A7F0BA", "#A7F0BA")
            isDashed: normal(false, true, false)
            radius: 8
            borderWitdh: isDashed ? 2 : 1
            borderColor: isDashed ? "#031F2B" : "#DEDEDE"
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QCircleIcon {
                    bgSize: 36
                    icon.iconSize: 18
                    icon.typeStr: modelData.type
                    icon.tag: modelData.tag
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    height: childrenRect.height
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: parent.width
                        text: modelData.name
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        spacing: 4
                        Rectangle {
                            width: signerTypeText.width + 8
                            height: 16
                            color: "#EAEAEA"
                            radius: 20
                            visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER && modelData.signer_type !== NUNCHUCKTYPE.PLATFORM
                            QText {
                                id: signerTypeText
                                text: GlobalData.signers(modelData.signer_type)
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                        QAccountIndexs {
                            height: 16
                            visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER && modelData.signer_type !== NUNCHUCKTYPE.PLATFORM
                            accountIndexs: modelData.account_indexs
                            walletType: modelData.wallet_type
                        }
                    }
                    QLato {
                        width: parent.width
                        text: "XFP: " + modelData.xfp
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        font.pixelSize: 12
                    }
                }
            }
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: normal(replaceButton, addButton, addedCheck)
            }
        }
    }
    Component {
        id: serverEmpty
        QDashRectangle {
            anchors.fill: parent
            radius: 8
            borderWitdh: 2
            borderColor: "#031F2B"
            enabled: !isKeyHolderLimited
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 36
                    height: 36
                    iconSize: 24
                    icon: "qrc:/Images/Images/Device_Icons/server-key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                QLato {
                    width: 150
                    text: STR.STR_QML_957
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            QTextButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*20
                height: 36
                type: eTypeB
                label.text: STR.STR_QML_958
                label.font.pixelSize: 16
                onButtonClicked: {
                    serkeyClicked()
                }
            }
        }
    }
    Component {
        id: serverAdded
        QDashRectangle {
            anchors.fill: parent
            radius: 8
            borderWitdh: 1
            borderColor: "#DEDEDE"
            enabled: !isKeyHolderLimited
            isDashed: false
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 36
                    height: 36
                    iconSize: 24
                    icon: "qrc:/Images/Images/Device_Icons/server-key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                QLato {
                    width: 150
                    text: modelData.name
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 16
                }
            }
        }
    }
}
