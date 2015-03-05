import QtQuick 2.4

Rectangle
{
    id: mainMenu
    color: "darkgrey"
    anchors.fill: parent

    property Loader loader

    Text
    {
        id: headline
        anchors.top: parent.top
        anchors.topMargin: parent.height / 32
        anchors.horizontalCenter: parent.horizontalCenter

        text: "Multiplayer"
        font.family: "Helvetica"
        font.pointSize: 150
    }

    MenuButton
    {
        posNum: 1
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
