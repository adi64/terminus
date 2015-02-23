import QtQuick 2.3
import Game 1.0

/*
 * Shows container for multiple PlayerWagonStatus
 */

Rectangle
{
    id: playerStatus
    color: "transparent"

    property int wagons: 8
    property QMLTrain train

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

        PlayerWagonStatus
        {
            id: ewagon0
            wagonIndex: 0
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon1
            wagonIndex: 1
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon2
            wagonIndex: 2
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon3
            wagonIndex: 3
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon4
            wagonIndex: 4
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon5
            wagonIndex: 5
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon6
            wagonIndex: 6
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon7
            wagonIndex: 7
            wagon: train.wagonAt(wagonIndex)
            totalWagons: wagons
        }
    }
}

