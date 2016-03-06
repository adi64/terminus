import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import Game 1.0

/*
 * Game.qml
 * The Game Item consists of:
 * + a Game element (see game.h)
 * + a Loader which is for enabling and disabling the UI
 * The Game is the main component that connects C++ and QML. The
 * QML side provides management to access different states such as
 * the Victory and Defeat screen. It also offers a method to load/unload
 * the UserInterface element.
 * The Game holds a reference to the main-Loader and knows if it is a
 * network game, the host and the ip address it should connect to. It also
 * contains the keyboard focus and sends keyboard events to C++ as well as
 * containing the MouseArea that is responsible for camera and shooting
 * to be able to navigate while the UI is disabled.
 *
 */

Item
{
    anchors.fill: parent

    property Loader loader
    property var playerTrain: []
    property var enemyTrain: [1, 2, 2, 2, 2, 2, 3, 2]
    property bool network
    property bool host
    property string ip

    Game
    {
        id: terminus
        anchors.fill: parent
        focus: true

        function winGame()
        {
            loader.setSource("qrc:/source/qml/Victory.qml", { "loader": loader })
        }
        function loseGame()
        {
            loader.setSource("qrc:/source/qml/Defeat.qml", { "loader": loader })
        }
        function stopGame()
        {
            loader.setSource("qrc:/source/qml/MainMenu.qml", { "loader": loader })
        }
        function loadUI(ui, isReversed)
        {
            uiLoader.setSource(ui, { "game": terminus, "isReversed": isReversed })
        }


        Component.onCompleted:
        {
            var data = {
                playerTrain: [],
                enemyTrain: []
            };

            playerTrain.forEach(function (wagonType) {
                data.playerTrain.push({ type: wagonType });
            });
            enemyTrain.forEach(function (wagonType) {
                data.enemyTrain.push({ type: wagonType });
            });

            writeQmlData(data);

            if(network && host)
            {
                hostNetworkGame();
            }
            else if(network && !host)
            {
                joinNetworkGame(ip);
            }
            else
            {
                startLocalGame();
            }
        }

        Keys.onPressed:
        {
            terminus.keyInput(event.key);
            event.accepted = true;
        }

        MouseArea
        {
            id: mouseCamera
            anchors.fill: parent
            cursorShape: "BlankCursor"
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton
            enabled: Qt.platform.os === "android" || Qt.platform.os === "ios"? false : true
            onPositionChanged:
            {
                // make sure the mouse is actually a mouse and not someone using touch
                if (containsMouse) {
                    if (Qt.platform.os !== "android" && Qt.platform.os !== "ios") {
                        terminus.moveInput(1, mouse.x, mouse.y);
                    }
                }
            }
            onReleased:
            {
                // 6 is the int value for the button value mouseButton (see eventhandler.h)
                terminus.buttonInput(6);
            }
        }

        Loader
        {
            id: uiLoader
            anchors.fill: parent
        }
    }
}
