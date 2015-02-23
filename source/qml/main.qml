import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import Game 1.0

Item
{
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    /*
     * Initializes Game in an empty fullscreen Item
     * After initialization setUI is called to get a reference to UI Element
     */

    Game
    {
        id: terminus
        anchors.fill: parent

        UserInterfaceContainer
        {
            ui: terminus.userInterface()
        }
    } 


}
