import QtQuick 2.3

/*
 * Displays the ActionButton (Fire, Repair, Faster) and the surrounding ActionArea
 */

Item
{
    id: actionArea
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: parent.width / 4
    height: parent.width / 8

    signal charge
    signal fire

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

            onPressed:
            {
                charge()
                actionButton.opacity = 0.9
            }

            onReleased:
            {
                fire()
                actionButton.opacity = 0.8
            }
        }

        Text
        {
            id: actionButtonText
            text: "Fire!"
            font.family: "Helvetica"
            font.pointSize: 36
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

