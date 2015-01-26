import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtSensors 5.3
import Game 1.0

Item
{
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    Game
    {
        id: terminus
        anchors.fill: parent

        UserInterface
        {
            id: ui
            anchors.fill: parent
            focus: true

            Component.onCompleted:
            {
                ui.setGame(terminus);
            }

            Keys.onPressed:
            {
                ui.keyPressEvent(event.key)
                event.accepted = true
            }
            Keys.onReleased:
            {
                ui.keyReleaseEvent(event.key)
                event.accepted = true
            }

            MouseArea
            {
                anchors.fill: parent
                cursorShape: "BlankCursor"
                hoverEnabled: true
                onPositionChanged:
                {
                    if(Qt.platform.os !== ("android" || "ios"))
                    {
                            ui.mouseMoveEvent(mouse.x, mouse.y);
                    }
                }
            }

            Rectangle
            {
                id: enemyStatus
                color: "transparent"

                property int wagons: 5

                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: parent.height * 0.01
                width: (parent.width * 0.04) * wagons
                height: parent.height * 0.03

                function initStatus()
                {
                    for(var i = 0; i < wagons; i++)
                    {
                        var qml =  'import QtQuick 2.3; '
                                +  'Rectangle{'
                                +   'id: enemyWagon' + i.toString() + ';'
                                +   'anchors.top: parent.top;'
                                +   'anchors.right: parent.right;'
                                +   'anchors.rightMargin: (parent.parent.width * 0.04) * ' + i.toString() + ';'
                                +   'width: parent.parent.width * 0.03;'
                                +   'height: parent.height;'
                                +   'property real health: 100;'
                                +   'color: health === 100? "green" : (health > 25? "yellow" : "red");'
                                +  '}'
                        Qt.createQmlObject(qml, enemyStatus, "enemyStatus");
                    }
                }

                Component.onCompleted:
                {
                    initStatus();
                }
            }

            Rectangle
            {
                id: playerStatus
                color: "transparent"

                property int wagons: 5

                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: parent.height * 0.01
                width: (parent.width * 0.04) * wagons
                height: parent.height * 0.03

                function initStatus()
                {
                    for(var i = 0; i < wagons; i++)
                    {
                        var qml =  'import QtQuick 2.3; '
                                +  'Rectangle{'
                                +   'id: playerWagon' + i.toString() + ';'
                                +   'anchors.bottom: parent.bottom;'
                                +   'anchors.right: parent.right;'
                                +   'anchors.rightMargin: (parent.parent.width * 0.04) * ' + i.toString() + ';'
                                +   'width: parent.parent.width * 0.03;'
                                +   'height: parent.height;'
                                +   'property real health: 100;'
                                +   'color: health === 100? "green" : (health > 25? "yellow" : "red");'
                                +  '}'
                        Qt.createQmlObject(qml, playerStatus, "playerStatus");
                    }
                }

                Component.onCompleted:
                {
                    initStatus();
                }
            }

            Rectangle
            {
                id: wagonSwitchArea
                color: "transparent"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: parent.height * 0.01
                width: parent.width * 0.25
                height: parent.height * 0.3

                Rectangle
                {
                    id: wagonSwitchLeft
                    color: "steelblue"
                    opacity: 0.8
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    width: parent.width * 0.45
                    height: parent.height

                    MultiPointTouchArea
                    {
                        id: switchLeft
                        anchors.fill: parent

                        onReleased:
                        {
                            ui.switchWagonLeft();
                        }
                    }

                    Text
                    {
                        id: switchLeftText
                        text: "<--"
                        font.family: "Helvetica"
                        font.pointSize: 36
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Rectangle
                {
                    id: wagonSwitchRight
                    color: "steelblue"
                    opacity: 0.8
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    width: parent.width * 0.45
                    height: parent.height

                    MultiPointTouchArea
                    {
                        id: switchRight
                        anchors.fill: parent

                        onReleased:
                        {
                            ui.switchWagonRight();
                        }
                    }

                    Text
                    {
                        id: switchRightText
                        text: "-->"
                        font.family: "Helvetica"
                        font.pointSize: 36
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Rectangle
            {
                id: reticle
                color: "transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                property real scale: 0.1

                height: parent.height * scale
                width: parent.height * scale

                Rectangle
                {
                    id: reticleRight
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    border.color: "black"
                    border.width: 2
                    height: 5
                    width: terminus.width * 0.01
                }
                Rectangle
                {
                    id: reticleLeft
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    border.color: "black"
                    border.width: 2
                    height: 5
                    width: terminus.width * 0.01
                }
                Rectangle
                {
                    id: reticleTop
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    border.color: "black"
                    border.width: 2
                    height: terminus.width * 0.01
                    width: 5
                }
                Rectangle
                {
                    id: reticleBottom
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    border.color: "black"
                    border.width: 2
                    height: terminus.width * 0.01
                    width: 5
                }
            }

            Rectangle
            {
                id: actionArea
                color: "transparent"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: parent.height * 0.01
                width: parent.width * 0.2
                height: parent.height * 0.3

                Rectangle
                {
                    id: actionButton
                    color: "steelblue"
                    opacity: 0.8
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    width: parent.width * 0.6
                    height: parent.height

                    MultiPointTouchArea
                    {
                        id: fire
                        anchors.fill: parent

                        onPressed:
                        {
                            ui.touchChargeFire();
                            actionButton.color = "red"
                            reticle.scale = 0.05
                        }

                        onReleased:
                        {
                            ui.touchFire();
                            actionButton.color = "yellow"
                            reticle.scale = 0.2
                        }
                    }

                    Text
                    {
                        id: actionButtonText
                        text: "Fire!"
                        font.family: "Helvetica"
                        font.pointSize: 36
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Rectangle
                {
                    id: actionStatusBar
                    color: "grey"
                    opacity: 0.8
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    width: parent.width * 0.3
                    height: parent.height

                    Rectangle
                    {
                        id: actionStatus
                        property real status: 0
                        color: "steelblue"
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: parent.height * status
                    }
                }
            }

            OrientationSensor
            {
                id: orientation
                dataRate: 50
                active: true
                onReadingChanged: {
                    if (reading.orientation === OrientationReading.LeftUp)
                    {
                        gyro.orientation_multiplier = 1
                    }
                    if (reading.orientation === OrientationReading.RightUp)
                    {
                        gyro.orientation_multiplier = -1
                    }
                }
            }

            Gyroscope
            {
                id: gyro
                dataRate: 50
                active: true

                property int orientation_multiplier: 1

                onReadingChanged:
                {
                    ui.gyroMoveEvent(gyro.reading.x * orientation_multiplier
                                     , gyro.reading.y * orientation_multiplier)
                }
            }

            /*MultiPointTouchArea
            {
                id: flick
                anchors.fill: parent
                minimumTouchPoints: 1
                touchPoints:
                [
                    TouchPoint { id: touchF1 }
                ]

                onTouchUpdated:
                {
                    ui.flickEvent(touchF1.startX, touchF1.x);
                }
                onReleased:
                {
                    ui.flickReset();
                }
            }*/

        }
    }
}
