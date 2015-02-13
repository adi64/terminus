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
                ui.keyPressEvent(event.key)
                event.accepted = true
            }
            Keys.onReleased:
            {
                ui.keyReleaseEvent(event.key)
                event.accepted = true
            }

            Reticle{}

            StatusBarContainer{}

            WagonActionArea
            {
                onCharge:
                {
                    ui.touchChargeFire()
                }
                onFire:
                {
                    ui.touchFire()
                }
            }

            WagonSwitchArea
            {
                onSwitchLeft:
                {
                    ui.switchWagonLeft()
                }
                onSwitchRight:
                {
                    ui.switchWagonRight()
                }
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
