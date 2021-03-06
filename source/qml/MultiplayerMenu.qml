import QtQuick 2.5
import Game 1.0

/*
 * MultiplayerMenu.qml
 * A MultiplayerMenu consists of:
 * + a background image
 * + a headline
 * + an ipprovider (see ipprovider.h)
 * + a TextInput that looks similar to the MenuButtons
 * + multiple MenuButtons
 * The MultiplayerMenu works similar to the MainMenu. Main
 * difference is the TextInput that displays the current IP
 * address of the device. The entered address will be used
 * when joining a game that is hosted on a different device.
 *
 */

Item
{
    id: multiplayerMenu
    anchors.fill: parent

    property Loader loader;
    property var train: [1];

    Image
    {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline
    {
        text: "Multiplayer"
    }

    IPProvider
    {
        id: ip
    }

    Item
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.width * 5 / 32
        width: parent.width
        height: parent.width * 2 / 32

        Image
        {
            source: "qrc:/data/MenuButton.png"
            anchors.fill: parent
        }

        TextInput
        {
            id: inputBox
            anchors.centerIn: parent
            anchors.bottomMargin: parent.height / 10

            maximumLength: 15
            selectByMouse: true
            text: ip.localIP
            font.family: "Helvetica"
            font.pointSize: 50
            color: "white"
        }
    }


    MenuButton
    {
        id: button1
        posNum: 1
        buttonText: "Join Game"

        MouseArea
        {
            anchors.fill: parent
            onPressed:
            {
                button1.buttonText = "Loading..."
            }
            onReleased:
            {
                loader.setSource("qrc:/source/qml/Game.qml", { "loader": loader, "network": true, "ip": inputBox.text, "train": train })
            }
        }
    }

    MenuButton
    {
        id: button2
        posNum: 2
        buttonText: "Host Game"

        MouseArea
        {
            anchors.fill: parent
            onPressed:
            {
                button2.buttonText = "Loading..."
            }
            onReleased:
            {
                loader.setSource("qrc:/source/qml/Game.qml", { "loader": loader, "network": true, "host": true, "train": train })
            }
        }
    }

    MenuButton
    {
        posNum: 3
        buttonText: "Back"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
