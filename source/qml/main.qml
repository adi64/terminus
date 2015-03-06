import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

Item
{
    width: 1024
    height: 768

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
