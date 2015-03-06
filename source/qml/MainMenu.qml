import QtQuick 2.4

Item
{
    id: mainMenu
    anchors.fill: parent

    property Loader loader

    Image
    {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline
    {
        text: "TERMINUS"
    }

    MenuButton
    {
        posNum: 0
        buttonText: "Singleplayer"
        loadSource: "qrc:/source/qml/Game.qml"
    }

    MenuButton
    {
        posNum: 1
        buttonText: "Multiplayer"
        loadSource: "qrc:/source/qml/MultiplayerMenu.qml"
    }

    MenuButton
    {
        posNum: 2
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

