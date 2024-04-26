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
import "./Service/HoneyBadger" as HB
import "./Service/IronHand" as IH
import "./Service/Byzantine" as B
import "./Service/Common"

Item {
    property var highestPermissionAccount: GroupWallet.highestPermissionAccount
    property bool hasGroupWallet: highestPermissionAccount.hasGroupWallet
    property bool hasGroupPending: highestPermissionAccount.hasGroupPending
    property bool isObserver: highestPermissionAccount.role === "OBSERVER"
    property bool hasWalletLockdown: ServiceSetting.servicesTag.listLockdown.length > 0
    Loader {
        anchors.fill: parent
        sourceComponent: {
            var user = ClientController.user
            if (user.isHoneyBadgerUser) { return hb_subscriber }
            else if (user.isIronHandUser) { return ih_subscriber }
            else if (user.isByzantineUser || user.isFinneyUser) {
                if (hasGroupWallet) { return isObserver ? b_observer_subscriber : b_subscriber }
                else { return b_empty_state }
            }
            else {
                if (!hasGroupWallet && hasGroupPending) { return b_empty_state }
                if (hasGroupWallet) { return isObserver ? b_observer_subscriber : b_subscriber }
                else { return notSubscriber }
            }
        }
    }
    Component {
        id: notSubscriber
        QServiceNotSubscriberLeftPannel { }
    }
    Component {
        id: hb_subscriber
        HB.QServiceSubscriberLeftPannel { }
    }
    Component {
        id: ih_subscriber
        IH.QServiceSubscriberLeftPannel { }
    }
    Component {
        id: b_subscriber
        B.QServiceSubscriberLeftPannel { }
    }
    Component {
        id: b_observer_subscriber
        B.QServiceObserverIterationLeftPannel { }
    }
    Component {
        id: b_empty_state
        B.QServiceEmptyStateLeftPannel { }
    }
}
