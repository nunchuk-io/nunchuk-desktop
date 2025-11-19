import QtQml 2.2

QtObject {
    id: inputtingIndex

    // Parameters
    property string device_name: ""
    property int signer_type: -1
    property string device_type: ""
    property string device_tag: ""
    property string device_xfp: ""
    property string device_bip32_path: ""
    property int current_index: -1
    property int new_index: -1
}
