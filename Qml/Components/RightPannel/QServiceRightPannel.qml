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
import "./Service/HoneyBadger-IronHand" as HBIH
import "./Service/Byzantine" as B
import "./Service/Common"

Item {
    property var  highestPermissionAccount: GroupWallet.highestPermissionAccount
    property bool hasGroupWallet: highestPermissionAccount.hasGroupWallet
    property bool hasGroupPending: highestPermissionAccount.hasGroupPending
    property bool isObserver: highestPermissionAccount.role === "OBSERVER"
    Loader {
        anchors.fill: parent
        sourceComponent: {
            var user = ClientController.user
            if (user.isIronHandUser) {
                return hb_ih_subscriber
            }
            else if (user.isHoneyBadgerUser) {
                return hb_ih_subscriber
            }
            else if (user.isByzantineUser) {
                if (hasGroupWallet) {
                    return isObserver ? observer_subscriber() : b_subscriber
                }
                else {
                    return observer_subscriber()
                }
            }
            else {
                if (!hasGroupWallet && hasGroupPending) {
                    return observer_subscriber()
                }
                if (hasGroupWallet) {
                    return isObserver ? observer_subscriber() : b_subscriber
                }
                else {
                    return not_subscriber()
                }
            }
        }
    }

    function observer_subscriber()
    {
        if (ServiceSetting.optionIndex === _CLAIM_AN_INHERITANCE) {
            return _claim_an_inheritance
        }
        return b_observer_subscriber
    }

    function not_subscriber()
    {
        if (ServiceSetting.optionIndex === _CLAIM_AN_INHERITANCE) {
            return _claim_an_inheritance
        }
        return notSubscriber
    }

    Component {
        id: _claim_an_inheritance
        QServiceClaimAnInheritance {
        }
    }

    Component {
        id: notSubscriber
        QServiceNotSubscriberRightPannel {
        }
    }
    Component {
        id: hb_ih_subscriber
        HBIH.QServiceSubscriberRightPannel {
        }
    }
    Component {
        id: b_subscriber
        B.QServiceSubscriberRightPannel {
        }
    }
    Component {
        id: b_observer_subscriber
        B.QServiceObserverIterationRightPannel {
        }
    }
}
