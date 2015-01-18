import QtQuick 2.3
import QtQuick.Controls 1.2
import QtSensors 5.3
import terminus 1.0

Item {

    width: 1334
    height: 750

    Game {
        id: terminusGame
        anchors.fill: parent
        focus: true
        Keys.onPressed: {
            terminusGame.keyPressEvent(event.key)
            event.accepted = true
        }
        Keys.onReleased: {
            terminusGame.keyReleaseEvent(event.key)
            event.accepted = true
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: "BlankCursor"
        hoverEnabled: true
        onPositionChanged: {
            terminusGame.mouseMoveEvent(mouse.x, mouse.y);
        }
    }

    PinchArea {
        id: pinch
        anchors.fill: parent
        onPinchStarted: {

        }
        onPinchFinished: {

        }

        //TODO implement pinch interaction
    }

    MultiPointTouchArea {
        id: multitouch
        anchors.fill: parent
        minimumTouchPoints: 1
        touchPoints: [
            TouchPoint { id: touch1 },
            TouchPoint { id: touch2 }
        ]

        property int flicked: 0

        function sendEvent()
        {
            if(touch1.previousX - touch1.x > 60 || touch1.previousX - touch1.x < -60)
            {
                if(flicked === 0)
                {
                    terminusGame.flickEvent(touch1.previousX - touch1.x);
                    flicked = 1;
                }
            }
            else
            {
                if(flicked === 0)
                {
                    terminusGame.touchMoveEvent(touch1.previousX - touch1.x, touch1.previousY - touch1.y);
                }
            }
        }

        onTouchUpdated: {
            sendEvent()
        }
        onReleased:
            flicked = 0
    }

    Gyroscope {
        id: gyro
        dataRate: 50
        active: true
        onReadingChanged: {
            terminusGame.gyroMoveEvent(gyro.reading.x, gyro.reading.y)
        }
    }
}
