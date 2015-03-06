import QtQuick 2.4

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
        posNum: 0
        buttonText: "back"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
