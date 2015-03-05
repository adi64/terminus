import QtQuick 2.4

Rectangle
{
    property string loadSource: ""
    property string buttonText: "buttonText"
    property int posNum: 1

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.topMargin: (parent.height * posNum * 4 / 32) + (parent.height * 3 / 16)
    width: parent.width / 2
    height: parent.height * 3 / 32
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
            loader.setSource(loadSource, { "loader": loader })
        }
    }
}

