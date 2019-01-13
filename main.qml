import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

ApplicationWindow {
    id: root            //We set the ApplicationWindow id: root so our Python code has something to refer to
    visible: true
    width: 800
    height: 480
    color: "#000000"
    property alias root: root       //for Python
    title: "Solar Gators Gauge"

    //Our data is "stored" here
    //Properties must begin with lowercase
    property real speedVal                      : 20
    property real runtimeVal                    : 3600


    /*
        QML JavaScript Updater
        This updates the data in QML from its slot

    */

    function updateSpeed(speed){
        speedVal            = speed;

        clockText.set();
    }

    /*
        QML Slot
        This takes the data from the Python code
    */

    function onSpeedUpdate(speed){
        updateSpeed(speed)
    }


    //Logo
    Image {
        id: logo
        x: 305
        y: -22
        width: 200
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "Gauge/logo.png"
    }


    //Time
    Item {
        id: time

        Image {
            id: timeSymbol
            x: 8
            y: 5
            width: 50
            height: 50
            fillMode: Image.PreserveAspectFit
            source: "Gauge/timer.png"

            Text {
                id: runtimeValue
                x: 209
                y: 1
                color: "#ffffff"
                text: {
                    var minutes = Math.floor(runtimeVal/60);
                    var seconds = runtimeVal - (minutes * 60);
                    if(seconds < 10) {
                        seconds = "0" + seconds;
                    }
                    return minutes + 'm ' + seconds + 's';
                }
                anchors.right: parent.right
                anchors.rightMargin: -159
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 40
            }

        }

        Text {
            id: clockText
            x: 794
            y: 0
            color: "#ffffff"
            anchors.right: parent.right
            anchors.rightMargin: -794
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -60
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 50
            function set() {
                clockText.text = Qt.formatTime(new Date(), "h:mm A");
            }
        }

        Rectangle {
            id: topLine
            x: 0
            y: 60
            width: 800
            height: 10
            color: "#ffffff"
        }
    }//end Time


    //Speed
    Item {
        id: speed
        transformOrigin: Item.Center
        CircularGauge {
            id: speedGauge
            x: -54
            y: 94
            width: 900
            height: 700
            stepSize: 0.001

            minimumValue: 0
            maximumValue: 50

            style: CircularGaugeStyle {
                minimumValueAngle: -90
                maximumValueAngle: 90

                minorTickmarkCount: 9
            }

            value: speedVal

        }

    } //end Speed


}//end ApplicationWindow

