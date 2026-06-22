import QtQuick 2.12
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import Features.Inheritance.OffChain.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../core/components"


QOnScreenContentTypeB {
    id: root
    
    SetupAnOffChainTimelockViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2152
    extraHeader: Item {}
    onCloseClicked: vm.close()
    
    // Sample timezone data
    
    content: Item {
        width: 728
        height: 200
        
        Column {
            width: parent.width
            spacing: 24
            
            // Description text
            QLato {
                width: parent.width
                text: STR.STR_QML_2189
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
            }
            QSettingTimezone {
                id: settingTimezone
                width: 362
                height: 68
                onTimezoneSelected: {
                    vm.valueTimezone = settingTimezone.timelockVM.timezones.selectedTimezone
                }
            }
            Connections {
                target: vm
                onValueTimezoneChanged: {
                    settingTimezone.timelockVM.timezones.setSelectedTimezone(vm.valueTimezone)
                }
            }
        }
    }
    onPrevClicked: vm.back()
    bottomRight: Row {
        spacing: 12        
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 14
            type: eTypeE
            onButtonClicked: {
                vm.timeLockContinueClicked()
                vm.close()
            }
        }
    }
}
