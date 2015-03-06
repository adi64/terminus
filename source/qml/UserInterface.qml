import QtQuick 2.3
import QtSensors 5.3
import Game 1.0

/*
 * Initializes UserInterface
 * UserInterface is a container for all UI elements
 *
 * Interaction will sent a integer to identify the type of interaction
 *
 * enum InteractionType
 * {
 *      MOUSE_MOVEMENT = 0,
 *      TOUCH_MOVEMENT = 1,
 *      GYRO_MOVEMENT = 2,
 *      NEXT_WAGON_BUTTON = 3,
 *      PREV_WAGON_BUTTON = 4,
 *      ACTION_BUTTON = 5,
 *      LEFT_MOUSE_BUTTON = 6
 *  };
 */

Item
{
    anchors.fill: parent
    focus: true

    property Game game

    Keys.onPressed:
    {
        terminus.keyInput(event.key)
        event.accepted = true
    }

    MouseArea
    {
        id: mouseCamera
        anchors.fill: parent
        cursorShape: "BlankCursor"
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        enabled: Qt.platform.os === ("android" || "ios")? false : true
        onPositionChanged:
        {
            if (containsMouse){
                terminus.moveInput(0, mouse.x, mouse.y)
            }
        }
        onReleased:
        {
            terminus.buttonInput(6)
        }
    }

    Reticle{}

    StatusBarContainer{}

    WagonActionArea
    {
        id: actionArea
        //visible: Qt.platform.os === ("android" || "ios")? true : false
        onFire:
        {
            terminus.buttonInput(5)
        }
    }

    WagonSwitchArea
    {
        id: switchArea
        //visible: Qt.platform.os === ("android" || "ios")? true : false
        onSwitchToNextWagon:
        {
            terminus.buttonInput(3)
        }
        onSwitchToPreviousWagon:
        {
            terminus.buttonInput(4)
        }
    }

    OrientationSensor
    {
        id: orientationSensor
        dataRate: 50
        active: true
        onReadingChanged:
        {
            if (reading.orientation === OrientationReading.LeftUp)
            {
                gyro.orientation = -1
            }
            if (reading.orientation === OrientationReading.RightUp)
            {
                gyro.orientation = 1
            }
        }
    }

    Gyroscope
    {
        id: gyro
        dataRate: 50
        active: true

        property int orientation: 1

        onReadingChanged:
        {
            terminus.moveInput(
                               2
                             , gyro.reading.x * -orientation
                             , gyro.reading.y * orientation)
        }
    }
}

