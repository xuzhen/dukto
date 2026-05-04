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

import QtQuick 2.14

Rectangle {
    id: switchBox
    implicitWidth: 44
    implicitHeight: 24
    border.color: indicatorArea.containsMouse ? theme.dimmedTextColor : theme.borderColor
    border.width: 2
    radius: height / 2
    gradient: Gradient {
        orientation: Gradient.Horizontal
        GradientStop { position: 0.1; color: "#F0F0F0" }
        GradientStop { position: 0.9; color: "#0F0F0F" }
    }

    Rectangle {
        id: indicator
        anchors.top: parent.top
        color: theme.bgColor
        height: parent.height
        width: height
        radius: height / 2
        border.color: parent.border.color
        border.width: parent.border.width
        x: guiBehind.darkMode ? handler.xAxis.maximum : handler.xAxis.minimum

        Behavior on x { SmoothedAnimation { duration: 200 } }
        Behavior on color { ColorAnimation { duration: 100 } }
    }

    MouseArea {
        id: indicatorArea
        hoverEnabled: true
        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
        anchors.fill: parent
        Connections {
            function onClicked() {
                guiBehind.darkMode = !guiBehind.darkMode;
                // walkaround a Qt bug: after dragging, the clicking won't change indicator.x
                indicator.x = (guiBehind.darkMode ? handler.xAxis.maximum : handler.xAxis.minimum)
            }
        }
    }

    DragHandler {
        id: handler
        target: indicator
        yAxis.enabled: false
        xAxis.enabled: true
        xAxis.maximum: switchBox.width - indicator.width
        xAxis.minimum: 0
        onActiveChanged: {
            if (!active) {
                if (indicator.x >= (xAxis.maximum - xAxis.minimum) / 2) {
                    guiBehind.darkMode = true
                    indicator.x = xAxis.maximum
                } else {
                    guiBehind.darkMode = false
                    indicator.x = xAxis.minimum
                }
            }
        }
    }
}
