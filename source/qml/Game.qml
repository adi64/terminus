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

        property bool isNetworkGame: false
        property bool isHost: true
        property string ip

        function winGame()
        {
            loader.setSource("qrc:/source/qml/Victory.qml", { "loader": loader })
        }
        function loseGame()
        {
            loader.setSource("qrc:/source/qml/Defeat.qml", { "loader": loader })
        }
        function loadUI()
        {
            uiLoader.setSource("qrc:/source/qml/UserInterface.qml", { "game": terminus })
        }

        Component.onCompleted:
        {
            if(network)
            {
                if(host)
                {
                    hostNetworkGame()
                }
                else
                {
                    joinNetworkGame()
                }
            }
            else
            {
                startLocalGame()
            }
        }

        Loader
        {
            id: uiLoader
            anchors.fill: parent
            focus: true
        }
    }
}
