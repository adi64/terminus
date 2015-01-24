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

            Keys.onPressed:
            {
                ui.keyPressEvent(event.key, terminus)
                event.accepted = true
            }
            Keys.onReleased:
            {
                ui.keyReleaseEvent(event.key, terminus)
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
                            ui.mouseMoveEvent(mouse.x, mouse.y, terminus);
                    }
                }
            }

            MultiPointTouchArea
            {
                id: fire
                anchors.fill: parent

                onPressed:
                {
                    ui.touchChargeFire(terminus);
                }

                onReleased:
                {
                    ui.touchFire(terminus);
                }
            }

            MultiPointTouchArea
            {
                id: flick
                anchors.bottom: parent.bottom
                height: parent.height * 0.4
                width: parent.width
                minimumTouchPoints: 1
                touchPoints:
                [
                    TouchPoint { id: touchF1 },
                    TouchPoint { id: touchF2 }
                ]

                onTouchUpdated:
                {
                    ui.flickEvent(touchF1.startX, touchF1.x, terminus);
                }
                onReleased:
                {
                    ui.flickReset(terminus);
                }
            }

            OrientationSensor {
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
                                     , gyro.reading.y * orientation_multiplier
                                     , terminus)
                }
            }
        }
    }
}
