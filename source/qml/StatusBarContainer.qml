import QtQuick 2.3

Item
{
    id: statusBarContainer
    anchors.fill: parent

    EnemyStatusBar
    {
        id: enemyStatusBar
    }

    PlayerStatusBar
    {
        id: playerStatusBar
    }
}
