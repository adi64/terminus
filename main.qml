import QtQuick 2.3
import QtQuick.Controls 1.2
import terminus 1.0

Item {

    width: 1024
    height: 768

    Game {
        id: terminusGame
        focus: true
        Keys.onPressed: {
            terminusGame.handleKeyboardEvent(event.key)
            event.accepted = true
        }
    }

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }

    Text {
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "Terminus 0.0.1 Alpha"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
}
