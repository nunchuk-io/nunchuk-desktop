#include "SignerNammingUseCase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"
#include "core/ui/UiServices.inc"

namespace features::signers::usecases {
using namespace core::usecase;

SignerNammingUseCase &SignerNammingUseCase::addParameter(AppModel *model) {
    m_model = model;
    return *this;
}

Result<QString> SignerNammingUseCase::execute(const QString &input) {
    auto masterList = m_model->masterSignerListPtr()->fullList();
    auto remoteList = m_model->remoteSignerListPtr()->fullList();
    QList<int> listId;
    for (auto signer : masterList) {
        if (signer->name().startsWith(input)) {
            if (qUtils::strCompare(signer->name(), input)) {
                listId.append(0);
            } else {
                QStringList listName = signer->name().split(input);
                QString lastName = listName.at(1);
                lastName.replace("#", "");
                bool isOK = false;
                int id = lastName.toInt(&isOK);
                DBG_INFO << listName << lastName << id << isOK;
                if (isOK) {
                    listId.append(id);
                }
            }
        }
    }
    for (auto signer : remoteList) {
        if (signer->name().startsWith(input)) {
            if (qUtils::strCompare(signer->name(), input)) {
                listId.append(0);
            } else {
                QStringList listName = signer->name().split(input);
                QString lastName = listName.at(1);
                lastName.replace("#", "");
                bool isOK = false;
                int id = lastName.toInt(&isOK);
                DBG_INFO << listName << lastName << id << isOK;
                if (isOK) {
                    listId.append(id);
                }
            }
        }
    }
    std::sort(listId.begin(), listId.end());
    QString nameKey = listId.isEmpty() ? input : QString("%1 #%2").arg(input).arg(QString::fromStdString(std::to_string(listId.last() + 1)));
    
    return Result<QString>::success(nameKey);
}

QString SignerNammingUseCase::getInheritanceNameKey() {
    return execute("Inheritance key").value();
}
}