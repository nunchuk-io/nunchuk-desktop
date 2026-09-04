import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import Features.Inheritance.OffChain.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../components"

QOnScreenContentTypeA {
    id: root
    
    ReleaseScheduleViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: QSTR.STR_QML_2151
    extraHeader: Item {}
    
    onCloseClicked: vm.close()
    
    content: Item {
        width: 661
        height: 450
        
        Column {
            anchors.fill: parent
            spacing: 24
            
            // Description text
            Item {
                width: parent.width
                height: descText.paintedHeight
                
                QLato {
                    id: descText
                    text: QSTR.STR_QML_2178
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                }
            }
            
            // Main content area with two columns
            Row {
                height: 428
                spacing: 12
                
                Rectangle {
                    width: 370
                    height: 428
                    border.color: "#DEDEDE"
                    border.width: 1
                    color: "#FFFFFF"
                    radius: 12
                    Flickable {
                        width: parent.width
                        height: parent.height
                        contentHeight: stagesColumn.height
                        clip: true
                        ScrollBar.vertical: QScrollBar { }

                        Column {
                            id: stagesColumn
                            width: parent.width - 8  // leave room for QScrollBar (8px)
                            spacing: 0
                            
                            Repeater {
                                model: vm.stagesData                                
                                QStageDetail {
                                    width: parent.width
                                    stageName: modelData.name
                                    stagePercentage: modelData.percentage
                                    stageColor: modelData.color
                                    firstWithdrawalDate: modelData.firstWithdrawalDate
                                    releaseInfo: modelData.releaseInfo
                                    isExpanded: modelData.isExpanded
                                    installments: modelData.displayInstallments
                                    isFirst: index === 0
                                    isLast: index === vm.stagesData.length - 1
                                    
                                    onEditClicked: {
                                        vm.onEditStageClicked(index)
                                    }
                                    
                                    onExpandClicked: {
                                        vm.onExpandClicked(index)
                                    }
                                }
                            }
                        }
                    }
                }
                
                
                // Right column: Overview
                Column {
                    width: 346
                    spacing: 10                   
                    Rectangle {
                        id: _bufferRect
                        width: 346
                        height: Math.max(48, _bpRow.height + 16)
                        color: "#F5F5F5"
                        radius: 12
                        QBufferPeriodRow {
                            id: _bpRow
                            width: parent.width - 12 - 12 - _editBtn.width - 4
                            wrap: true
                            buffer_period: vm.buffer_period
                            buffer_apply_on: vm.buffer_apply_on
                            anchors {
                                left: parent.left
                                leftMargin: 12
                                verticalCenter: parent.verticalCenter
                            }
                        }
                        QTextLink {
                            id: _editBtn
                            width: 29
                            height: 20
                            anchors {
                                right: parent.right
                                rightMargin: 12
                                top: _bpRow.top
                            }
                            text: QSTR.STR_QML_849
                            color: "#031F2B"
                            onTextClicked: {
                                vm.onBufferPeriodonClicked()
                            }
                        }
                    }
                    Column {
                        width: 346
                        spacing: 8
                        Rectangle {
                            width: 346
                            height: 99
                            color: "#F5F5F5"
                            radius: 12
                            // Total allocated section
                            Column {
                                anchors.fill: parent
                                anchors.margins: 16
                                spacing: 10
                                Row {
                                    height: 16
                                    spacing: 0
                                    
                                    QLato {
                                        text: QSTR.STR_QML_2179 + ": "
                                        font.pixelSize: 12
                                        font.weight: Font.Bold
                                    }
                                    
                                    QLato {
                                        text: vm.totalPercentage + "%"
                                        font.pixelSize: 12
                                        font.weight: Font.Bold
                                    }
                                }
                                // Timeline visualization
                                QTimelineVisualization {
                                    width: 314
                                    displayPercentage: true
                                    timelineStages: vm.timelineStagesData
                                }
                            }
                        }
                        Row {
                            height: 24
                            spacing: 4
                            visible: vm.totalPercentage > 100
                            QIcon {
                                iconSize: 20
                                anchors.verticalCenter: parent.verticalCenter
                                source: "qrc:/Images/Images/error_outline_24px.png"
                            }
                            QLato {
                                width: 327
                                height: 16
                                color: "#CF4018"
                                anchors.verticalCenter: parent.verticalCenter
                                text: QSTR.STR_QML_2221.arg(100)
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                   }
                }
            }
        }
    }
    bottomLeft: Row {
        spacing: 12
        QButtonTextLink {
            width: 97
            height: 48
            label: QSTR.STR_QML_059
            visible: (vm.beneficiary_mode === "MULTIPLE" && vm.release_method === "INDIVIDUAL")
            onButtonClicked: {
                vm.back()
            }
        }
        QTextButton {
            width: 100
            height: 48
            visible: vm.beneficiary_mode === "SINGLE" || (vm.beneficiary_mode === "MULTIPLE" && vm.release_method === "SHARED")
            label.text: QSTR.STR_QML_245
            label.font.pixelSize: 14
            type: eTypeF
            onButtonClicked: vm.close()
        }
    }
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 133
            height: 48
            label: QSTR.STR_QML_2204
            icons: ["add-dark.svg", "add-dark.svg", "add-dark.svg","add-dark.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeR
            onButtonClicked: {
                // Add new stage
                vm.onAddStageClicked()
            }
        }
        
        QTextButton {
            width: 66
            height: 48
            label.text: QSTR.STR_QML_835
            label.font.pixelSize: 14
            type: eTypeE
            enabled: vm.totalPercentage == 100
            onButtonClicked: {
                vm.onSaveClicked()
            }
        }
    }
}
