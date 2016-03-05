import QtQuick 2.5
import Game 1.0

/*
 * StatusBarContainer.qml
 * The StatusBarContainer Item consists of:
 * + EnemyStatusBar (see EnemyStatusBar.qml)
 * + PlayerStatusBar (see PlayerStatusBar.qml)
 * This is a container for both status bars. It will use
 * the parents game reference and isReversed property and
 * save it, since its children will access these as well.
 *
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
