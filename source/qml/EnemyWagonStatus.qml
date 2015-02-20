import QtQuick 2.3

/*
 * Shows health, type and optionally cooldown of an enemy wagon
 */

Rectangle{
    id: enemyWagonStatus

    property int wagonIndex
    property int totalWagons
    property real health

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: (parent.width / totalWagons * 0.05) + (parent.width / totalWagons * wagonIndex)
    width: parent.width / totalWagons * 14 / 16
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

