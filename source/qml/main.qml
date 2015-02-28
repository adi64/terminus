import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtSensors 5.3
import Game 1.0

/*
 * Interaction will sent a integer to identify the type of interaction
 *
 * enum InteractionType
 * {
 *      KeyboardPress = 0,
 *      KeyboardRelease = 1,
 *      MouseMovement = 2,
 *      TouchMovement = 3,
 *      GyroMovement = 4,
 *      NextWagonButton = 5,
 *      PrevWagonButton = 6,
 *      ActionButton = 7
 *  };
 */

Item
{
    width: Screen.width
    height: Screen.height

    /*
     * Initializes Game in an empty fullscreen Item
     * After initialization setUI is called to get a reference to UI Element
     */

    Game
    {
        id: terminus
        anchors.fill: parent

        /*
         * Initializes UserInterface
         * UserInterface is a container for all UI elements
         */
        Item
        {
            anchors.fill: parent
            focus: true

            property Game game: terminus

            Keys.onPressed:
            {
                terminus.keyInput(0, event.key)
                event.accepted = true
            }
            Keys.onReleased:
            {
                terminus.keyInput(1, event.key)
                event.accepted = true
            }

            MouseArea
            {
                id: mouseCamera
                anchors.fill: parent
                cursorShape: "BlankCursor"
                hoverEnabled: true
                enabled: Qt.platform.os === ("android" || "ios")? false : true
                onPositionChanged:
                {
                    terminus.moveInput(2, mouse.x, mouse.y)
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
                    terminus.buttonInput(7)
                }
            }

            WagonSwitchArea
            {
                id: switchArea
                //visible: Qt.platform.os === ("android" || "ios")? true : false
                onSwitchToNextWagon:
                {
                    terminus.buttonInput(5)
                }
                onSwitchToPreviousWagon:
                {
                    terminus.buttonInput(6)
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
                        gyro.orientation = 1
                    }
                    if (reading.orientation === OrientationReading.RightUp)
                    {
                        gyro.orientation = -1
                    }
                }
            }

            Gyroscope
            {
                id: gyro
                dataRate: 50
                active: true

                property int orientation: -1

                onReadingChanged:
                {
                    terminus.moveInput(
                                       4
                                     , gyro.reading.x * 4 * orientation
                                     , gyro.reading.y * 4 * orientation)
                }
            }
        }
    } 
}
