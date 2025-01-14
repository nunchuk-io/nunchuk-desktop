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
import NUNCHUCKTYPE 1.0
import EWARNING 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../OnlineMode/AddHardwareKeys"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property int hardwareType: GroupWallet.qIsByzantine ? GroupWallet.qAddHardware : UserWallet.qAddHardware
    readonly property int _ASK_PASSPHRASE: 1
    readonly property int _IMPORTANT_NOTICE: 2
    readonly property int _BACKUP_PASSPHRASE: 3
    readonly property int _PASSPHRASE_DONE: 4
    property int _passPhrase: _ASK_PASSPHRASE
    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: {
            switch(hardwareType) {
            case NUNCHUCKTYPE.ADD_LEDGER: return _Ledger
            case NUNCHUCKTYPE.ADD_TREZOR: return _Trezor
            case NUNCHUCKTYPE.ADD_COLDCARD: return function() {
                var dashboard = GroupWallet.qIsByzantine ? GroupWallet.dashboardInfo : UserWallet.dashboardInfo
                var is_inheritance = dashboard.isInheritance()
                if (is_inheritance) {
                    switch(_passPhrase) {
                        case _ASK_PASSPHRASE: return _passPhraseSelect
                        case _IMPORTANT_NOTICE: return _importantNotice
                        case _BACKUP_PASSPHRASE: return _passPhraseBackup
                        case _PASSPHRASE_DONE: return _Coldcard
                        default: return null
                    }
                } else {
                    return _Coldcard
                }
            }()
            case NUNCHUCKTYPE.ADD_BITBOX: return _BitBox
            case NUNCHUCKTYPE.ADD_JADE: return _Jade
            default: return null
            }
        }
    }
    Component {
        id: _Ledger
        QScreenAddLedger {}
    }
    Component {
        id: _Trezor
        QScreenAddTrezor {}
    }
    Component {
        id: _Coldcard
        QScreenAddColdcard {}
    }
    Component {
        id: _BitBox
        QScreenAddBitBox {}
    }
    Component {
        id: _Jade
        QScreenAddJade {}
    }
    Component {
        id: _passPhraseSelect
        QSelectPassPhraseQuestion {
            onRequestBack: {
                closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
            onRequestNext: {
                if (option === "not-have-a-passphrase") {
                    _passPhrase = _PASSPHRASE_DONE
                } else {
                    _passPhrase = _IMPORTANT_NOTICE
                }
            }
        }
    }
    Component {
        id: _importantNotice
        QImportantNoticeAboutPassphrase {
            onRequestBack: {
                _passPhrase = _ASK_PASSPHRASE
            }
            onRequestNext: {
                _passPhrase = _BACKUP_PASSPHRASE
            }
            onRequestWithout: {
                var alert = GroupWallet.dashboardInfo.alert
                var can_replace = alert.payload.can_replace
                if (can_replace) {
                    GroupWallet.dashboardInfo.requestShowReplacementKey();
                } else {
                    GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
                }
            }
        }
    }
    Component {
        id: _passPhraseBackup
        QPassphraseBackupReminder {
            onRequestBack: {
                _passPhrase = _IMPORTANT_NOTICE
            }
            onRequestNext: {
                _passPhrase = _PASSPHRASE_DONE
            }
        }
    }

    function doneOrTryAgainAddHardwareKey(isSuccess) {
        if (isSuccess) {
            AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
            closeTo(NUNCHUCKTYPE.WALLET_TAB)
        } else {
            GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
        }
    }
}
