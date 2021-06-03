
#include "STATE_ID_SCR_INPUT_PIN.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_INPUT_PIN_Entry(QVariant msg) {

}

void SCR_INPUT_PIN_Exit(QVariant msg) {

}

void EVT_INPUT_PIN_SEND_PIN_HANDLER(QVariant msg) {
    QString pinInputted = msg.toString();
    DBG_INFO << pinInputted;
    if(AppModel::instance()->deviceList()){
        for (int i = 0; i < AppModel::instance()->deviceList()->rowCount(); i++) {
            QSharedPointer<Device> selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(i) ;
            if(selectedDv && selectedDv.data()->needsPinSent()){
                QWarningMessage msgwarning;
                bridge::nunchukSendPinToDevice(selectedDv, pinInputted, msgwarning);
            }
        }
        AppModel::instance()->startScanDevices();
    }
}

void EVT_INPUT_PIN_CLOSE_HANDLER(QVariant msg) {

}

