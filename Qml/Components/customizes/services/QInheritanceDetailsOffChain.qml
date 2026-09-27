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
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

// NUN-10192: "Share your secrets" result dialog (Post 02D-05D). Secret 2 content and the warning
// text now depend on claim_options instead of the old TAPSIGNER-only wording.
Item {
    property var planInfo: inheritancePlanInfo.planInfo
    property string title: STR.STR_QML_887
    // 0 = Direct (Beneficiary), 1 = Indirect (Trustee), 2 = Joint. Selects party wording + the Joint-only note.
    property int shareMode: 0
    signal learnMoreClicked(string kind)

    property var magicPhrases: {
        var bens = planInfo.beneficiaries || [];
        var result = [];
        for (var i = 0; i < bens.length; i++) {
            if (bens[i].magic) result.push({ email: bens[i].email, magic: bens[i].magic });
        }
        return result;
    }

    // BUGFIX (confirmed via inheritanceGetPlan response log): claim_options ships directly on each
    // inheritance_keys[] entry -- no cross-reference into dashboardInfo.keys needed (that lookup
    // always returned [] here, which is why Secret 2 rendered empty).
    // 1 bullet per method per key -- a single key with both methods yields 2 bullets (mockup 04D/04iD/05D).
    property var secret2Bullets: {
        var iKeys = (planInfo && planInfo.inheritance_keys) ? planInfo.inheritance_keys : []
        var bullets = []
        for (var i = 0; i < iKeys.length; i++) {
            var opts = iKeys[i].claim_options !== undefined ? iKeys[i].claim_options : []
            if (opts.indexOf("ENCRYPTED_BACKUP") !== -1) bullets.push({ text: STR.STR_QML_2333, link: "backup" })
            if (opts.indexOf("SEED_PHRASE") !== -1) bullets.push({ text: STR.STR_QML_2332, link: "seed" })
        }
        return bullets
    }
    readonly property bool hasSeed: secret2Bullets.some(function(b) { return b.link === "seed" })
    readonly property bool hasEncrypted: secret2Bullets.some(function(b) { return b.link === "backup" })
    // Illustration side (mockup 02D/02iD/03D/03iD/04D/04iD/05D) mirrors the same seed/encrypted/both split.
    readonly property string illustrationSource: hasSeed && hasEncrypted ? "qrc:/Images/Images/both_backup_secrets.svg"
        : hasEncrypted ? "qrc:/Images/Images/encrypted_backup_secrets.svg" : "qrc:/Images/Images/seed_phrase_secrets.svg"
    readonly property int illustrationWidth: hasSeed && hasEncrypted ? 296 : 228
    readonly property int illustrationHeight: hasSeed && hasEncrypted ? 91 : (hasEncrypted ? 140 : 160)

    function warningText() {
        // Seed-only (no encrypted backup exists anywhere in the plan) is party-agnostic (mockup 02D/02iD).
        if (hasSeed && !hasEncrypted) return STR.STR_QML_2334
        if (shareMode === 0) return (hasEncrypted && !hasSeed) ? STR.STR_QML_890 : STR.STR_QML_2335
        if (shareMode === 1) return (hasEncrypted && !hasSeed) ? STR.STR_QML_893 : STR.STR_QML_2336
        // Joint (shareMode 2): only the "do both" case has a mockup (05D); no evidence for Joint + single method, fall back to the party-agnostic wording.
        return STR.STR_QML_2337
    }
    property string warning: warningText()

    width: 346
    height: 512
    Column {
        width: parent.width
        spacing: 16
        QLato {
            width: parent.width
            text: title
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        Rectangle {
            width: 346
            height: _secret1Content.height + 24
            radius: 12
            color: "#FFFFFF"
            border.color: "#DEDEDE"
            border.width: 1
            Column {
                id: _secret1Content
                anchors { left: parent.left; right: parent.right; top: parent.top; margins: 12 }
                spacing: 12
                Row {
                    id: _secret1Heading
                    width: parent.width
                    spacing: 12
                    QIcon {
                        id: _secret1Icon
                        iconSize: 20
                        source: "qrc:/Images/Images/security-answer-distribution.svg"
                    }
                    QLato {
                        width: _secret1Heading.width - _secret1Icon.width - _secret1Heading.spacing
                        text: STR.STR_QML_2330
                        font.weight: Font.DemiBold
                        horizontalAlignment: Text.AlignLeft
                    }
                }
                // BUGFIX: gray box now spans the full card width (aligned with the icon's left edge), matching mockup -- was indented under the heading text only.
                Rectangle {
                    width: parent.width
                    height: 48
                    color: "#F5F5F5"
                    radius: 12
                    visible: magicPhrases.length === 0
                    QLato {
                        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter; margins: 12 }
                        text: planInfo.magic
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Rectangle {
                    width: parent.width
                    height: 48
                    color: "#F5F5F5"
                    radius: 12
                    visible: magicPhrases.length > 0
                    Flickable {
                        anchors.fill: parent
                        clip: true
                        contentHeight: infoColumn.implicitHeight
                        flickableDirection: Flickable.VerticalFlick
                        ScrollBar.vertical: QScrollBar { }
                        Column {
                            id: infoColumn
                            anchors {
                                fill: parent
                                topMargin: -6
                                leftMargin: 12
                            }
                            Repeater {
                                model: magicPhrases
                                Rectangle {
                                    width: 283
                                    height: 64
                                    color: "transparent"
                                    Column {
                                        anchors {
                                            fill: parent
                                            topMargin: 12
                                            bottomMargin: 12
                                        }
                                        width: parent.width
                                        spacing: 0
                                        QLato {
                                            font.pixelSize: 16
                                            width: parent.width
                                            height: 20
                                            font.weight: Font.DemiBold
                                            text: modelData.email ?? "null null null"
                                        }
                                        QLato {
                                            font.pixelSize: 16
                                            height: 20
                                            width: parent.width
                                            text: modelData.magic ?? "null null null"
                                        }
                                        Item {
                                            width: parent.width
                                            height: 11
                                        }
                                        QLine {
                                            width: parent.width
                                            height: 1
                                            color: "#DEDEDE"
                                            visible: index < magicPhrases.length - 1
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            width: 346
            height: _secret2Content.height + 24
            radius: 12
            color: "#FFFFFF"
            border.color: "#DEDEDE"
            border.width: 1
            Column {
                id: _secret2Content
                anchors { left: parent.left; right: parent.right; top: parent.top; margins: 12 }
                spacing: 8
                Row {
                    id: _secret2Heading
                    width: parent.width
                    spacing: 12
                    QIcon {
                        id: _secret2Icon
                        iconSize: 20
                        source: "qrc:/Images/Images/key-dark.svg"
                    }
                    QLato {
                        width: _secret2Heading.width - _secret2Icon.width - _secret2Heading.spacing
                        text: STR.STR_QML_2331
                        font.weight: Font.DemiBold
                        horizontalAlignment: Text.AlignLeft
                    }
                }
                // BUGFIX: bullets now align with the icon's left edge (parent.width), matching mockup -- was indented an extra 24px under the heading text.
                Repeater {
                    model: secret2Bullets
                    Row {
                        width: parent.width
                        spacing: 8
                        QLato {
                            width: 12
                            text: "•"
                            horizontalAlignment: Text.AlignLeft
                        }
                        QText {
                            width: parent.width - 20
                            textFormat: Text.RichText
                            wrapMode: Text.WordWrap
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 22
                            text: modelData.text + " <a href=\"" + modelData.link + "\">" + STR.STR_QML_2254 + "</a>"
                            onLinkActivated: learnMoreClicked(modelData.link)
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                                acceptedButtons: Qt.NoButton
                            }
                        }
                    }
                }
                QLato {
                    width: parent.width
                    visible: shareMode === 2
                    text: STR.STR_QML_2338
                    color: "#666666"
                    font.pixelSize: 14
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
        Rectangle {
            width: 346
            height: _warningContent.height + 24
            color: "#FDEBD2"
            radius: 8
            Row {
                id: _warningContent
                anchors { left: parent.left; right: parent.right; top: parent.top; margins: 12 }
                spacing: 12
                QIcon {
                    id: _warningIcon
                    iconSize: 24
                    source: "qrc:/Images/Images/warning-dark.svg"
                }
                QLato {
                    width: _warningContent.width - _warningIcon.width - _warningContent.spacing
                    text: warning
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }
}
