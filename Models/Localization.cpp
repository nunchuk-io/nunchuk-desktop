#include "Localization.h"
#include "QOutlog.h"

Localization::Localization() : translator(new QTranslator())
{

}

Localization::~Localization()
{

}

Localization *Localization::instance()
{
    static Localization mInstance;
    return &mInstance;
}

void Localization::initLanguage()
{
    setLanguage("");
}

QString Localization::language() const
{
    return "" ;
}

void Localization::setLanguage(const QString &lang)
{
    if(translator != NULL) {
        qApp->removeTranslator( translator );
    }
    DBG_INFO << translator->load(":/localization/nunchuk_en.qm");
    qApp->installTranslator( translator );
    DBG_INFO << QObject::tr("STR_HOME_MEMBER_OF_SIGNER");
    emit languageChanged();
}

