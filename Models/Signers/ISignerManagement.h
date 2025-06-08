#ifndef ISIGNERMANAGEMENT_H
#define ISIGNERMANAGEMENT_H

typedef bool (*Execute)();

class ISignerManagement {
public:
    virtual ~ISignerManagement() = default;

    virtual void registerCreateMasterSigner(Execute func) {};
    virtual bool finishCreateMasterSigner() {
        return false;
    };

    virtual void registerCreateRemoteSigner(Execute func) {};
    virtual bool finishCreateRemoteSigner() {
        return false;
    };

    virtual void registerCreateSoftwareSigner(Execute func) {};
    virtual bool finishCreateSoftwareSigner() {
        return false;
    };

    virtual void registerCreateSoftwareSignerXprv(Execute func) {};
    virtual bool finishCreateSoftwareSignerXprv() {
        return false;
    };
    virtual void clearExecute() {};
};

#endif // ISIGNERMANAGEMENT_H