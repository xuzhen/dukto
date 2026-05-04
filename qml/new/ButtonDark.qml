/* DUKTO - A simple, fast and multi-platform file transfer tool for LAN users
 * Copyright (C) 2011 Emanuele Colombo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

import QtQuick 2.14

Rectangle {
    id: button
    width: 150
    height: 30
    border.color: buttonEnabled ? theme.themeColor : theme.disabledColor
    border.width: 2
    color: buttonArea.containsMouse ? (buttonArea.pressed ? theme.darkerBgColor : theme.lighterBgColor) : theme.bgColor
    clip: true

    property bool buttonEnabled: true
    property alias label: textLabel.text
    signal clicked()

    SText {
        id: textLabel
        anchors.fill: parent
        color: buttonEnabled ? theme.themeColor : theme.disabledColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }

    MouseArea {
        id: buttonArea
        anchors.fill: parent
        hoverEnabled: true
        enabled: button.buttonEnabled
        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ForbiddenCursor
        onClicked: button.clicked();
    }
}
