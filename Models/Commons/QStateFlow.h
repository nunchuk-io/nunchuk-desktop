#ifndef QSTATEFLOW_H
#define QSTATEFLOW_H

#include <QObject>
#include <QStack>

class QStateFlow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString screenFlow            READ screenFlow                 WRITE setScreenFlow       NOTIFY screenFlowChanged)
public:
    QString screenFlow() const;
    void setScreenFlow(const QString& flow);

public slots:
    void backScreen();
    void clearState();
signals:
    void screenFlowChanged();
private:
    QStack<QString> stringQStack {};
};

#endif // QSTATEFLOW_H
