import QtQuick 2.3
import Game 1.0

/*
 * Shows container for multiple EnemyWagonStatus
 */

Rectangle
{
    id: enemyStatus
    color: "transparent"

    property Game game: parent.game

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

        property Game game: parent.game

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
    }
}


