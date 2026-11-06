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
import "../../../../localization/STR_QML.js" as STR

QAddBlockstreamJadeViaQR {
    readonly property var configurableSigner: SignerManagement.currentSigner
    readonly property bool hasClaimAccount: configurableSigner.claim_account !== undefined
    readonly property bool isSingleAccountCard: configurableSigner.single_account === true
    readonly property bool isFallbackCard: isSingleAccountCard && configurableSigner.section === "FALLBACK"
    readonly property bool hasPresetAccount: isSingleAccountCard && configurableSigner.preset_account !== undefined
    readonly property int reusedAccount: hasPresetAccount ? configurableSigner.preset_account : (isFallbackCard ? configurableSigner.fallback_index + 2 : 0)
    label.text: isSingleAccountCard ? QSTR.STR_QML_2067 : (isFlowClamOrAddKeyClaim() ? QSTR.STR_QML_2067 : QSTR.STR_QML_1925.arg(1))
    guideDescriptionOne: isSingleAccountCard ? "" : (isFlowClamOrAddKeyClaim() ? QSTR.STR_QML_817 : QSTR.STR_QML_1926)
    guideDescriptionTwo: isFallbackCard ? (hasPresetAccount ? STR.STR_QML_2444.arg(configurableSigner.fallback_index + 1).arg(reusedAccount) : STR.STR_QML_2379.arg(reusedAccount)) : (isSingleAccountCard ? STR.STR_QML_2378 : (isFlowClamOrAddKeyClaim() ? "" : QSTR.STR_QML_1927))
    guideExportString: hasClaimAccount ? QSTR.STR_QML_2044.arg(configurableSigner.claim_account) : (isFallbackCard ? (hasPresetAccount ? QSTR.STR_QML_2044.arg(reusedAccount) : QSTR.STR_QML_2044.arg(STR.STR_QML_2380.arg(reusedAccount))) : QSTR.STR_QML_2044.arg("0"))
}
