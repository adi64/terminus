import QtQuick 2.5

/*
 * MainMenu.qml
 * The MainMenu consists of:
 * + the background image
 * + a headline
 * + a number of MenuButtons
 * The Item will fill the window provided by the parent. The loader
 * should be set at the creation of the MainMenu as it will change
 * the source according to what button is pressed.
 *
 */

Item {
    id: mainMenu
    anchors.fill: parent

    property var train: [1];
    property Loader loader

    Image {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline {
        text: "TERMINUS"
    }

    SimpleButton {
        posNum: 0
        buttonText: "Singleplayer"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                parent.buttonText = "Loading..."
            }
            onReleased: {
                var source = "qrc:/source/qml/Game.qml";
                var params = { "loader": loader, "network": false, "train": train };

                if (train.length < 2) {
                    loader.setSource("qrc:/source/qml/SetupMenu.qml", { "loader": loader, "loaderSource": source, "loaderParams": params });
                } else {
                    loader.setSource(source, params);
                }

            }
        }
    }

    SimpleButton{
        posNum: 1
        buttonText: "Multiplayer"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                parent.buttonText = "Loading..."
            }
            onReleased: {
                var source = "qrc:/source/qml/MultiplayerMenu.qml";
                var params = { "loader": loader, "train": train };

                if (train.length < 2) {
                    loader.setSource("qrc:/source/qml/SetupMenu.qml", { "loader": loader, "loaderSource": source });
                } else {
                    loader.setSource(source, { "loader": loader });
                }
            }
        }
    }

    SimpleButton {
        posNum: 2
        buttonText: "Train Setup"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                parent.buttonText = "Loading..."
            }
            onReleased: {
                loader.setSource("qrc:/source/qml/SetupMenu.qml", { "loader": loader, "loaderSource": "qrc:/source/qml/MainMenu.qml" });
            }
        }
    }

    SimpleButton {
        posNum: 3
        buttonText: "Quit"
        visible: !(Qt.platform.os === "android" || Qt.platform.os === "ios")

        MouseArea {
            anchors.fill: parent
            onReleased: {
                Qt.quit()
            }
        }
    }
}

