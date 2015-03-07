import QtQuick 2.3
import Game 1.0

/*
 * Container element for EnemyStatusBar and PlayerStatusBar
 */

Item
{
    id: statusBarContainer
    anchors.fill: parent

    property Game game: parent.game
    property bool isReversed: parent.isReversed

    EnemyStatusBar{}
    PlayerStatusBar{}
}
