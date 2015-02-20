import QtQuick 2.3

Rectangle
{
    id: actionArea
    color: "transparent"
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: parent.width / 4
    height: parent.width * 3 / 16

    signal charge
    signal fire

    Image
    {
        source: "qrc:/data/ActionArea.png"
        anchors.fill: parent
    }

    Rectangle
    {
        id: actionButton
        color: "transparent"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width * 0.9
        height: parent.height * 0.9

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

