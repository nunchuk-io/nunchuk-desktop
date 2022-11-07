#ifndef APPONLINEMODEL_H
#define APPONLINEMODEL_H

#include <QObject>

class AppOnlineModel: public QObject
{
    Q_OBJECT
public:
    static AppOnlineModel *instance();
    AppOnlineModel(AppOnlineModel &other) = delete;
    AppOnlineModel(AppOnlineModel const &other) = delete;
    void operator=(const AppOnlineModel &other) = delete;
private:
    AppOnlineModel();
    ~AppOnlineModel();
};

#endif // APPONLINEMODEL_H
