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
    property int totalWagons: game.qmlData["PlayerTrain"]["wagons"].length
    property real health: game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["currentHealth"]
    property real maxHealth: game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["maxHealth"]
    property int currentWagon: game.qmlData["PlayerTrain"]["currentWagon"]

    onRefresh:
    {
        health = game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["currentHealth"]
        currentWagon = game.qmlData["PlayerTrain"]["currentWagon"]

        playerWagonMaxHealth.border.width = (currentWagon === wagonIndex? 2 : 0)
        playerWagonCurrentHealth.width = (parent.width * health / maxHealth)
    }

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: (parent.width / totalWagons / 16) + (parent.width / totalWagons * wagonIndex)
    width: parent.width / totalWagons * 7 / 8
    height: parent.height * 6 / 8

    function setColor()
    {
        var type = game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["type"]
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
        color: setColor();
        border.width: currentWagon === wagonIndex? 2 : 0
        border.color: "yellow"

        Rectangle
        {
            id: playerWagonCurrentHealth
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.border.width
            anchors.left: parent.left
            anchors.leftMargin: parent.border.width
            width: parent.width * health / maxHealth - parent.border.width * 2
            height: parent.height - parent.border.width * 2
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
