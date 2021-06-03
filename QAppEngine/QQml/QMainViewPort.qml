import QtQuick 2.0

Item {
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT

    property var screen_layer_base: screen_layer_1

    Loader {
        id: screen_layer_1
        width: QAPP_DEVICE_WIDTH
        height: QAPP_DEVICE_HEIGHT
        onLoaded: { switch_screen_layer_base( screen_layer_2, screen_layer_1 ); }
    }

    Loader {
        id: screen_layer_2
        width: QAPP_DEVICE_WIDTH
        height: QAPP_DEVICE_HEIGHT
        onLoaded: { switch_screen_layer_base( screen_layer_1, screen_layer_2 ); }
    }

    function switch_screen_layer_base(from, to) {
        if ( from.item !== null ) { from.item.visible = false; from.sourceComponent = null }
        if ( to.item !== null ) { to.item.visible = true }
        screen_layer_base = from;
    }

    function screen_Transition() {
        screen_layer_base.sourceComponent = null
        screen_layer_base.sourceComponent = QAppScreen
    }

    ListModel { id: onsDataList }
    Repeater {
        id:ons_Creator
        model: onsDataList
        anchors.left: parent.left;
        Loader {
            id: osd_Loader
            width: QAPP_DEVICE_WIDTH
            height: QAPP_DEVICE_HEIGHT
            source: osdSource
        }
    }

    function load_Popup(Onsdata, popCount) {
        onsDataList.clear()
        for(var onsCnt = 0; onsCnt < popCount; onsCnt++ ) { var data = {'osdSource': Onsdata[onsCnt]}; onsDataList.append(data) }
    }
}
