import QtQuick 2.5

/*
 * MainMenu.qml
 * The MainMenu consists of:
 * + the background image
 * + a headline
 * + a number of MenuButtons
 * The Item will fill the window provided by the parent. The loader
 * should be set at the creation of the MainMenu as it will change
 * the source according to what button is pressed.
 *
 */

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
        loadSource: "qrc:/source/qml/SetupMenu.qml"
    }

    MenuButton
    {
        posNum: 1
        buttonText: "Multiplayer"
        loadSource: "qrc:/source/qml/MultiplayerMenu.qml"
    }

    SimpleButton
    {
        posNum: 2
        buttonText: "Quit"
        visible: !(Qt.platform.os === "android" || Qt.platform.os === "ios")

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

