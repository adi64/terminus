import QtQuick 2.3
import Game 1.0

/*
 * WagonActionArea.qml
 * The WagonActionArea Item consists of:
 * + a background image
 * + a button with an image
 * + a MultiPointTouchArea for the button
 * + a Text element on the button
 * The WagonActionArea displays a button that calls the primaryAction
 * for the current wagon. The text is dependant on wagonType, isDisabled
 * and cooldown.
 *
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
    property int wagonType: game.qmlData["PlayerTrain"]["wagons"][currentWagon]["type"]
    property real cooldown: game.qmlData["PlayerTrain"]["wagons"][currentWagon]["currentCooldown"]

    function setText()
    {
        if(isDisabled)
        {
            return "Destroyed";
        }

        if(cooldown != 1)
        {
            return "Reloading";
        }

        switch(wagonType)
        {
        case 1:
            return "Faster"; //engine
        case 2:
            return "Fire"; //weapon
        case 3:
            return "Repair"; //repair
        default:
            return "No action"; //invalid
        }
    }

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
            text: setText()
            font.family: "Helvetica"
            font.pointSize: 36
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

