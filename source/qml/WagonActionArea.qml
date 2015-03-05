import QtQuick 2.4
import Game 1.0

/*
 * Displays the ActionButton (Fire, Repair, Faster) and the surrounding ActionArea
 */

Item
{    
    signal fire

    id: actionArea
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: parent.width / 4
    height: parent.width / 8

    property Game game: parent.game
    property int currentWagon: game.qmlData["PlayerTrain"]["currentWagon"]
    property bool isDisabled: game.qmlData["PlayerTrain"]["wagons"][currentWagon]["isDisabled"]

    Image
    {
        source: "qrc:/data/ActionArea.png"
        anchors.fill: parent
    }

    Item
    {
        id: actionButton
        anchors.fill: parent

        Image
        {
            source: "qrc:/data/ActionButton.png"
            anchors.fill: parent
        }

        MultiPointTouchArea
        {
            id: primaryAction
            anchors.fill: parent

            onReleased:
            {
                if (!isDisabled){fire()}
            }
        }

        Text
        {
            id: actionButtonText
            text: isDisabled? "Destroyed" : "Fire"
            font.family: "Helvetica"
            font.pointSize: 36
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

