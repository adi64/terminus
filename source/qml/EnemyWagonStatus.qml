import QtQuick 2.3

Rectangle{
    id: enemyWagonStatus

    property int wagonIndex

    anchors.top: parent.top
    anchors.right: parent.right
    anchors.rightMargin: (parent.parent.width * 0.035) * wagonIndex
    width: parent.parent.width * 0.03
    height: parent.height
    color: [0, 3, 7].indexOf(wagonIndex) > -1? (' + i + ' !== 0? "purple" : "orange") : "blue"
}

