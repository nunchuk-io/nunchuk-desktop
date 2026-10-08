import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.12
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
import "../components"
import Features.Inheritance.OffChain.ViewModels 1.0

QOnScreenContentTypeB {
    id: root
    
    DetailBeneficiaryViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: vm.benefiEmail
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
                    text: STR.STR_QML_2178
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
                        ScrollBar.vertical: ScrollBar { active: true }
                        
                        Column {
                            id: stagesColumn
                            width: parent.width
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
                                    text: STR.STR_QML_2179 + ": "
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
                            text: STR.STR_QML_2221.arg(100)
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: vm.close()
    bottomRight: Row {
        spacing: 12        
        QIconTextButton {
            width: 133
            height: 48
            label: STR.STR_QML_2204
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
            width: 100
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 14
            type: eTypeE
            enabled: vm.totalPercentage == 100
            onButtonClicked: {
                vm.onContinueClicked()
            }
        }
    }
}

