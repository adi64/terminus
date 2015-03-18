import QtQuick 2.3

/*
 * Defeat.qml
 * The Defeat Item consists of:
 * + a background image
 * + a Headline
 * + a MenuButton
 * Shows the Defeat screen which will be loaded by Game.
 * The button provides a way back to the MainMenu as well as
 * a motivational text.
 *
 */

Item
{
    id: defeatScreen
    anchors.fill: parent

    property Loader loader

    Image
    {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline
    {
        text: "Defeat"
    }

    MenuButton
    {
        posNum: 1
        buttonText: "I'll win next time!"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
