#ifndef SERVICESETTING_H
#define SERVICESETTING_H

#include <QObject>

class ServiceSetting : public QObject
{
    Q_OBJECT
public:
    explicit ServiceSetting(QObject *parent = nullptr);
    static ServiceSetting *instance();
    ServiceSetting(ServiceSetting &other) = delete;
    ServiceSetting(ServiceSetting const &other) = delete;
    void operator=(const ServiceSetting &other) = delete;

signals:

};

#endif // SERVICESETTING_H
