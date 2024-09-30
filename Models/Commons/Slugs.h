#ifndef SLUGS_H
#define SLUGS_H
#include <QStringList>

class Slugs
{
public:
    virtual QStringList slugs() const = 0;
    virtual bool isSubscribed() const;
    virtual bool isByzantineStandard() const;
    virtual bool isByzantinePremier() const;
    virtual bool isByzantinePro() const;
    virtual bool isFinneyPro() const;
    virtual bool isFinneyStandard() const;
    virtual bool isFinney() const;
    virtual bool isByzantine() const;
    virtual bool isHoneyBadger() const;
    virtual bool isHoneyBadgerPlus() const;
    virtual bool isIronHand() const;
    virtual bool isMultiSubscriptions() const;

    virtual bool isPremierGroup() const;
    virtual bool isGroupWallet() const;
    virtual bool isUserWallet() const;
};

#endif // SLUGS_H
