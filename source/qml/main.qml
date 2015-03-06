import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

Item
{
    width: 1024
    height: 768

    MainMenu
    {
        id: mainmenu
        loader: loader
    }

    Loader
    {
        id: loader
        focus: true
        onLoaded:
        {
            mainmenu.visible = false
            mainmenu.enabled = false
        }
    }
}
