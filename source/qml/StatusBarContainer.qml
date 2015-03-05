import QtQuick 2.4
import Game 1.0

/*
 * Container element for EnemyStatusBar and PlayerStatusBar
 */

Item
{
    id: statusBarContainer
    anchors.fill: parent

    property Game game: parent.game

    EnemyStatusBar{}
    PlayerStatusBar{}
}
