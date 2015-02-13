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

        Component.onCompleted:
        {
            terminus.setUI(ui);
        }

        UserInterface
        {
            id: ui
            anchors.fill: parent
            focus: true

            function switchText()
            {
                if(wagonType === "WeaponWagon")
                {
                    actionButtonText.text = "Fire!";
                    actionButton.color = "steelblue";
                }
                if(wagonType === "RepairWagon")
                {
                    actionButtonText.text = "Repair!";
                    actionButton.color = "grey";
                }
                if(wagonType === "EngineWagon")
                {
                    actionButtonText.text = "Faster!";
                    actionButton.color = "grey";
                }
            }

            onChargeChanged:
            {
                actionStatusBarProgress.actionfactor = charge;
                reticle.factor = (1.0 / (0.5 + charge)) * 0.2;
            }
            onReloadChanged:
            {
                actionStatusBarProgress.actionfactor = reload;
                actionStatusBarProgress.color = "yellow";
            }
            onWagonTypeChanged:
            {
                switchText();
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
                id: mouseCamera
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

                property int wagons: 11

                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: parent.height * 0.01
                width: (parent.width * 0.035) * wagons
                height: parent.height * 0.03

                function initStatus()
                {
                    for(var i = 0; i < wagons; i++)
                    {
                        var qml =  'import QtQuick 2.3; '
                                +  'Rectangle{'
                                +   'opacity: 0.8;'
                                +   'anchors.top: parent.top;'
                                +   'anchors.right: parent.right;'
                                +   'anchors.rightMargin: (parent.parent.width * 0.035) * ' + i.toString() + ';'
                                +   'width: parent.parent.width * 0.03;'
                                +   'height: parent.height;'
                                +   'color: [0, 3, 7].indexOf(' + i + ') > -1? (' + i + ' !== 0? "purple" : "orange") : "blue";'
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

                property int wagons: 13

                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: parent.height * 0.01
                width: (parent.width * 0.035) * wagons
                height: parent.height * 0.03

                function initStatus()
                {
                    for(var i = 0; i < wagons; i++)
                    {
                        var qml =  'import QtQuick 2.3; '
                                +  'Rectangle{'
                                +   'opacity: 0.8;'
                                +   'anchors.bottom: parent.bottom;'
                                +   'anchors.right: parent.right;'
                                +   'anchors.rightMargin: (parent.parent.width * 0.035) * ' + i.toString() + ';'
                                +   'width: parent.parent.width * 0.03;'
                                +   'height: parent.height;'
                                +   'color: [0, 3, 6, 9].indexOf(' + i + ') > -1? (' + i + ' !== 0? "purple" : "orange") : "blue";'
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
                    color: "transparent"
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

                    Image
                    {
                        id: leftArrow
                        source: "/data/arrow.png"
                        anchors.fill: parent
                    }
                }

                Rectangle
                {
                    id: wagonSwitchRight
                    color: "transparent"
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

                    Image
                    {
                        id: rightArrow
                        source: "/data/arrow2.png"
                        anchors.fill: parent
                    }
                }
            }

            Rectangle
            {
                id: reticle
                color: "transparent"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                property real factor: 0.2

                height: parent.height * factor
                width: parent.height * factor

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
                        id: primaryAction
                        anchors.fill: parent

                        onPressed:
                        {
                            ui.touchChargeFire();
                            actionButton.opacity = 0.9
                        }

                        onReleased:
                        {
                            ui.touchFire();
                            actionButton.opacity = 0.8
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
                        id: actionStatusBarProgress
                        color: "steelblue"
                        opacity: 0.8
                        property real actionfactor: 0.0
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: parent.width
                        height: parent.height * actionfactor
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
