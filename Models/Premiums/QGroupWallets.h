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

#ifndef QGROUPWALLETS_H
#define QGROUPWALLETS_H

#include <QObject>
#include <QtConcurrent>
#include <QVariantList>
#include "QCommonDefines.h"
#include "Servers/QRest.h"
#include "Servers/DracoDefines.h"
#include "Servers/Byzantine.h"
#include "Premiums/QAssistedDraftWallets.h"
#include "Premiums/QGroupDashboard.h"

class QGroupWallets : public QAssistedDraftWallets
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*>  dashboards          READ dashboards                             NOTIFY dashboardListChanged)
    Q_PROPERTY(QGroupDashboard* dashboardInfo       READ dashboardInfo                          NOTIFY dashboardInfoChanged)
    Q_PROPERTY(int currentIndex                     READ currentIndex  WRITE setCurrentIndex    NOTIFY currentIndexChanged)
    Q_PROPERTY(QVariant highestPermissionAccount    READ highestPermissionAccount               NOTIFY highestPermissionAccountChanged)
private:
    QGroupWallets();
    ~QGroupWallets();
public:
    static QGroupWallets *instance();
    QGroupWallets(QGroupWallets &other) = delete;
    QGroupWallets(QGroupWallets const &other) = delete;
    void operator=(const QGroupWallets &other) = delete;

    QGroupDashboard* dashboardInfo();
    QGroupDashboardPtr dashboardInfoPtr();
    QList<QObject *> dashboards() const;

    void GetAllGroups();
    bool AcceptGroupWallet();
    void DenyGroupWallet();
    void ResetGroupWallet();

    int currentIndex() const;
    void setCurrentIndex(int currentIndex);

    void reset();
    bool Contains(const QString& group_id);
    QGroupDashboardPtr GetDashboard(const QString& group_id);

    void findPermissionAccount();
    void setHighestPermissionAccount(const QJsonObject &highestPermissionAccount);
    QVariant highestPermissionAccount();

    bool existGroupPending() const;
public:
    bool AddOrUpdateAKeyToDraftWallet() final;
    void updateRequestKey() final;

public slots:
    void MakePendingDashboardList(const QJsonArray& groups);
    void dashboard(const QString& group_id, const QString& wallet_id);
    void accept(const QString& group_id);
    void deny(const QString& group_id);
    void reset(const QString& group_id);
    void markRead(const QString& alert_id);
    void dismiss(const QString& alert_id);
    void refresh();
    void setDashboardInfo(const QString& group_id, const QString &wallet_id = "");
    void setDashboardInfo(const QWalletPtr &wallet);
    void slotAcceptChanged(const QString& group_id);
    void clearDashBoard();
signals:
    void dashboardList(const QJsonArray& array);
    void dashboardListChanged();
    void dashboardInfoChanged();
    void currentIndexChanged();
    void showDashBoardChanged();
    void highestPermissionAccountChanged();
    void acceptChanged(const QString& group_id);
private:
    QList<QGroupDashboardPtr>   mPendingWallets {};
    QGroupDashboardPtr          mDashboard {};
    int                         mCurrentIndex {-1};
    QJsonObject                 mHighestPermissionAccount;
};

#endif // QGROUPWALLETS_H
