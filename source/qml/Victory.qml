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

        text: "Victory"
        font.family: "Helvetica"
        font.pointSize: 150
    }

    MenuButton
    {
        posNum: 1
        buttonText: "back"
        loadSource: "qrc:/source/qml/MainMenu.qml"
    }
}
