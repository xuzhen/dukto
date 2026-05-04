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

import QtQuick 2.3

Item {
    id: box
    width: parent.width * 0.7
    implicitHeight: backRecangle.height
    clip: true
    state: (guiBehind.showUpdateBanner && (["","ip","progress"].indexOf(guiBehind.overlayState) >= 0)) ? "showed" : "hidden"
    visible: guiBehind.latestVersion !== ""

    Rectangle {
        id: backRecangle
        color: theme.themeColor
        width: parent.width
        anchors.top: parent.top
        implicitHeight: labelText.implicitHeight + labelText.anchors.topMargin + labelText.anchors.bottomMargin + progressBar.height

        Image {
            anchors.fill: parent
            source: "TileGradient.png"
        }

        SText {
            id: labelText
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: 14
            wrapMode: Text.WordWrap
            text: "New version " + guiBehind.latestVersion + " is available, click here to download it!"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                Qt.openUrlExternally("https://github.com/xuzhen/dukto/releases/v" + guiBehind.latestVersion)
                timer.triggered()
            }
        }

        Rectangle {
            id: progressBar
            width: parent.width
            height: 4
            anchors.bottom: parent.bottom
            color: theme.themeLighterColor

            Behavior on x { NumberAnimation { duration: timer.interval } }
        }

        Behavior on x { NumberAnimation { duration: 1000; easing.type: "OutCubic" } }
    }

    Timer {
        id: timer
        interval: 15000
        running: false
        repeat: false
        onTriggered: guiBehind.showUpdateBanner = false
    }

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: backRecangle
                x: box.width
            }
            PropertyChanges {
                target: timer
                running: false
            }
            PropertyChanges {
                target: progressBar
                x: 0
            }
        },
        State {
            name: "showed"
            PropertyChanges {
                target: backRecangle
                x: 0
            }
            PropertyChanges {
                target: timer
                running: true
            }
            PropertyChanges {
                target: progressBar
                x: progressBar.width
            }
        }
    ]

}
