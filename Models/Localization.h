#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <QObject>
#include <QTranslator>
#include <QGuiApplication>


class Localization : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language  NOTIFY languageChanged)
public:
    static Localization *instance();
    Localization(Localization &other) = delete;
    Localization(Localization const &other) = delete;
    void operator=(const Localization &other) = delete;

    void initLanguage();
    QString language() const;
    void setLanguage(const QString &lang);
private:
    Localization();
    ~Localization();
    QTranslator* translator;

signals:
    void languageChanged();
};

#endif // LOCALIZATION_H
