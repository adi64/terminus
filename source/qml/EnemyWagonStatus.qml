import QtQuick 2.3

Rectangle{
    id: enemyWagonStatus

    property int wagonIndex
    property int totalWagons

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: (parent.width / totalWagons * 0.05) + (parent.width / totalWagons * wagonIndex)
    width: parent.width / totalWagons * 0.9
    height: parent.height * 0.8
    color: [0, 3, 7].indexOf(wagonIndex) > -1? (wagonIndex !== 0? "purple" : "orange") : "blue"
}

