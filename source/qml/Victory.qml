import QtQuick 2.3

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
