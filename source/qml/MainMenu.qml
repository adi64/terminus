import QtQuick 2.3

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
        id: button0
        posNum: 0
        buttonText: "Singleplayer"
        MouseArea
        {
            anchors.fill: parent
            onPressed:
            {
                button0.buttonText = "Loading..."
            }
            onReleased:
            {
                loader.setSource("qrc:/source/qml/Game.qml", { "loader": loader, "network": false })
            }
        }
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
        buttonText: "Settings"
        loadSource: "qrc:/source/qml/Settings.qml"
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

