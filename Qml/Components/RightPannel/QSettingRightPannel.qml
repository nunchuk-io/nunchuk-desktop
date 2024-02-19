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
import QtQuick 2.12
import "./Setting"

Item {
    readonly property var option_map: [
        {screen:_DISPLAY_UNIT,         screen_component: _display_unit          },
        {screen:_LOCAL_CURRENCY,       screen_component: _local_currency        },
        {screen:_ACCOUNT_SETTINGS,     screen_component: _account_settings      },
        {screen:_NETWORK_SETTINGS,     screen_component: _network_settings      },
        {screen:_HARDWARE_DRIVER,      screen_component: _hardware_driver       },
        {screen:_DATABASE_ENCRYTION,   screen_component: _database_encrytion    },
        {screen:_DEVELOPER_SETTINGS,   screen_component: _developer_settings    },
        {screen:_ABOUT,                screen_component: _about                 },
    ]
    readonly property var anyChanged: _loader.item.anyChanged
    property var itemOption: option_map.find(function(e) {if (e.screen === ProfileSetting.optionIndex) return true; else return false})
    function applySettings() {
        _loader.item.applySettings()
    }
    Loader {
        id: _loader
        anchors.fill: parent
        sourceComponent: itemOption.screen_component
    }
    Component {
        id: _display_unit
        QSettingDisplayUnit {
        }
    }
    Component {
        id: _local_currency
        QSettingLocalCurrency {
        }
    }
    Component {
        id: _account_settings
        QSettingAccountSettings {
        }
    }
    Component {
        id: _network_settings
        QSettingNetworkSettings {
        }
    }
    Component {
        id: _hardware_driver
        QSettingHardwareDriver {
        }
    }
    Component {
        id: _database_encrytion
        QSettingDatabaseEncrytion {
        }
    }
    Component {
        id: _developer_settings
        QSettingDeveloperSettings {
        }
    }
    Component {
        id: _about
        QSettingAbout {
        }
    }
}
