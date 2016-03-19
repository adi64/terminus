import QtQuick 2.5

Item {
    id: setupMenu
    anchors.fill: parent

    property Loader loader
    property string loaderSource: ""
    property var loaderParams: ({})
    property var train: [1]

    function trainToString() {
        var trainString = "";

        train.forEach(function (wagon) {
            switch (wagon) {
            case 1:
                trainString += "E "; break;
            case 2:
                trainString += "W "; break;
            case 3:
                trainString += "R "; break;
            default:
                break;
            }
        })

        return trainString;
    }

    Image {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline {
        text: "Setup"
    }

    SimpleButton {
        id: trainButton
        posNum: 0
        buttonText: trainToString()
    }

    SimpleButton {
        posNum: 1
        buttonText: "Add Weapon Wagon"

        MouseArea {
            anchors.fill: parent
            onReleased: {
                if (train.length < 7) {
                    train.push(2);
                    trainButton.buttonText = trainToString();
                }
            }
        }
    }

    SimpleButton {
        posNum: 2
        buttonText: "Add Repair Wagon"

        MouseArea {
            anchors.fill: parent
            onReleased: {
                if (train.length < 7) {
                    train.push(3);
                    trainButton.buttonText = trainToString();
                }
            }
        }
    }

    SimpleButton {
        posNum: 3
        buttonText: "Confirm"

        MouseArea {
            anchors.fill: parent
            onPressed: {
                trainButton.buttonText = "Loading...";
            }
            onReleased: {
                loaderParams.loader = loader;
                loaderParams.train = train;
                loader.setSource(loaderSource, loaderParams);
            }
        }
    }
}
