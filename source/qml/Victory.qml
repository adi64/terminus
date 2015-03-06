import QtQuick 2.4

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
        posNum: 0
        buttonText: "back"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
