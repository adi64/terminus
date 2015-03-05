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

        text: "TERMINUS"
        font.family: "Helvetica"
        font.pointSize: 150
    }

    MenuButton
    {
        posNum: 1
        buttonText: "Singleplayer"
        loadSource: "qrc:/source/qml/Game.qml"
    }

    MenuButton
    {
        posNum: 2
        buttonText: "Multiplayer"
        loadSource: "qrc:/source/qml/MultiplayerMenu.qml"
    }

    MenuButton
    {
        posNum: 3
        buttonText: "Quit"
        visible: Qt.platform.os === ("android" || "ios")? false : true

        MouseArea
        {
            anchors.fill: parent
            onReleased:
            {
                Qt.quit()
            }
        }
    }
}

