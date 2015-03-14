import QtQuick 2.3

/*
 * Victory.qml
 * The Victory Item consists of:
 * + a background image
 * + a Headline
 * + a MenuButton
 * Shows the Victory screen which will be loaded by Game.
 * The button provides a way back to the MainMenu as well as
 * a motivational text.
 *
 */

Item
{
    id: victoryScreen
    anchors.fill: parent

    property Loader loader

    Image
    {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline
    {
        text: "Victory"
    }

    MenuButton
    {
        posNum: 1
        buttonText: "I'm the best!"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
