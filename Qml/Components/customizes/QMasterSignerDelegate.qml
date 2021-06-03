import QtQuick 2.4
import QtGraphicalEffects 1.0
import "../origins"

Rectangle {
    id: signermanagerdlg
    property alias  nameWidth: text.width
    property alias  signername: id_signername.text
    property alias  signerXFP: id_xfp.text
    property string devicetype: ""
    color: mastersignerMouse.containsMouse ? Qt.rgba(255, 255, 255, 0.1) : "transparent"

    function geticons(type) {
        if(type === "trezor"){return "qrc:/Images/Images/Device_Icons/trezor.png"}
        else if(type === "coldcard"){return "qrc:/Images/Images/Device_Icons/coldcard.png"}
        else if(type === "bitbox02"){return "qrc:/Images/Images/Device_Icons/bitbox.png"}
        else if(type === "ledger"){return "qrc:/Images/Images/Device_Icons/ledger.png"}
        else{return "qrc:/Images/Images/Device_Icons/other.png"}
    }

    Rectangle {
        width: parent.width
        height: 1
        color: "#C9DEF1"
        opacity: 0.1
        anchors.bottom: parent.bottom
    }

    QImage {
        id: indicator
        width: 30
        height: 30
        source: signermanagerdlg.geticons(devicetype)
        anchors.left: parent.left
        anchors.leftMargin: 24
        anchors.verticalCenter: parent.verticalCenter
    }

    Column {
        id: text
        anchors.left: indicator.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        width: 200
        height: 37

        QText {
            id: id_signername
            width: parent.width
            height: 21
            font.family: "Montserrat"
            color: "#F1FAFE"
            elide: Text.ElideRight
            font.pixelSize: 14
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignLeft
        }

        QText {
            id: id_xfp
            width: parent.width
            height: 16
            color: "#F1FAFE"
            font.family: "Lato"
            elide: Text.ElideRight
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
            font.capitalization: Font.AllUppercase
        }
    }

    signal buttonClicked()
    MouseArea {
        id: mastersignerMouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: signermanagerdlg.buttonClicked()
    }
}
