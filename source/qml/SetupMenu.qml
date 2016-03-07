import QtQuick 2.5

Item {
    id: setupMenu
    anchors.fill: parent

    property Loader loader
    property var playerTrain: [1]
    property var enemyTrain: [1]
    property var activeTrain: playerTrain

    function trainToString() {
        var trainString = "";

        activeTrain.forEach(function (wagon) {
            switch (wagon) {
            case 1:
                trainString += "Engine "; break;
            case 2:
                trainString += "Weapon "; break;
            case 3:
                trainString += "Repair "; break;
            default:
                break;
            }
        })

        return trainString;
    }

    function activeTrainString() {
        if (playerTrain.length > 1) {
            return "Setup - Enemy Train";
        } else {
            return "Setup - Player Train";
        }
    }

    Image
    {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline
    {
        text: activeTrainString()
    }

    SimpleButton
    {
        id: trainButton
        posNum: 0
        buttonText: trainToString()
    }

    SimpleButton
    {
        posNum: 1
        buttonText: "Add Weapon Wagon"

        MouseArea
        {
            anchors.fill: parent
            onReleased:
            {
                activeTrain.push(2);
                trainButton.buttonText = trainToString();
            }
        }
    }

    SimpleButton
    {
        posNum: 2
        buttonText: "Add Repair Wagon"

        MouseArea
        {
            anchors.fill: parent
            onReleased:
            {
                activeTrain.push(3);
                trainButton.buttonText = trainToString();
            }
        }
    }

    SimpleButton
    {
        posNum: 3
        buttonText: "Start Game"

        MouseArea
        {
            anchors.fill: parent
            onPressed:
            {
                trainButton.buttonText = "Loading..."
            }
            onReleased:
            {
                if (playerTrain.length > 1 && enemyTrain.length > 1) {
                    loader.setSource("qrc:/source/qml/Game.qml", { "loader": loader, "network": false, "playerTrain": playerTrain, "enemyTrain": enemyTrain });
                } else if (playerTrain.length > 1) {
                    loader.setSource("qrc:/source/qml/SetupMenu.qml", { "loader": loader, "playerTrain": playerTrain, "enemyTrain": enemyTrain, "activeTrain": enemyTrain });
                } else {
                    loader.setSource("qrc:/source/qml/SetupMenu.qml", { "loader": loader });
                }
            }
        }
    }
}
