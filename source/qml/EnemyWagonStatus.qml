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

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    // initial offset + offset for prev wagons + offset for unused wagons
    anchors.rightMargin: parent.width / totalWagons * (1 / 16 + wagonIndex + (8 - totalWagons) * 0.5)
    width: cellWidth * 14 / 16
    height: parent.height * 6 / 8

    Rectangle
    {
        id: playerWagonMaxHealth
        anchors.fill: parent
        color: [0, 3, 6].indexOf(wagonIndex) > -1? (wagonIndex !== 0? "purple" : "orange") : "blue"

        Rectangle
        {
            id: playerWagonCurrentHealth
            anchors.bottom: parent.bottom
            width: parent.width * health
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

