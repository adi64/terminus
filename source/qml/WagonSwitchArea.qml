import QtQuick 2.3

Rectangle
{
    id: wagonSwitchArea
    color: "transparent"
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.margins: parent.height * 0.01
    width: parent.width * 0.25
    height: parent.height * 0.3

    signal switchToNextWagon
    signal switchToPreviousWagon

    Rectangle
    {
        id: wagonSwitchLeft
        color: "transparent"
        opacity: 0.8
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        width: parent.width * 0.45
        height: parent.height

        Image
        {
            id: leftArrow
            source: "/data/arrow.png"
            anchors.fill: parent
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

    Rectangle
    {
        id: wagonSwitchRight
        color: "transparent"
        opacity: 0.8
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        width: parent.width * 0.45
        height: parent.height

        Image
        {
            id: rightArrow
            source: "/data/arrow2.png"
            anchors.fill: parent
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

