import QtQuick 2.4

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
        buttonText: "back"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }

    /*Rectangle
    {
        anchors.top: parent.top
        anchors.topMargin: parent.height / 2
        anchors.horizontalCenter: parent.horizontalCenter
        color: "grey"

        width: parent.width / 2

        TextInput
        {
            anchors.fill: parent
            text: "127.0.0.1:1337"
        }
    }*/
}
