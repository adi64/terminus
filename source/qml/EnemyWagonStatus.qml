import QtQuick 2.3
import Game 1.0

/*
 * Shows health, type and optionally cooldown of an enemy wagon
 */

Rectangle
{
    id: enemyWagonStatus

    property int wagonIndex
    property Game game: parent.game
    property int totalWagons: game.qmlData["EnemyTrain"]["wagons"].length
    property real health: game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["currentHealth"]
    property real maxHealth: game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["maxHealth"]

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    // initial offset + offset for prev wagons + offset for unused wagons
    anchors.rightMargin: parent.width / totalWagons * (1 / 16 + wagonIndex + (8 - totalWagons) * 0.5)
    width: parent.width / totalWagons * 7 / 8
    height: parent.height * 6 / 8

    function setColor()
    {
        var type = game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["type"]
        switch(type){
            case 1:
                return "orange";
            case 2:
                return "blue";
            case 3:
                return "purple";
            default:
                return "grey";
        }

    }

    Rectangle
    {
        id: playerWagonMaxHealth
        anchors.fill: parent
        color: setColor()

        Rectangle
        {
            id: playerWagonCurrentHealth
            anchors.bottom: parent.bottom
            width: parent.width * health / maxHealth
            height: parent.height
            color: parent.color
        }
    }

    Rectangle
    {
        id: playerWagonMaxCooldown
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height / 6

        Rectangle
        {
            id: playerWagonCurrentCooldown
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height / 6
        }
    }
}

