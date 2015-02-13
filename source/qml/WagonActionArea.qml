import QtQuick 2.3

Rectangle
{
    id: actionArea
    color: "transparent"
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.margins: parent.height * 0.01
    width: parent.width * 0.2
    height: parent.height * 0.3

    signal charge
    signal fire

    Rectangle
    {
        id: actionButton
        color: "steelblue"
        opacity: 0.8
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width * 0.6
        height: parent.height

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

    Rectangle
    {
        id: actionStatusBar
        color: "grey"
        opacity: 0.8
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width * 0.3
        height: parent.height

        Rectangle
        {
            id: actionStatusBarProgress
            color: "steelblue"
            opacity: 0.8
            property real actionfactor: 0.0
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: parent.height * actionfactor
        }
    }
}

