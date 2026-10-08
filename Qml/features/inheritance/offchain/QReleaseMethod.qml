import QtQuick 2.12
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import Features.Inheritance.OffChain.ViewModels 1.0

QOnScreenContentTypeB {
    id: root
    
    ReleaseMethodViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2153
    extraHeader: Item {}
    onCloseClicked: vm.close()
    
    property var methodMaps: [
        { id: "SHARED", labelTop: STR.STR_QML_2182, labelBottom: STR.STR_QML_2183 },
        { id: "INDIVIDUAL", labelTop: STR.STR_QML_2167, labelBottom: STR.STR_QML_2185 }
    ]
    
    
    content: Item {
        width: 728
        height: 240
        Row {
            width: parent.width
            height: 120
            spacing: 16
            Repeater {
                model: methodMaps
                delegate: QRadioButtonTypeF {
                    width: 356
                    height: 120
                    labelMaxWidth: 284
                    labelBottomLineHeight: 20
                    selected: vm.selectedMethod === modelData.id
                    labelTop: modelData.labelTop
                    labelBottom: modelData.labelBottom
                    onButtonClicked: { vm.selectedMethod = modelData.id }
                }
            }
        }
    }
    onPrevClicked: vm.close()
    nextEnable: vm.selectedMethod !== "" && vm.selectedMethod !== vm.selectedMethodOld
    onNextClicked: vm.onContinueClicked()
}
