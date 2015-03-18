import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

/*
 * main.qml
 * The main Item consists of:
 * + a fullscreen Item
 * + a loader
 * The Loader is responsible for loading the various game states. The
 * default state is the MainMenu. Loaded items should all contain a
 * property loader and be passed a reference to the main loader.
 *
 */

Item
{
    width: Screen.width
    height: Screen.height

    Loader
    {
        id: loader
        anchors.fill: parent
        focus: true
    }

    Component.onCompleted:
    {
        loader.setSource("qrc:/source/qml/MainMenu.qml", { "loader": loader })
    }
}
