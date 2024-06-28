#ifndef ISIGNER_H
#define ISIGNER_H
#include <QJsonArray>

class ISigner
{
public:
    bool GetHistorySignerList();
    QVariantList healthCheckHistory() const;
    void setHealthCheckHistory(QJsonArray list);
    virtual QString fingerPrint() const = 0;
    virtual void healthCheckHistoryChanged() = 0;
private:
    QJsonArray m_healthCheckHistory {};
};
#endif // ISIGNER_H
