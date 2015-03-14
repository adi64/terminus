import QtQuick 2.3
import Game 1.0

/*
 * EnemyStatusBar.qml
 * The EnemyStatusBar Item consists of:
 * + a background image
 * + eight EnemyWagonStatuses
 * This is a container for EnemyWagonStatuses. It gets a
 * reference to Game and isReversed to provide to the children.
 * It takes the parent width to set its height to remain the same
 * aspect ratio on all devices.
 *
 */

Item
{
    property Game game: parent.game
    property bool isReversed: parent.isReversed

    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    width: parent.width
    height: parent.width / 16

    Image
    {
        source: "qrc:/data/EnemyStatus.png"
        anchors.fill: parent
    }

    Item
    {
        id: playerStatusContainer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: parent.width / 2
        height: parent.height / 2

        property Game game: parent.game
        property bool isReversed: parent.isReversed

        EnemyWagonStatus
        {
            id: ewagon0
            wagonIndex: 0
        }
        EnemyWagonStatus
        {
            id: ewagon1
            wagonIndex: 1
        }
        EnemyWagonStatus
        {
            id: ewagon2
            wagonIndex: 2
        }
        EnemyWagonStatus
        {
            id: ewagon3
            wagonIndex: 3
        }
        EnemyWagonStatus
        {
            id: ewagon4
            wagonIndex: 4
        }
        EnemyWagonStatus
        {
            id: ewagon5
            wagonIndex: 5
        }
        EnemyWagonStatus
        {
            id: ewagon6
            wagonIndex: 6
        }
        EnemyWagonStatus
        {
            id: ewagon7
            wagonIndex: 7
        }
    }
}

