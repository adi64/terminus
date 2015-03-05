import QtQuick 2.4
import Game 1.0

/*
 * Shows health, type and optionally cooldown of a player wagon
 */

Item
{
    id: playerWagonStatus

    property int wagonIndex
    property Game game: parent.game

    property int totalWagons: game.qmlData["EnemyTrain"]["wagons"].length
    property bool load: totalWagons > wagonIndex

    property int wagonType: load? game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["type"] : 0
    property real health: load? game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["currentHealth"] : 0
    property real maxHealth: load? game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["maxHealth"] : 0
    property real cooldown: load? game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["currentCooldown"] : 1
    property bool isDisabled: load? game.qmlData["EnemyTrain"]["wagons"][wagonIndex]["isDisabled"] : true

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: parent.width / 8 * (1 / 16 + wagonIndex + (8 - totalWagons) / 2)
    width: (parent.width / 8) * (7 / 8)
    height: parent.height * 6 / 8
    visible: load

    function setColor(opac)
    {
        if(isDisabled)
        {
            return "grey";
        }

        switch(wagonType)
        {
        case 1:
            return Qt.rgba(0.8, 0.48, 0.08, opac); //orange
        case 2:
            return Qt.rgba(0.08, 0.48, 0.8, opac); //blue
        case 3:
            return Qt.rgba(0.48, 0.08, 0.8, opac); //purple
        default:
            return Qt.rgba(0.4, 0.4, 0.4, opac); //grey
        }
    }

    Rectangle
    {
        id: playerWagonMaxHealth
        anchors.fill: parent
        color: setColor(0.4)

        Rectangle
        {
            id: playerWagonCurrentHealth
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * health / maxHealth
            height: parent.height
            color: setColor(1.0)
        }
    }

    Rectangle
    {
        id: playerWagonMaxCooldown
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height / 6
        color: "white"

        Rectangle
        {
            id: playerWagonCurrentCooldown
            anchors.bottom: parent.bottom
            width: parent.width * (1 - cooldown)
            height: parent.height
            color: "grey"
        }
    }
}
