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
import QtQuick 2.4
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Texts"

Item {
	id: root
	width: 627
	height: contentColumn.implicitHeight

	property string label: ""
	property string description: ""
	property bool checked: false
	property alias switchItem: enableSwitch

	signal toggled(bool checked)

	Column {
		id: contentColumn
		width: parent.width
		spacing: 8

		Row {
			width: parent.width
			height: 36

			QLato {
				width: parent.width - enableSwitch.width
				height: parent.height
				text: root.label
				font.pixelSize: 16
				font.weight: Font.Normal
				verticalAlignment: Text.AlignVCenter
				horizontalAlignment: Text.AlignLeft
			}

			QSwitchTypeB {
				id: enableSwitch
				width: 64
				height: 36
				anchors.verticalCenter: parent.verticalCenter
				switchOn: root.checked
				onSwitchOnChanged: {
					root.checked = switchOn
					root.toggled(switchOn)
				}
			}
		}

		QLato {
			width: parent.width
			text: root.description
			visible: text !== ""
			font.pixelSize: 12
			font.weight: Font.Normal
			color: "#595959"
			lineHeight: 16
			lineHeightMode: Text.FixedHeight
			wrapMode: Text.WordWrap
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignLeft
		}
	}
}