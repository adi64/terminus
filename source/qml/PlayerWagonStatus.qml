import QtQuick 2.3
import Game 1.0

/*
 * Shows health, type and optionally cooldown of a player wagon
 */

Rectangle
{
    id: playerWagonStatus

    property int wagonIndex
    property Game game: parent.game
    property int totalWagons: game.qmlData["PlayerTrain"]["wagons"].length
    property real health: game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["currentHealth"]
    property real maxHealth: game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["maxHealth"]

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: (parent.width / totalWagons / 16) + (parent.width / totalWagons * wagonIndex)
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
        border.width: 0// activeWagon === wagonIndex? 2 : 0
        border.color: "yellow"
        color: setColor();

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
