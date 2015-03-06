import QtQuick 2.4

Item
{
    property string loadSource: ""
    property string buttonText: "buttonText"
    property int posNum: 1

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.topMargin: (parent.width * posNum * 3 / 32) + (parent.width * 5 / 32)
    width: parent.width
    height: parent.width * 2 / 32

    Image
    {
        source: "qrc:/data/MenuButton.png"
        anchors.fill: parent
    }

    Text
    {
        id: button
        anchors.centerIn: parent
        anchors.bottomMargin: parent.height / 10
        text: buttonText
        font.family: "Helvetica"
        font.pointSize: 50
        color: "white"
    }

    MouseArea
    {
        anchors.fill: parent
        onPressed:
        {
            button.text = "Loading..."
        }
        onReleased:
        {
            loader.setSource(loadSource, { "loader": loader })
        }
    }
}

