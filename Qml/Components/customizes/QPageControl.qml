import QtQuick 2.4

Row {
    width: 144
    height: 24
    spacing: 16
    readonly property var chevronLeft: ["qrc:/Images/Images/chevron_left_24px_n.png", "qrc:/Images/Images/chevron_left_24px_d.png"]
    readonly property var chevronRight: ["qrc:/Images/Images/chevron_right_24px_n.png", "qrc:/Images/Images/chevron_right_24px_d.png"]
    readonly property var firstPage: ["qrc:/Images/Images/first_page_24px_n.png", "qrc:/Images/Images/first_page_24px_d.png"]
    readonly property var lastPage: ["qrc:/Images/Images/last_page_24px_n.png", "qrc:/Images/Images/last_page_24px_d.png"]

    property int currentPage: 1
    property int totalPage: 5

    signal pageRequest(var pageNumber)

    Image {
        width: 24
        height: 24
        enabled: currentPage != 1
        source: enabled ? firstPage[0] : firstPage[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage = 1
                pageRequest(currentPage)
            }
        }
    }
    Image {
        width: 24
        height: 24
        enabled: (currentPage > 1)
        source: enabled ? chevronLeft[0] : chevronLeft[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage--;
                pageRequest(currentPage)
            }
        }
    }

    Image {
        width: 24
        height: 24
        enabled: (currentPage < totalPage)
        source: enabled ? chevronRight[0] : chevronRight[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage++;
                pageRequest(currentPage)
            }
        }
    }
    Image {
        width: 24
        height: 24
        enabled: (currentPage != totalPage)
        source: enabled ? lastPage[0] : lastPage[1]
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                currentPage = totalPage
                pageRequest(currentPage)
            }
        }
    }
}
