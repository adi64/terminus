import QtQuick 2.3
import Game 1.0

/*
 * Shows container for multiple PlayerWagonStatus
 */

Rectangle
{
    id: playerStatus
    color: "transparent"

    property Game game: parent.game

    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    width: parent.width
    height: parent.width / 9

    Image
    {
        source: "qrc:/data/PlayerStatus.png"
        anchors.fill: parent
    }

    Rectangle
    {
        id: playerStatusContainer
        color: "transparent"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - (parent.width / 9)
        height: parent.height

        property Game game: parent.game

        PlayerWagonStatus
        {
            id: ewagon0
            wagonIndex: 0
        }
        PlayerWagonStatus
        {
            id: ewagon1
            wagonIndex: 1
        }
        PlayerWagonStatus
        {
            id: ewagon2
            wagonIndex: 2
        }
        PlayerWagonStatus
        {
            id: ewagon3
            wagonIndex: 3
        }
        PlayerWagonStatus
        {
            id: ewagon4
            wagonIndex: 4
        }
        PlayerWagonStatus
        {
            id: ewagon5
            wagonIndex: 5
        }
        PlayerWagonStatus
        {
            id: ewagon6
            wagonIndex: 6
        }
        PlayerWagonStatus
        {
            id: ewagon7
            wagonIndex: 7
        }
    }
}

