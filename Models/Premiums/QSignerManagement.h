#ifndef QSIGNERMANAGEMENT_H
#define QSIGNERMANAGEMENT_H

#include <QObject>

typedef bool (*Execute)();
class QSignerManagement : public QObject
{
    Q_OBJECT
private:
    QSignerManagement();
    ~QSignerManagement();
public:
    static QSignerManagement *instance();
    QSignerManagement(QSignerManagement &other) = delete;
    QSignerManagement(QSignerManagement const &other) = delete;
    void operator=(const QSignerManagement &other) = delete;

    void registerCreateMasterSigner(Execute func);
    bool finishCreateMasterSigner();

    void registerCreateRemoteSigner(Execute func);
    bool finishCreateRemoteSigner();

    void registerCreateSoftwareSigner(Execute func);
    bool finishCreateSoftwareSigner();

    void registerCreateSoftwareSignerXprv(Execute func);
    bool finishCreateSoftwareSignerXprv();

    void clearExecute();

signals:
private:
    Execute m_executeMaster {nullptr};
    Execute m_executeRemote {nullptr};
    Execute m_executeSoftware {nullptr};
    Execute m_executeSoftXprv {nullptr};
};

#endif // QSIGNERMANAGEMENT_H
