import QtQuick 2.3
import Game 1.0

/*
 * Container element for EnemyStatusBar and PlayerStatusBar
 */

Item
{
    id: statusBarContainer
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    height: parent.height
    width: parent.width * 9 / 16

    property QMLTrain enemyTrain
    property QMLTrain playerTrain

    EnemyStatusBar
    {
        id: enemyStatusBar
        train: enemyTrain
    }

    PlayerStatusBar
    {
        id: playerStatusBar
        train: playerTrain
    }
}
