import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import Game 1.0

Item
{
    anchors.fill: parent

    property Loader loader
    property bool network
    property bool host
    property string ip

    /*
     * Initializes Game in an empty fullscreen Item
     * After initialization setUI is called to get a reference to UI Element
     */

    Game
    {
        id: terminus
        anchors.fill: parent

        function winGame()
        {
            loader.setSource("qrc:/source/qml/Victory.qml", { "loader": loader })
        }
        function loseGame()
        {
            loader.setSource("qrc:/source/qml/Defeat.qml", { "loader": loader })
        }
        function loadUI(isReversed)
        {
            uiLoader.setSource("qrc:/source/qml/UserInterface.qml", { "game": terminus, "isReversed": isReversed })
        }

        Component.onCompleted:
        {
            /*if(network)
            {
                if(host)
                {
                    hostNetworkGame()
                }
                else
                {
                    joinNetworkGame(ip)
                }
            }
            else
            {
                startLocalGame()
            }*/
            uiLoader.setSource("qrc:/source/qml/UserInterface.qml", { "game": terminus })
        }

        Loader
        {
            id: uiLoader
            anchors.fill: parent
            focus: true
        }
    }
}
