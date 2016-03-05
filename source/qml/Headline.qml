import QtQuick 2.5

/*
 * Headline.qml
 * The Headline Item consists of:
 * + a Text element
 * The Headline is a plain Text element that is
 * preformatted to be in the upper center of the screen.
 * The text of the headline can be set with the text
 * property.
 *
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

