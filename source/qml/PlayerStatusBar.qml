import QtQuick 2.3

Rectangle
{
    id: playerStatus
    color: "transparent"

    property int wagons: 8

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
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon1
            wagonIndex: 1
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon2
            wagonIndex: 2
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon3
            wagonIndex: 3
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon4
            wagonIndex: 4
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon5
            wagonIndex: 5
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon6
            wagonIndex: 6
            totalWagons: wagons
        }
        PlayerWagonStatus
        {
            id: ewagon7
            wagonIndex: 7
            totalWagons: wagons
        }
    }
}

