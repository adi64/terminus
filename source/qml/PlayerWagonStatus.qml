import QtQuick 2.3

Rectangle{
    id: playerWagonStatus

    property int wagonIndex
    property int totalWagons
    property int activeWagon: -1

    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: (parent.width / totalWagons * 0.05) + (parent.width / totalWagons * wagonIndex)
    width: parent.width / totalWagons * 0.9
    height: parent.height * 0.8
    border.width: activeWagon === wagonIndex? 2 : 0
    border.color: "yellow"
    color: [0, 3, 6].indexOf(wagonIndex) > -1? (wagonIndex !== 0? "purple" : "orange") : "blue"
}
