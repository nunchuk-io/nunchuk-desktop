import QtQuick 2.0
import QtGraphicalEffects 1.0
import DataPool 1.0
import HMIEVENTS 1.0
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _id
    function contains(id) {
        for (var i = 0; i < ServiceSetting.qAssistedSetuped.length; i++) {
            if (ServiceSetting.qAssistedSetuped[i] === id)
            {
                return true
            }
        }
        return false
    }
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        QLato {
            text: STR.STR_QML_837
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 28
            font.weight: Font.Bold
        }


        Repeater {
            model: AppModel.walletList
            QLinearGradient {
                width: 651
                height: dele.visible ? 92 : 0
                QAssistedWalletDelegate {
                    id: dele
                    anchors.fill: parent
                    walletName: model.wallet_name
                    walletBalance: model.wallet_Balance
                    walletCurrency: model.wallet_Balance_Currency
                    walletM: model.wallet_M
                    walletN: model.wallet_N
                    visible: model.wallet_isAssistedWallet && _id.contains(model.wallet_id)
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_SERVICE_SELECT_WALLET_REQUEST, model.wallet_id)
                    }
                }
            }
        }
    }
}
