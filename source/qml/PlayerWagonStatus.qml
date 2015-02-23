import QtQuick 2.3
import Game 1.0

/*
 * Shows health, type and optionally cooldown of a player wagon
 */

Rectangle
{
    id: playerWagonStatus

    property QMLWagon wagon
    property int wagonIndex
    property int totalWagons
    property int activeWagon: -1
    property real health: 1.0

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: (parent.width / totalWagons / 16) + (parent.width / totalWagons * wagonIndex)
    width: parent.width / totalWagons * 7 / 8
    height: parent.height * 6 / 8

    Rectangle
    {
        id: playerWagonMaxHealth
        anchors.fill: parent
        border.width: activeWagon === wagonIndex? 2 : 0
        border.color: "yellow"
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
