import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtSensors 5.3
import Game 1.0

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

            /*onChargeChanged:
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
            }*/

            Keys.onPressed:
            {
                terminus.userInterface.keyPressEvent(event.key)
                event.accepted = true
            }
            Keys.onReleased:
            {
                terminus.userInterface.keyReleaseEvent(event.key)
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
                    terminus.userInterface.mouseMoveEvent(mouse.x, mouse.y)
                }
            }

            Reticle{}

            StatusBarContainer{}

            WagonActionArea
            {
                //visible: Qt.platform.os === ("android" || "ios")? true : false
                onCharge:
                {
                    terminus.userInterface.touchChargeFire()
                }
                onFire:
                {
                    terminus.userInterface.touchFire()
                }
            }

            WagonSwitchArea
            {
                //visible: Qt.platform.os === ("android" || "ios")? true : false
                onSwitchToNextWagon:
                {
                    terminus.userInterface.switchToNextWagon()
                }
                onSwitchToPreviousWagon:
                {
                    terminus.userInterface.switchToPreviousWagon()
                }
            }

            OrientationSensor
            {
                id: orientation
                dataRate: 50
                active: true
                onReadingChanged:
                {
                    if (reading.orientation === OrientationReading.LeftUp)
                    {
                        gyro.orientation_multiplier = 2
                    }
                    if (reading.orientation === OrientationReading.RightUp)
                    {
                        gyro.orientation_multiplier = -2
                    }
                }
            }

            Gyroscope
            {
                id: gyro
                dataRate: 100
                active: true

                property int orientation_multiplier: -2

                onReadingChanged:
                {
                    terminus.userInterface.gyroMoveEvent(gyro.reading.x * orientation_multiplier
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
