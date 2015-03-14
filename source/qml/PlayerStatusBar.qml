import QtQuick 2.3
import Game 1.0

/*
 * PlayerStatusBar.qml
 * The PlayerStatusBar Item consists of:
 * + a background image
 * + a cooldown bar for the current wagon
 * + a progress bar
 * + eight EnemyWagonStatuses
 * This is a container for PlayerWagonStatuses. It gets a
 * reference to Game and isReversed to provide to the children.
 * It takes the parent width to set its height to remain the same
 * aspect ratio on all devices.
 * Additionally it has a larger cooldown bar for the current wagon
 * and a progress bar. The required information are stored in game.qmlData
 * (see game.cpp).
 *
 */

Item
{
    id: playerStatus

    property Game game: parent.game
    property bool isReversed: parent.isReversed

    property int currentWagon: game.qmlData["PlayerTrain"]["currentWagon"]
    property real cooldown: game.qmlData["PlayerTrain"]["wagons"][currentWagon]["currentCooldown"]
    property real progress: game.qmlData["PlayerTrain"]["progress"]

    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    width: parent.width
    height: parent.width / 16

    Image
    {
        source: "qrc:/data/PlayerStatus.png"
        anchors.fill: parent
    }

    Rectangle
    {
        id: currentWagonCDBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 4 / 16
        width: parent.width * 17 / 32
        height: parent.height / 8
        color: "white"

        Rectangle
        {
            id: currentWagonCD
            anchors.bottom: parent.bottom
            width: parent.width * (1 - cooldown)
            height: parent.height
            color: "grey"
        }

        Text
        {
            id: cdBarText
            text: "Cooldown"
            font.family: "Helvetica"
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle
    {
        id: currentLevelProgressBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 1 / 16
        width: parent.width * 9 / 16
        height: parent.height / 8
        color: "white"

        Rectangle
        {
            id: currentLevelProgress
            anchors.bottom: parent.bottom
            width: parent.width * progress
            height: parent.height
            color: "grey"
        }

        Text
        {
            id: progressBarText
            text: "Level Progress"
            font.family: "Helvetica"
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Item
    {
        id: playerStatusContainer
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 2
        height: parent.height

        property Game game: parent.game
        property bool isReversed: parent.isReversed

        PlayerWagonStatus
        {
            id: ewagon0
            wagonIndex: 0
        }
        PlayerWagonStatus
        {
            id: ewagon1
            wagonIndex: 1
        }
        PlayerWagonStatus
        {
            id: ewagon2
            wagonIndex: 2
        }
        PlayerWagonStatus
        {
            id: ewagon3
            wagonIndex: 3
        }
        PlayerWagonStatus
        {
            id: ewagon4
            wagonIndex: 4
        }
        PlayerWagonStatus
        {
            id: ewagon5
            wagonIndex: 5
        }
        PlayerWagonStatus
        {
            id: ewagon6
            wagonIndex: 6
        }
        PlayerWagonStatus
        {
            id: ewagon7
            wagonIndex: 7
        }
    }
}

