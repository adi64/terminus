import QtQuick 2.3

Item
{
    id: statusBarContainer
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    height: parent.height
    width: parent.width * 9 / 16

    EnemyStatusBar
    {
        id: enemyStatusBar
    }

    PlayerStatusBar
    {
        id: playerStatusBar
    }
}
