import QtQuick 2.3

Rectangle
{
    id: wagonSwitchArea
    color: "transparent"
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: parent.width / 4
    height: parent.width * 3 / 16


    signal switchToNextWagon
    signal switchToPreviousWagon

    Image
    {
        source: "qrc:/data/SwitchArea.png"
        anchors.fill: parent
    }

    Rectangle
    {
        id: wagonSwitchLeft
        color: "transparent"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width * 7 / 16
        height: parent.height

        Image
        {
            source: "qrc:/data/NextButton.png"
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
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width * 9 / 16
        height: parent.height

        Image
        {
            source: "qrc:/data/PrevButton.png"
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

