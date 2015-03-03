import QtQuick 2.3

Rectangle {
    id: mainMenu
    anchors.fill: parent
    color: "darkgrey"

    property Loader loader

    Text
    {
        anchors.top: parent.top
        anchors.topMargin: parent.height / 16
        anchors.horizontalCenter: parent.horizontalCenter

        text: "TERMINUS"
        font.family: "Helvetica"
        font.pointSize: 72
    }

    MenuButton
    {
        position: 0
        buttonText: "Start Game!"
        loadSource: "qrc:/source/qml/Game.qml"
    }

    MenuButton
    {
        position: 1
        buttonText: "Also start game!"
        loadSource: "qrc:/source/qml/Game.qml"
    }

    MenuButton
    {
        position: 2
        buttonText: "Load the game now!!"
        loadSource: "qrc:/source/qml/Game.qml"
    }

}

