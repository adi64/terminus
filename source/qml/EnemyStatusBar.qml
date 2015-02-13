import QtQuick 2.3

Rectangle
{
    id: enemyStatus
    color: "white"

    property int wagons: 11

    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.margins: parent.height * 0.01
    width: parent.width * 0.5
    height: parent.height * 0.03

    Rectangle
    {
        id: enemyStatusContainer
        color: "transparent"

        anchors.fill: parent

        EnemyWagonStatus
        {
            id: ewagon0
            wagonIndex: 0
        }
        EnemyWagonStatus
        {
            id: ewagon1
            wagonIndex: 1
        }
        EnemyWagonStatus
        {
            id: ewagon2
            wagonIndex: 2
        }
        EnemyWagonStatus
        {
            id: ewagon3
            wagonIndex: 3
        }
        EnemyWagonStatus
        {
            id: ewagon4
            wagonIndex: 4
        }
        EnemyWagonStatus
        {
            id: ewagon5
            wagonIndex: 5
        }
        EnemyWagonStatus
        {
            id: ewagon6
            wagonIndex: 6
        }
        EnemyWagonStatus
        {
            id: ewagon7
            wagonIndex: 7
        }
        EnemyWagonStatus
        {
            id: ewagon8
            wagonIndex: 8
        }
        EnemyWagonStatus
        {
            id: ewagon9
            wagonIndex: 9
        }
    }
}


