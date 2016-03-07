import QtQuick 2.5
import Game 1.0

/*
 * EnemyWagonStatus.qml
 * The EnemyWagonStatus Item consists of:
 * + a health bar
 * + a cooldown bar
 * This element should be nested in a StatusBar element.
 * It will load various data provided by game.qmlData (see game.cpp).
 * The position is determined by the wagonIndex and totalWagons. Color
 * is dependant on wagonType and isDisabled. The health bar uses the
 * ratio of health/maxHealth and the cooldown bar uses (1 - cooldown).
 *
 */

Item
{
    id: playerWagonStatus

    property int wagonIndex
    property Game game: parent.game
    property int totalWagons: game.qmlData.enemyTrain.length
    property bool isReversed: parent.isReversed
    property int index: isReversed ? totalWagons - (wagonIndex + 1) : wagonIndex
    property real healthPercent: game.qmlData.enemyTrain[wagonIndex].currentHealth / game.qmlData.enemyTrain[wagonIndex].maxHealth

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: parent.width / 8 * (1 / 16 + index + (8 - totalWagons) / 2)
    width: (parent.width / 8) * (7 / 8)
    height: parent.height * 6 / 8
    visible: totalWagons > wagonIndex

    function setColor(opac)
    {
        if (game.qmlData.enemyTrain[wagonIndex].isDisabled)
        {
            return "grey";
        }

        switch (game.qmlData.enemyTrain[wagonIndex].type)
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

        Rectangle
        {
            id: playerWagonCurrentHealth
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * healthPercent
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
            width: parent.width * (1 - game.qmlData.enemyTrain[wagonIndex].cooldown)
            height: parent.height
            color: "grey"
        }
    }
}
