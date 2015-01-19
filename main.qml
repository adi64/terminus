import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtSensors 5.3
import terminus 1.0

Item
{
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

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

    PinchArea
    {
        id: pinch
        anchors.fill: parent
        onPinchStarted:
        {
        }
        onPinchFinished:
        {
        }

        //TODO implement pinch interaction
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

        onTouchUpdated:
        {
            terminusGame.touchMoveEvent(touchM1.previousX - touchM1.x, touchM1.previousY - touchM1.y);
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

    Gyroscope
    {
        id: gyro
        dataRate: 50
        active: true
        onReadingChanged:
        {
            terminusGame.gyroMoveEvent(gyro.reading.x, gyro.reading.y)
        }
    }
}
