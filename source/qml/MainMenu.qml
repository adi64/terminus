import QtQuick 2.3

Rectangle {
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
        buttonText: "Start Game!"
        loadSource: "qrc:/source/qml/Game.qml"
    }

    MenuButton
    {
        posNum: 2
        buttonText: "Quit"

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

