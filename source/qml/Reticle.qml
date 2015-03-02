import QtQuick 2.3

/*
 * Creates a reticle out of 4 white bars with black borders
 */

Rectangle
{
    id: reticle
    color: "transparent"
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    property real factor: 10

    height: parent.height / 100 * factor
    width: parent.height / 100 * factor

    Rectangle
    {
        id: reticleRight
        color: "white"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        border.color: "black"
        border.width: 2
        height: 5
        width: parent.width * 0.1
    }
    Rectangle
    {
        id: reticleLeft
        color: "white"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        border.color: "black"
        border.width: 2
        height: 5
        width: parent.width * 0.1
    }
    Rectangle
    {
        id: reticleTop
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        border.color: "black"
        border.width: 2
        height: parent.width * 0.1
        width: 5
    }
    Rectangle
    {
        id: reticleBottom
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        border.color: "black"
        border.width: 2
        height: parent.width * 0.1
        width: 5
    }
}

