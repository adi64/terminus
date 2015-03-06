import QtQuick 2.3

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

    MenuButton
    {
        posNum: 0
        buttonText: "Host Game"
        loadSource: "qrc:/source/qml/MultiplayerMenu.qml"
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
        buttonText: "Back"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
