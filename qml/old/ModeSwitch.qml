/* DUKTO - A simple, fast and multi-platform file transfer tool for LAN users
 * Copyright (C) 2026 Xu Zhen
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

import QtQuick 2.3

Rectangle {
    id: switchBox
    implicitWidth: 24
    implicitHeight: 44
    border.color: indicatorArea.containsMouse ? theme.dimmedTextColor : theme.borderColor
    border.width: 2
    radius: width / 2
    rotation: -90
    gradient: Gradient {
        GradientStop { position: 0.1; color: "#F0F0F0" }
        GradientStop { position: 0.9; color: "#0F0F0F" }
    }

    Rectangle {
        id: indicator
        anchors.left: parent.left
        color: theme.bgColor
        width: parent.width
        height: width
        radius: width / 2
        border.color: parent.border.color
        border.width: parent.border.width
        y: guiBehind.darkMode ? indicatorArea.drag.maximumY : indicatorArea.drag.minimumY

        Behavior on y { SmoothedAnimation { duration: 200 } }
        Behavior on color { ColorAnimation { duration: 100 } }
    }

    MouseArea {
        id: indicatorArea
        hoverEnabled: true
        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
        anchors.fill: parent
        drag.target: indicator
        drag.axis: Drag.YAxis
        drag.minimumY: 0
        drag.maximumY: switchBox.height - indicator.height
        drag.onActiveChanged: {
            if (!drag.active) {
                if (indicator.y >= (drag.maximumY - drag.minimumY) / 2) {
                    indicator.y = drag.maximumY
                    guiBehind.darkMode = true
                } else {
                    indicator.y = drag.minimumY
                    guiBehind.darkMode = false
                }
            }
        }
        onClicked: {
            guiBehind.darkMode = !guiBehind.darkMode;
            // walkaround a Qt bug: after dragging, the clicking won't change indicator.y
            indicator.y = guiBehind.darkMode ? drag.maximumY : drag.minimumY
        }
    }
}
