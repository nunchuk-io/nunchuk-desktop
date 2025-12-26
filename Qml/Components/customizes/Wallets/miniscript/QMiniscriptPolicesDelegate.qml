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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    id: miniscriptContainer
    height: getHeight()

    property bool   useDerivationPath: true
    property var    miniscript
    property var    levels: miniscript.levels
    property int    level: levels.length

    enabled: miniscript.enable
    Row {
        id: miniscriptRow
        anchors.fill: parent
        spacing: 0
        Item {
            id: spacer
            width: 20 * level
            height: parent.height
            Row {
                anchors.fill: parent
                spacing: 0
                Repeater {
                    model: levels
                    delegate: Item {
                        width: 20
                        height: parent.height
                        Rectangle {
                            width: 1
                            height: index == (level - 1) && miniscript.isLast ? 8 : parent.height
                            color: "#757575"
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: modelData != 0
                        }
                        QIcon {
                            id: icon
                            visible: index == (level - 1)
                            anchors {
                                top: parent.top
                                topMargin: 8
                            }
                            iconSize: 20
                            source: "qrc:/Images/Images/tree-atom.svg"
                        }
                    }
                }
            }            
        }
        Loader {
            id: templateLoader
            width: parent.width - spacer.width
            height: parent.height
            opacity: miniscript.enable ? 1.0 : 0.4
            sourceComponent: switch(miniscript.type) {
                case ScriptNodeHelper.Type.PK: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.OLDER:
                case ScriptNodeHelper.Type.AFTER: return getAfterOlder();
                case ScriptNodeHelper.Type.HASH160: return getCommon();
                case ScriptNodeHelper.Type.HASH256: return getCommon();
                case ScriptNodeHelper.Type.RIPEMD160: return getCommon();
                case ScriptNodeHelper.Type.SHA256: return getCommon();
                case ScriptNodeHelper.Type.AND: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.OR: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.ANDOR: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.THRESH: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.MULTI: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.MUSIG: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.OR_TAPROOT: return miniscript.key ? keyCoponent : getCommon();
                case ScriptNodeHelper.Type.NONE: return getCommon();
                default: return getCommon();
            } 
        }
    }
    function getKeyHeight() {
        if(miniscript.keyObj !== null && miniscript.keyObj !== undefined) {
            if (miniscript.keyObj.singleSigner_derivationPath !== undefined && miniscript.keyObj.singleSigner_derivationPath !== null && useDerivationPath) {
                return miniscript.keyObj.single_signer_type !== NUNCHUCKTYPE.SERVER ? 76 : 38
            } else {
                return miniscript.keyObj.single_signer_type !== NUNCHUCKTYPE.SERVER ? 58 : 38
            }            
        } else {
            return 48
        }
    }
    function getCommonHeight() {
        var tmpHeight = 46
        if (miniscript.coinsGroup !== undefined && miniscript.coinsGroup !== null) {
            var heightX = ((miniscript.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE) && (miniscript.coinsGroup.signXCount > 0)) ? 24 : 0
            var heightActive = miniscript.coinsGroup.hasActiveAfter ? 24 : 0
            return tmpHeight + heightX + heightActive + (heightX > 0 && heightActive > 0 ? 4 : 0)
        } else {
            return tmpHeight
        }
    }
    function getHeight() {
        var tmpHeight = 0
        switch(miniscript.type) {
            case ScriptNodeHelper.Type.PK: tmpHeight =  miniscript.key ? getKeyHeight() : 38 + 4; break;
            case ScriptNodeHelper.Type.OLDER: tmpHeight = getAfterOlderHeight(); break;
            case ScriptNodeHelper.Type.AFTER: tmpHeight = getAfterOlderHeight(); break;
            case ScriptNodeHelper.Type.HASH160: tmpHeight = 58; break;
            case ScriptNodeHelper.Type.HASH256: tmpHeight = 74; break;
            case ScriptNodeHelper.Type.RIPEMD160: tmpHeight = 58; break;
            case ScriptNodeHelper.Type.SHA256: tmpHeight = 74; break;
            case ScriptNodeHelper.Type.AND: tmpHeight =  miniscript.key ? getKeyHeight() : getCommonHeight(); break;
            case ScriptNodeHelper.Type.OR: tmpHeight =  miniscript.key ? getKeyHeight() : getCommonHeight(); break;
            case ScriptNodeHelper.Type.ANDOR: tmpHeight =  miniscript.key ? getKeyHeight() : 72; break;
            case ScriptNodeHelper.Type.THRESH: tmpHeight =  miniscript.key ? getKeyHeight() : getCommonHeight(); break;
            case ScriptNodeHelper.Type.MULTI: tmpHeight =  miniscript.key ? getKeyHeight() : getCommonHeight(); break;
            case ScriptNodeHelper.Type.MUSIG: tmpHeight =  miniscript.key ? getKeyHeight() : getCommonHeight(); break;
            case ScriptNodeHelper.Type.OR_TAPROOT: tmpHeight =  miniscript.key ? getKeyHeight() : getCommonHeight(); break;
            case ScriptNodeHelper.Type.NONE:
            default: tmpHeight = 20; break;
        }
        return tmpHeight
    }
    function getTitle() {
        switch(miniscript.type) {
            case ScriptNodeHelper.Type.PK: return ""
            case ScriptNodeHelper.Type.OLDER: {
                if (miniscript.lockType === ScriptNodeHelper.TimelockBased.TIME_LOCK) {
                    return qsTr("After %1").arg(miniscript.relativeTimestamp.valueDisplay)
                } else if (miniscript.lockType === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK) {
                    return qsTr("After %1 blocks").arg(miniscript.relativeBlockheight)
                }
            }
            case ScriptNodeHelper.Type.AFTER: {
                if (miniscript.lockType === ScriptNodeHelper.TimelockBased.TIME_LOCK) {
                    return qsTr("After %1").arg(miniscript.absoluteTimestamp.valueDisplay)
                } else if (miniscript.lockType === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK) {
                    return qsTr("After block %1").arg(miniscript.absoluteBlockheight)
                }
            }
            case ScriptNodeHelper.Type.HASH160: return "Hash160"
            case ScriptNodeHelper.Type.HASH256: return "Sha256"
            case ScriptNodeHelper.Type.RIPEMD160: return "Ripemd160"
            case ScriptNodeHelper.Type.SHA256: return "Sha256"
            case ScriptNodeHelper.Type.AND: return "AND"
            case ScriptNodeHelper.Type.OR: return "OR"
            case ScriptNodeHelper.Type.ANDOR: return "AND OR"
            case ScriptNodeHelper.Type.THRESH: return qsTr("Thresh %1/%2").arg(miniscript.m).arg(miniscript.n)
            case ScriptNodeHelper.Type.MULTI: return qsTr("Multisig %1/%2").arg(miniscript.m).arg(miniscript.n)
            case ScriptNodeHelper.Type.MUSIG: return qsTr("MuSig")
            case ScriptNodeHelper.Type.OR_TAPROOT: return "OR"
            case ScriptNodeHelper.Type.NONE: 
            default: return ""
        }
    }
    function getDescription() {
        switch(miniscript.type) {
            case ScriptNodeHelper.Type.PK: return ""
            case ScriptNodeHelper.Type.OLDER: return STR.STR_QML_1853
            case ScriptNodeHelper.Type.AFTER: {
                if (miniscript.lockType === ScriptNodeHelper.TimelockBased.TIME_LOCK) {
                    if(miniscript.absoluteTimestamp.valueRemaining === "" || miniscript.absoluteTimestamp.valueRemaining === "expired") {
                        return ""
                    }
                    else {
                        return STR.STR_QML_1855.arg(miniscript.absoluteTimestamp.valueRemaining).arg(miniscript.absoluteTimestamp.valueFrom)
                    }
                }
                else if (miniscript.lockType === ScriptNodeHelper.TimelockBased.HEIGHT_LOCK) {
                    var blockRemaining = Math.max(0, parseInt(miniscript.absoluteBlockheight) - AppModel.blockHeight)
                    if(blockRemaining === 0) {
                        return ""
                    }
                    else{
                        return STR.STR_QML_1856.arg(blockRemaining).arg(blockRemaining > 1 ? "s" : "")
                    }
                }
                return "";
            }
            case ScriptNodeHelper.Type.HASH160: 
            case ScriptNodeHelper.Type.HASH256: 
            case ScriptNodeHelper.Type.RIPEMD160:
            case ScriptNodeHelper.Type.SHA256: return miniscript.hashData
            case ScriptNodeHelper.Type.AND: return STR.STR_QML_1851
            case ScriptNodeHelper.Type.OR: return STR.STR_QML_1852
            case ScriptNodeHelper.Type.ANDOR: return STR.STR_QML_1850
            case ScriptNodeHelper.Type.THRESH: return STR.STR_QML_1848
            case ScriptNodeHelper.Type.MULTI: return STR.STR_QML_1849
            case ScriptNodeHelper.Type.MUSIG: return STR.STR_QML_1876
            case ScriptNodeHelper.Type.OR_TAPROOT: return STR.STR_QML_1854
            case ScriptNodeHelper.Type.NONE: 
            default: return ""
        }
    }
    function getCommon() {
        return common
    }
    function getAfterOlder() {
        return after_older
    }
    function getAfterOlderHeight() {
        return 58
    }
    Component {
        id: common
        QCommonComponent {
        }
    }
    Component {
        id: after_older
        QAfterOlderComponent {

        }
    }
}
