import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtSensors 5.3
import terminus 1.0

Item
{
    width: 333
    height: 222

    Game
    {
        id: terminusGame
        anchors.fill: parent
        focus: true
        Keys.onPressed:
        {
            terminusGame.keyPressEvent(event.key)
            event.accepted = true
        }
        Keys.onReleased:
        {
            terminusGame.keyReleaseEvent(event.key)
            event.accepted = true
        }
    }

    MouseArea
    {
        anchors.fill: parent
        cursorShape: "BlankCursor"
        hoverEnabled: true
        onPositionChanged:
        {
            terminusGame.mouseMoveEvent(mouse.x, mouse.y);
        }
    }

    MultiPointTouchArea
    {
        id: touchMove
        anchors.top: parent.top
        height: parent.height * 0.6
        width: parent.width
        minimumTouchPoints: 1
        touchPoints:
        [
            TouchPoint { id: touchM1 },
            TouchPoint { id: touchM2 }
        ]

        onPressed:
        {
            terminusGame.touchChargeFire();
        }

        onReleased:
        {
            terminusGame.touchFire();
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
            terminusGame.flickEvent(touchF1.startX, touchF1.x);
        }
        onReleased:
        {
            terminusGame.flickReset();
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
            terminusGame.gyroMoveEvent(gyro.reading.x * orientation_multiplier, gyro.reading.y * orientation_multiplier)
        }
    }
}
