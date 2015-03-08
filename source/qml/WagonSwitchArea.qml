import QtQuick 2.3
import Game 1.0

/*
 * Displays WagonSwitchButtons and the container element WagonArea
 */

Item
{
    signal switchToNextWagon
    signal switchToPreviousWagon

    id: wagonSwitchArea
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: parent.width / 4
    height: parent.width / 8

    property Game game: parent.game
    property int totalWagons: game.qmlData["PlayerTrain"]["wagons"].length
    property int currentWagon: game.qmlData["PlayerTrain"]["currentWagon"]
    property bool isReversed: parent.isReversed
    property bool leftVisible: isReversed? currentWagon > 0 : currentWagon + 1 < totalWagons
    property bool rightVisible: isReversed? currentWagon + 1 < totalWagons : currentWagon > 0

    Image
    {
        source: "qrc:/data/SwitchArea.png"
        anchors.fill: parent
    }

    Item
    {
        id: wagonSwitchLeft
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width / 2
        height: parent.height

        Image
        {
            id: nextButton
            source: "qrc:/data/NextButton.png"
            anchors.fill: parent
            visible: leftVisible
        }

        MultiPointTouchArea
        {
            id: switchLeft
            anchors.fill: parent
            onReleased:
            {
                wagonSwitchArea.switchToNextWagon()
            }
        }
    }

    Item
    {
        id: wagonSwitchRight
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width / 2
        height: parent.height

        Image
        {
            id: prevButton
            source: "qrc:/data/PrevButton.png"
            anchors.fill: parent
            visible: rightVisible
        }

        MultiPointTouchArea
        {
            id: switchRight
            anchors.fill: parent

            onReleased:
            {
                wagonSwitchArea.switchToPreviousWagon()
            }
        }
    }
}

