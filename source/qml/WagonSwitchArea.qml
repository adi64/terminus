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
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.05
        width: parent.width * 0.4
        height: parent.height * 0.9

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
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: parent.width * 0.05
        width: parent.width * 0.45
        height: parent.height * .9

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

