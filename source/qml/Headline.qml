import QtQuick 2.3

/*
 * Headline for
 */

Item
{
    property string text

    anchors.top: parent.top
    anchors.topMargin: parent.width / 32
    anchors.horizontalCenter: parent.horizontalCenter
    height: parent.width * 3 / 32
    width: parent.width

    Text
    {
        id: headline
        anchors.centerIn: parent
        text: parent.text
        font.family: "Helvetica"
        font.pointSize: 130
        color: "white"
    }
}

