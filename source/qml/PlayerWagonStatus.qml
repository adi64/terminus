import QtQuick 2.3
import Game 1.0

/*
 * Shows health, type and optionally cooldown of a player wagon
 */

Item
{
    id: playerWagonStatus

    property int wagonIndex
    property Game game: parent.game
    property int totalWagons: game.qmlData["PlayerTrain"]["wagons"].length
    property bool load: totalWagons > wagonIndex
    property bool isReversed: parent.isReversed
    property int index: isReversed? totalWagons - (wagonIndex + 1) : wagonIndex

    property int wagonType: load? game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["type"] : -1
    property real health: load? game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["currentHealth"] : 1
    property real maxHealth: load? game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["maxHealth"] : 1
    property bool isDisabled: load? game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["isDisabled"] : true
    property real cooldown: load? game.qmlData["PlayerTrain"]["wagons"][wagonIndex]["currentCooldown"] : 1
    property int currentWagon: load? game.qmlData["PlayerTrain"]["currentWagon"] : -1

    anchors.top: parent.top
    anchors.topMargin: parent.height * 1 / 8
    anchors.right: parent.right
    anchors.rightMargin: parent.width / 8 * (1 / 16 + index + ((8 - totalWagons) * 0.5))
    width: (parent.width / 8) * (7 / 8)
    height: parent.height * 3 / 8
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
            return Qt.rgba(0.5725, 0.4, 0.2353, opac); //engine
        case 2:
            return Qt.rgba(0.3216, 0.5882, 0.5647, opac); //weapon
        case 3:
            return Qt.rgba(0.6196, 0.7333, 0.2745, opac); //repair
        default:
            return Qt.rgba(0.0, 0.0, 0.0, opac); //invalid
        }
    }

    Rectangle
    {
        id: playerWagonMaxHealth
        anchors.fill: parent
        color: setColor(0.4)
        border.width: currentWagon === wagonIndex? 3 : 0
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
