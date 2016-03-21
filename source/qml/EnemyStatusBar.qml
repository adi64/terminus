import QtQuick 2.5
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
    property var train: game.qmlData.enemyTrain
    property bool isReversed: parent.isReversed

    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    width: parent.width
    height: parent.width / 16

    function createWagonStatus(wagonIndex) {
        var component = Qt.createComponent("qrc:/source/qml/EnemyWagonStatus.qml");
        var status = component.createObject(playerStatusContainer, { "wagonIndex": wagonIndex })
    }

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

        Component.onCompleted: {
            train.forEach(function (wagon, index) {
                createWagonStatus(index);
            });
        }
    }
}

