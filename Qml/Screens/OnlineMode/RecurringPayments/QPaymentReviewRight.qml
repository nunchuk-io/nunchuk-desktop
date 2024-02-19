import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR
Item {
    id: _item
    height: _col.childrenRect.height
    Column {
        id: _col
        spacing: 16
        QTextInputBoxTypeB {
            id: _allow_co_sign
            visible: AppModel.walletInfo.recurringPayment.hasServerKey()
            label: STR.STR_QML_1117
            boxWidth: right_width
            boxHeight: 48
            textInputted: payment.allow_cosigning ? STR.STR_QML_433 : STR.STR_QML_432
            enabled: false
        }
        QTextAreaBoxTypeA  {
            id: _note
            boxWidth: right_width
            boxHeight: input.paintedHeight + 20
            label: STR.STR_QML_218
            textInputted: payment.transaction_note !== "" ? payment.transaction_note : STR.STR_QML_1160
            isValid: true
            input.verticalAlignment: payment.transaction_note !== "" && _note.input.lineCount > 1 ? Text.AlignTop : Text.AlignVCenter
            input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            enabled: false
        }
    }

}
