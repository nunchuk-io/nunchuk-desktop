#include "Slugs.h"
#include "QOutlog.h"

bool Slugs::isSubscribed() const
{
    return slugs().size() > 0;
}

bool Slugs::isByzantineStandard() const
{
    return slugs().contains("byzantine") || slugs().contains("byzantine_testnet");
}

bool Slugs::isByzantinePremier() const
{
    return slugs().contains("byzantine_premier_testnet") || slugs().contains("byzantine_premier");
}

bool Slugs::isByzantinePro() const
{
    return slugs().contains("byzantine_pro_testnet") || slugs().contains("byzantine_pro");
}

bool Slugs::isFinneyPro() const
{
    return slugs().contains("finney_pro_testnet") || slugs().contains("finney_pro");
}

bool Slugs::isFinneyStandard() const
{
    return slugs().contains("finney_testnet") || slugs().contains("finney");
}

bool Slugs::isFinney() const
{
    return isFinneyPro() || isFinneyStandard();
}

bool Slugs::isByzantine() const
{
    return isByzantineStandard() || isByzantinePro() || isByzantinePremier() || isHoneyBadgerPlus();
}

bool Slugs::isHoneyBadger() const
{
    return slugs().contains("honey_badger_testnet") || slugs().contains("honey_badger");
}

bool Slugs::isIronHand() const
{
    return slugs().contains("iron_hand_testnet") || slugs().contains("iron_hand");
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
    return isByzantine() || isFinney() || isHoneyBadgerPlus();
}

bool Slugs::isUserWallet() const
{
    return  isHoneyBadger() || isIronHand();
}

bool Slugs::isHoneyBadgerPlus() const
{
    return slugs().contains("honey_badger_plus_testnet") || slugs().contains("honey_badger_plus");
}

bool Slugs::isUserDraftWallet() const
{
    return false;
}
