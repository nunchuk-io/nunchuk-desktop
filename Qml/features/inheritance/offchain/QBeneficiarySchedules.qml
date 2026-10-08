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

QOnScreenContentTypeA {
    id: root
    BeneficiarySchedulesViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2154
    extraHeader: Item {}
    onCloseClicked: vm.close()
    content: Item {
        width: 740
        height: 420
        
        Column {
            anchors.fill: parent
            anchors.margins: 0
            spacing: 24

            Rectangle {
                width: 740
                height: 48
                color: "#66D0E2FF"
                visible: vm.hasEditBtn
                QLato {
                    width: 500
                    height: 16
                    text: STR.STR_QML_2153 + ":" + vm.release_method === "INDIVIDUAL" ? STR.STR_QML_2167 : STR.STR_QML_2182
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    anchors {
                        left: parent.left
                        leftMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                }

                QTextLink {
                    width: 29
                    height: 20
                    anchors {
                        right: parent.right
                        rightMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                    text: STR.STR_QML_849
                    color: "#031F2B"
                    onTextClicked: {
                        vm.onReleaseMethodEditClicked()
                    }
                }
            }
            
            // Description
            QLato {
                width: parent.width
                height: paintedHeight
                text: vm.hasEditBtn ? STR.STR_QML_2231: STR.STR_QML_2178
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
            }
            
            // Main content: Beneficiaries and chart
            Row {
                width: parent.width
                height: parent.height
                spacing: 24
                
                // Left column: Beneficiary schedules
                Rectangle {
                    width: 378
                    height: 428
                    color: "#FFFFFF"
                    border {
                        color: "#DEDEDE"
                        width: 1
                    }
                    radius: 8
                    
                    Flickable {
                        anchors.fill: parent
                        anchors.margins: 12
                        contentHeight: contentDisp.implicitHeight
                        clip: true
                        ScrollBar.vertical: ScrollBar { active: true }
                        
                        Column {
                            id: contentDisp
                            width: parent.width
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 8
                            Repeater {
                                model: vm.beneficiaries
                                Rectangle {
                                    width: 343
                                    height: beneficiaryColumn.childrenRect.height + 2*12
                                    border.color: "#DEDEDE"
                                    border.width: 1
                                    radius: 12
                                    Column {
                                        id: beneficiaryColumn
                                        anchors {
                                            left: parent.left
                                            leftMargin: 12
                                            right: parent.right
                                            rightMargin: 12
                                            top: parent.top
                                            topMargin: 12
                                        }
                                        spacing: 8
                                        
                                        // Email
                                        Item {
                                            width: parent.width
                                            height: 20
                                            QLato {
                                                height: 20
                                                text: modelData.email
                                                font.pixelSize: 16
                                                font.weight: Font.Bold
                                            }
                                            QIconButton {
                                                width: 16
                                                height: 16
                                                anchors {
                                                    top: parent.top
                                                    right: parent.right
                                                    rightMargin: 0
                                                }
                                                
                                                icon: "qrc:/Images/Images/edit-dark.svg"
                                                bgColor: "transparent"
                                                onClicked: {
                                                    vm.onEditBeneficiaryEmailClicked(modelData.email)
                                                }
                                            }
                                        }
                                        
                                        
                                        // First withdrawal date
                                        QFirstWithdrawalDateRow {
                                            height: 16
                                            firstWithdrawalDate: modelData.firstWithdrawalDate
                                        }
                                        
                                        // Buffer period (if exists)
                                        QBufferPeriodRow {
                                            height: 16
                                            buffer_period: modelData.buffer_period
                                            buffer_apply_on: modelData.buffer_apply_on
                                        }
                                        
                                        // Timeline visualization
                                        QTimelineVisualization {
                                            timelineStages: modelData.stages
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                 // Right column: Donut chart
                Rectangle {
                    width: 343
                    height: legendColumn.childrenRect.height + 18*2
                    color: "#F5F5F5"
                    radius: 8
                    
                    Row {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 16
                        width: parent.width
                        
                        DonutChart {
                            id: donutChart
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 60                                
                            chartData: vm.assetAllocation
                        }
                        
                        // Legend
                        Column {
                            id: legendColumn
                            anchors.verticalCenter: parent.verticalCenter
                            width: 172
                            spacing: 4
                            
                            Repeater {
                                model: vm.assetAllocation
                                
                                Row {
                                    width: parent.width
                                    height: 16
                                    spacing: 8
                                    
                                    Rectangle {
                                        width: 12
                                        height: 12
                                        color: modelData.color
                                        radius: 2
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    
                                    QLato {
                                        text: STR.STR_QML_2177 + " " + (index + 1) + ": " + (modelData.asset_percentage !== undefined ? modelData.asset_percentage + "%" : "Not set up yet")
                                        color: modelData.asset_percentage !== undefined ? "#031F2B" : "#757575"
                                        font.pixelSize: 12
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    onPrevClicked: vm.back()
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 100
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 14
            type: eTypeE
            enabled: vm.isValidData
            onButtonClicked: {
                vm.onSaveClicked()
            }
        }
    }
}
