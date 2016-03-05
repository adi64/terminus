import QtQuick 2.5

/*
 * MenuButton.qml
 * A MenuButton consists of:
 * + an image
 * + the button text
 * + a mousearea
 * When creating a MenuButton the posNum should be set with a value
 * between 0 and 4. The buttonText can be set through the buttonText
 * Property. If the button loads a new gamestate, loadSource should be
 * set to the address of that .qml file.
 * With the current size settings the Image should be 1/16 the height of
 * the display and the same width. The button will be touch sensitive in
 * that same area. If you dont want the standard onPressed behavior, you
 * can add a MouseArea as a child to the specific instance of the MenuButton.
 *
 */

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

