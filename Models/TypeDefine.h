#pragma once
#include "QOutlog.h"
class QGroupDashboard;
typedef OurSharedPointer<QGroupDashboard> QGroupDashboardPtr;
class QGroupWalletDummyTx;
typedef OurSharedPointer<QGroupWalletDummyTx> QGroupWalletDummyTxPtr;
class QGroupWalletHealthCheck;
typedef OurSharedPointer<QGroupWalletHealthCheck> QGroupWalletHealthCheckPtr;
class Wallet;
typedef OurSharedPointer<Wallet> QWalletPtr;
class QWalletServicesTag;
typedef OurSharedPointer<QWalletServicesTag> QWalletServicesTagPtr;
class QServerKey;
typedef OurSharedPointer<QServerKey> QServerKeyPtr;
class QInheritancePlan;
typedef OurSharedPointer<QInheritancePlan> QInheritancePlanPtr;
class QKeyRecovery;
typedef OurSharedPointer<QKeyRecovery> QKeyRecoveryPtr;
typedef const QString WalletId;
typedef const QString GroupId;
typedef QStringList WalletIdList;
typedef QStringList GroupIdList;
typedef QMap<QString, QString> WalletGroupIdMap;
class QWalletDummyTx;
typedef OurSharedPointer<QWalletDummyTx> QWalletDummyTxPtr;
class QUserWalletDummyTx;
typedef OurSharedPointer<QUserWalletDummyTx> QUserWalletDummyTxPtr;
class QRecurringPayment;
typedef OurSharedPointer<QRecurringPayment> QRecurringPaymentPtr;
