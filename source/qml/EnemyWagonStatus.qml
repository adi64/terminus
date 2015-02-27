import QtQuick 2.3
import Game 1.0

/*
 * Shows health, type and optionally cooldown of a player wagon
 */

Rectangle
{
    id: playerWagonStatus

    signal refresh

    property int wagonIndex
    property Game game: parent.game
    property int totalWagons: game.qmlData["EnemyTrain"]["wagons"].length
    property int wagonType: game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["type"]
    property real health: game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["currentHealth"]
    property real maxHealth: game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["maxHealth"]
    property bool isDisabled: game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["isDisabled"]

    onRefresh:
    {
        health = game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["currentHealth"]
        isDisabled = game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["isDisabled"]

        playerWagonMaxHealth.color = setColor()
        playerWagonMaxHealth.border.width = (currentWagon === wagonIndex? 3 : 0)
        playerWagonCurrentHealth.color = parent.color
        playerWagonCurrentHealth.width = (parent.width * health / maxHealth)
    }

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: parent.width / totalWagons * (1 / 16 + wagonIndex + (8 - totalWagons) * 0.5)
    width: parent.width / totalWagons * 7 / 8
    height: parent.height * 6 / 8

    function setColor(opac)
    {
        if(isDisabled)
        {
            return "grey";
        }

        switch(wagonType)
        {
        case 1:
            return "#" + opac + "CC8014"; //orange
        case 2:
            return "#" + opac + "1480CC"; //blue
        case 3:
            return "#" + opac + "8014CC"; //purple
        default:
            return "#" + opac + "808080"; //grey
        }
    }

    Rectangle
    {
        id: playerWagonMaxHealth
        anchors.fill: parent
        color: setColor("40")

        Rectangle
        {
            id: playerWagonCurrentHealth
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * health / maxHealth
            height: parent.height
            color: setColor("FF")
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
