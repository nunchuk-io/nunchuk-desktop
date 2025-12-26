#include "Slugs.h"
#include "QOutlog.h"
#include <QMutexLocker>

namespace {
    inline constexpr auto BYZANTINE = "byzantine";
    inline constexpr auto BYZANTINE_TESTNET = "byzantine_testnet";
    inline constexpr auto HONEY_BADGER = "honey_badger";
    inline constexpr auto HONEY_BADGER_TESTNET = "honey_badger_testnet";
    inline constexpr auto HONEY_BADGER_PLUS = "honey_badger_plus";
    inline constexpr auto HONEY_BADGER_PLUS_TESTNET = "honey_badger_plus_testnet";
    inline constexpr auto HONEY_BADGER_PREMIER = "honey_badger_premier";
    inline constexpr auto HONEY_BADGER_PREMIER_TESTNET = "honey_badger_premier_testnet";
    inline constexpr auto IRON_HAND = "iron_hand";
    inline constexpr auto IRON_HAND_TESTNET = "iron_hand_testnet";
    inline constexpr auto FINNEY = "finney";
    inline constexpr auto FINNEY_TESTNET = "finney_testnet";
    inline constexpr auto FINNEY_PRO = "finney_pro";
    inline constexpr auto FINNEY_PRO_TESTNET = "finney_pro_testnet";
    inline constexpr auto BYZANTINE_PRO = "byzantine_pro";
    inline constexpr auto BYZANTINE_PRO_TESTNET = "byzantine_pro_testnet";
    inline constexpr auto BYZANTINE_PREMIER = "byzantine_premier";
    inline constexpr auto BYZANTINE_PREMIER_TESTNET = "byzantine_premier_testnet";
    inline constexpr auto BYZANTINE_GUARDIAN = "byzantine_premier_testnet"; //TBD
    inline constexpr auto BYZANTINE_GUARDIAN_TESTNET = "byzantine_premier_testnet"; //TBD
}

QStringList Slugs::safeSlugs() const
{
    auto slugList = slugs();
    if (slugList.isEmpty()) {
        DBG_FATAL << "slugs() returned null!";
        return QStringList();
    }
    return slugList;
}

bool Slugs::containsSlug(const QStringList& slugsToCheck) const
{
    const auto& slugList = safeSlugs();
    for (const auto& slug : slugsToCheck) {
        if (slugList.contains(slug)) {
            return true;
        }
    }
    return false;
}

bool Slugs::isSubscribed() const
{
    return (safeSlugs().size() > 0);
}

bool Slugs::isByzantineStandard() const
{
    return containsSlug({BYZANTINE, BYZANTINE_TESTNET});
}

bool Slugs::isByzantinePremier() const
{
    return containsSlug({BYZANTINE_PREMIER, BYZANTINE_PREMIER_TESTNET});
}

bool Slugs::isByzantinePro() const
{
    return containsSlug({BYZANTINE_PRO, BYZANTINE_PRO_TESTNET});
}

bool Slugs::isFinneyPro() const
{
    return containsSlug({FINNEY_PRO, FINNEY_PRO_TESTNET});
}

bool Slugs::isFinneyStandard() const
{
    return containsSlug({FINNEY, FINNEY_TESTNET});
}

bool Slugs::isFinney() const
{
    return isFinneyPro() || isFinneyStandard();
}

bool Slugs::isByzantine() const
{
    return isByzantineStandard() || isByzantinePro() || isByzantinePremier();
}

bool Slugs::isHoneyBadger() const
{
    return containsSlug({HONEY_BADGER, HONEY_BADGER_TESTNET});
}

bool Slugs::isIronHand() const
{
    return containsSlug({IRON_HAND, IRON_HAND_TESTNET});
}

bool Slugs::isMultiSubscriptions() const
{
    return slugs().count() > 1;
}

bool Slugs::isPremierGroup() const
{
    return isByzantinePremier();
}

bool Slugs::isGroupWallet() const
{
    return isByzantine() || isFinney() || isHoneyBadgerPlus() || isHoneyBadgerPremier();
}

bool Slugs::isUserWallet() const
{
    return  isHoneyBadger() || isIronHand();
}

bool Slugs::isHoneyBadgerPlus() const
{
    return containsSlug({HONEY_BADGER_PLUS, HONEY_BADGER_PLUS_TESTNET});
}

bool Slugs::isHoneyBadgerPremier() const
{
    return containsSlug({HONEY_BADGER_PREMIER, HONEY_BADGER_PREMIER_TESTNET});
}

bool Slugs::isUserDraftWallet() const
{
    return false;
}

bool Slugs::isFreeWallet() const
{
    return (safeSlugs().size() == 0);
}
