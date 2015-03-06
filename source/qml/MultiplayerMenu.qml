import QtQuick 2.3
import Game 1.0

Item
{
    id: multiplayerMenu
    anchors.fill: parent

    property Loader loader

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
            id: button
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
        posNum: 1
        buttonText: "Join Game"
        loadSource: "qrc:/source/qml/MultiplayerMenu.qml"
    }

    MenuButton
    {
        posNum: 2
        buttonText: "Host Game"
        loadSource: "qrc:/source/qml/MultiplayerMenu.qml"
    }

    MenuButton
    {
        posNum: 3
        buttonText: "Back"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
