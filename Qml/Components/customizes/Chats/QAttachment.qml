/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../Components/origins"
import "../../../../localization/STR_QML.js" as STR

QImage {
    id: rootAttachment
    sourceSize.width: file_mimeType == _FILE_OTHER ? 30 : 100
    sourceSize.height: file_mimeType == _FILE_OTHER ? 30 : 100

    readonly property int _FILE_IMAGE: 0
    readonly property int _FILE_VIDEO: 1
    readonly property int _FILE_OTHER: 2

    property string roomEventID: ""
    property var    progressObject : null
    property int    file_mimeType: _FILE_IMAGE
    property string file_path: ""
    readonly property bool downloaded: progressObject && !progressObject.isUpload && progressObject.completed
    readonly property bool openAfterDownloaded: true

    source: {
        if(file_mimeType === _FILE_IMAGE){
            return (file_path !== "") ? file_path : "qrc:/Images/Images/no-image.jpg"
        }
        else if(file_mimeType === _FILE_VIDEO){
            return "qrc:/Images/Images/video-thumb.png"
        }
        else{
            return "qrc:/Images/Images/attach_file.svg"
        }
    }
    Component.onCompleted: {
        if (file_mimeType === _FILE_IMAGE && (progressObject.localPath.toString() || downloaded )) {
            rootAttachment.source = progressObject.localPath.toString()
        }
    }

    function openExternally()
    {
        if (progressObject.localPath.toString() || downloaded ) { openLocalFile() }
        else {
            if(RoomWalletData.currentRoom !== null) {
                RoomWalletData.currentRoom.downloadFile(rootAttachment.roomEventID)
                tracing.restart()
            }
        }
    }

    function openLocalFile()
    {
        if (Qt.openUrlExternally(progressObject.localPath)) { return; }
        AppModel.showToast(-693, STR.STR_QML_693, EWARNING.EXCEPTION_MSG);

        if (Qt.openUrlExternally(progressInfo.localDir)) { return; }
        AppModel.showToast(-694, STR.STR_QML_694, EWARNING.EXCEPTION_MSG);
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: { openExternally() }
    }

    Timer {
        id: tracing
        interval: 500
        repeat: true
        onTriggered: {
            if(progressObject.completed){
                stop()
                if(openAfterDownloaded) { openLocalFile() }
                if(file_mimeType === _FILE_IMAGE){
                    rootAttachment.source = progressObject.localPath.toString()
                }
            }
        }
    }
}
