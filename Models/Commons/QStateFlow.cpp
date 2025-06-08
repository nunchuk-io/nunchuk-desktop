#include "QStateFlow.h"
#include "QOutlog.h"

QString QStateFlow::screenFlow() const {
    if (stringQStack.isEmpty()) {
        return "";
    } else {
        return stringQStack.top();
    }
}

void QStateFlow::setScreenFlow(const QString &flow) {
    if (screenFlow() != flow && stringQStack.size() > 25)
        stringQStack.pop_front();
    stringQStack.push(flow);
    emit screenFlowChanged();
}

void QStateFlow::backScreen() {
    if (stringQStack.isEmpty())
        return;
    stringQStack.pop();
    emit screenFlowChanged();
}

void QStateFlow::clearState() {
    stringQStack.clear();
    emit screenFlowChanged();
}

bool QStateFlow::isScreenFlow(const QString &flow) {
    if (stringQStack.isEmpty())
        false;
    if (stringQStack.top() == flow) {
        return true;
    }
    return false;
}

bool QStateFlow::isEmptyScreenFlow() {
    return stringQStack.isEmpty();
}