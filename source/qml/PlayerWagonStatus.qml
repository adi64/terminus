import QtQuick 2.3

Rectangle{
    id: playerWagonStatus

    property int wagonIndex

    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.rightMargin: (parent.parent.width * 0.035) * wagonIndex
    width: parent.parent.width * 0.03
    height: parent.height
    color: [0, 3, 6, 9].indexOf(wagonIndex) > -1? (' + i + ' !== 0? "purple" : "orange") : "blue"
}
