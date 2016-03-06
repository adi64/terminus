import QtQuick 2.5

Item {
    id: setupMenu
    anchors.fill: parent

    property Loader loader
    property var train: [0]

    function trainToString() {
        var trainString = "";

        train.forEach(function (wagon) {
            switch (wagon) {
            case 0:
                trainString += "Engine "; break;
            case 1:
                trainString += "Weapon "; break;
            case 2:
                trainString += "Repair "; break;
            }
        })

        return trainString;
    }

    Image
    {
        source: "qrc:/data/MenuBackground.png"
        anchors.fill: parent
    }

    Headline
    {
        text: "SETUP - My Train"
    }

    SimpleButton
    {
        id: button0
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
                train.push(1);
                button0.buttonText = trainToString();
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
                train.push(2);
                button0.buttonText = trainToString();
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
                button0.buttonText = "Loading..."
            }
            onReleased:
            {
                loader.setSource("qrc:/source/qml/Game.qml", { "loader": loader, "network": false, "playerTrain": train })
            }
        }
    }
}
