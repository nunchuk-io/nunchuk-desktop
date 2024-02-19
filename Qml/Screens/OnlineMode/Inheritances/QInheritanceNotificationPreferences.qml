import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property var inheritance
    property string emails: inheritance.display_emails
    property var splitEmails: emails.split(",")

    height: 64 + Math.max(1, splitEmails.length) * 20
    Column {
        anchors.fill: parent
        spacing: 12
        QLato {
            font.weight: Font.Bold
            font.pixelSize: 16
            text: STR.STR_QML_852
            anchors.left: _item.left
        }
        Rectangle {
            width: _item.width
            height: _email.height + _notify.height + 16*4 + 1
            radius: 12
            color: "#F5F5F5"
            Column {
                anchors.centerIn: parent
                spacing: 16
                Item {
                    id: _email
                    width: _item.width - 16*2
                    height: Math.max(40, Math.max(1, splitEmails.length) * 20)
                    QLato {
                        width: 245
                        height: 40
                        anchors.left: parent.left
                        text: STR.STR_QML_867
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Column {
                        anchors.right: parent.right
                        spacing: 1
                        QLato {
                            width: 302
                            height: 20
                            font.weight: Font.Bold
                            text: "None"
                            color: "#031F2B"
                            horizontalAlignment: Text.AlignRight
                            visible: emails === ""
                        }
                        Repeater {
                            model: splitEmails
                            QLato {
                                width: 302
                                height: 20
                                font.weight: Font.Bold
                                text: splitEmails[index]
                                color: inheritance.display_emails_isChanged ? "#CF4018": "#031F2B"
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                }
                Rectangle {
                    width: 311
                    height: 1
                    color: "#EAEAEA"
                }
                Item {
                    id: _notify
                    width: _item.width - 16*2
                    height: 20
                    QLato {
                        width: 245
                        height: 20
                        anchors.left: parent.left
                        text: STR.STR_QML_868
                    }
                    QLato {
                        width: 245
                        height: 20
                        font.weight: Font.Bold
                        anchors.right: parent.right
                        text: inheritance.notify_today ? STR.STR_QML_433 : STR.STR_QML_432
                        color: inheritance.notify_today_isChanged ? "#CF4018": "#031F2B"
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}
