import QtQuick 2.5
import QtSensors 5.3
import Game 1.0

/*
 * UserInterface.qml
 * The UserInterface Item consists of:
 * + a MultiPointTouchArea
 * + the Reticle
 * + the WagonSwitchArea
 * + the WagonActionArea
 * + an OrientationSensor
 * + a Gyroscope
 * The UI initializes the on-screen buttons and graphics as well as
 * starting the necessary sensors. It is not necessary when playing on
 * desktop (mouse and keyboard) but provides information that can be useful.
 * The game property has to be set when constructing a UserInterface.
 *
 * Interaction will sent a integer to identify the type of interaction
 * (see eventhandler.h)
 * enum InteractionType
 * {
 *      MOUSE_MOVEMENT = 1,
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

    property Game game
    property bool isReversed: false

    MultiPointTouchArea
    {
        id: touchCamera
        anchors.fill: parent
        enabled: Qt.platform.os === "android" || Qt.platform.os === "ios"? true : false
        touchPoints: [ TouchPoint { id: t } ]
        onTouchUpdated: game.touchInput(t.previousX, t.previousY, t.x, t.y)
        onReleased: if(-5 < (t.x - t.startX) < 5 && -5 < (t.y - t.startY) < 5) { game.buttonInput(5) }
    }

    Reticle{}

    StatusBarContainer{}

    WagonActionArea
    {
        id: actionArea
        onFire:
        {
            game.buttonInput(5)
        }
    }

    WagonSwitchArea
    {
        id: switchArea
        onSwitchToNextWagon:
        {
            isReversed? game.buttonInput(4) : game.buttonInput(3)
        }
        onSwitchToPreviousWagon:
        {
            isReversed? game.buttonInput(3) : game.buttonInput(4)
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
            game.moveInput(
                               2
                             , gyro.reading.x * -orientation
                             , gyro.reading.y * orientation)
        }
    }
}

