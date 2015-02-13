import QtQuick 2.3

Rectangle
{
    id: playerStatus
    color: "white"

    property int wagons: 13

    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.margins: parent.height * 0.01
    width: parent.width * 0.5
    height: parent.height * 0.03

    Rectangle
    {
        id: playerStatusContainer
        color: "transparent"

        anchors.fill: parent

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
        PlayerWagonStatus
        {
            id: ewagon8
            wagonIndex: 8
        }
        PlayerWagonStatus
        {
            id: ewagon9
            wagonIndex: 9
        }
    }
}

