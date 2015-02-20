import QtQuick 2.3

/*
 * Shows container for multiple EnemyWagonStatus
 */

Rectangle
{
    id: enemyStatus
    color: "transparent"

    property int wagons: 8

    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    width: parent.width
    height: parent.width / 9

    Image
    {
        source: "qrc:/data/EnemyStatus.png"
        anchors.fill: parent
    }

    Rectangle
    {
        id: enemyStatusContainer
        color: "transparent"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - (parent.width / 9)
        height: parent.height

        EnemyWagonStatus
        {
            id: ewagon0
            wagonIndex: 0
            totalWagons: wagons
        }
        EnemyWagonStatus
        {
            id: ewagon1
            wagonIndex: 1
            totalWagons: wagons
        }
        EnemyWagonStatus
        {
            id: ewagon2
            wagonIndex: 2
            totalWagons: wagons
        }
        EnemyWagonStatus
        {
            id: ewagon3
            wagonIndex: 3
            totalWagons: wagons
        }
        EnemyWagonStatus
        {
            id: ewagon4
            wagonIndex: 4
            totalWagons: wagons
        }
        EnemyWagonStatus
        {
            id: ewagon5
            wagonIndex: 5
            totalWagons: wagons
        }
        EnemyWagonStatus
        {
            id: ewagon6
            wagonIndex: 6
            totalWagons: wagons
        }
        EnemyWagonStatus
        {
            id: ewagon7
            wagonIndex: 7
            totalWagons: wagons
        }
    }
}


