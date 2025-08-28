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
    auto pushOrMoveToTop = [this](const QString &value) {
        int index = stringQStack.indexOf(value);
        if (index != -1) {
            stringQStack.remove(index);   // remove old position
        }
        stringQStack.push(value);         // push to top
    };
    pushOrMoveToTop(flow);
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
        return false;
    if (stringQStack.top() == flow) {
        return true;
    }
    return false;
}

bool QStateFlow::isEmptyScreenFlow() {
    return stringQStack.isEmpty();
}
