import QtQuick 2.3

Rectangle
{
    property string loadSource: ""
    property string buttonText: "buttonText"
    property int position: 0

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.topMargin: parent.height * (4 + position * 3) / 16
    width: parent.width / 4
    height: parent.height / 8
    color: "lightgrey"
    border.width: 2
    border.color: "black"

    Text
    {
        anchors.centerIn: parent
        text: buttonText
        font.family: "Helvetica"
        font.pointSize: 36
    }

    MouseArea
    {
        anchors.fill: parent
        onReleased:
        {
            loader.source = loadSource
        }
    }
}

