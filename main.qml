import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

ApplicationWindow {
    id: root            //We set the ApplicationWindow id: root so our Python code has something to refer to
    visible: true
    width: 800
    height: 480
    color: "#000000"
    title: "Solar Gators Gauge"

    //Our data is "stored" here
    property real voltageVal    : 0
    property real currentVal    : 0
    property real loadVal       : 0
    property real tempVal       : 0
    property real speedVal      : 0
    property real runtimeVal    : 0

    /*
        QML JavaScript Updater
        This updates the data in QML from its slot

    */
    function updateNormal(voltage, current, load, temp, speed, runtime) {
        voltageVal  = voltage;
        currentVal  = current;
        loadVal     = load;
        tempVal     = temp;
        speedVal    = speed;
        runtimeVal  = runtime;
        clockText.set();
    }

    /*
        QML Slot
        This takes the data from the Python code
    */
    function onNormalUpdate(voltage, current, load, temp, speed, runtime){
        updateNormal(voltage, current, load, temp, speed, runtime)
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
                        seconds = "0"+seconds;
                    }
                    return minutes+'m '+seconds+'s';
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


    //Battery
    Item {
        id: battery

        Image {
            id: batterySymbol
            x: 54
            y: 82
            width: 50
            height: 50
            fillMode: Image.PreserveAspectFit
            source: "Gauge/battery.png"

            Text {
                id: loadValue
                x: 88
                y: 8
                color: "#FFFFFF"
                text: loadVal.toFixed() + "%"
                anchors.right: parent.right
                anchors.rightMargin: -83
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 30
            }

            Gauge {
                id: voltageGauge
                x: -33
                y: 93
                height: 300
                minimumValue: 0
                maximumValue: 20

                value: voltageVal

                Text {
                    id: voltageValue
                    x: -7
                    y: -29
                    color: {
                        if (voltageVal < 12)
                            return "#FF0000";
                        else
                            return "#FFFFFF";
                    }
                    text: voltageVal.toFixed(1) + qsTr(" V")
                    lineHeight: 1
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 20
                }


            }

            Gauge {
                id: currentGauge
                x: 42
                y: 93
                height: 300
                minimumValue: 0
                maximumValue: 50

                value: currentVal

                tickmarkAlignment: Qt.AlignRight

                Text {
                    id: currentValue
                    x: 0
                    y: -29
                    color: {
                        if (currentVal < 12)
                            return "#FF0000";
                        else
                            return "#FFFFFF";
                    }
                    text: currentVal.toFixed(1) + qsTr(" A")
                    anchors.right: parent.right
                    anchors.rightMargin: -7
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 20
                }
            }
        }

        StatusIndicator {
            id: batteryError
            x: 266
            y: 80
            width: 40
            height: 40

            color: if(voltageVal <= 10 || voltageVal >= 15){
                       return "#FF0000"
                   }
                   else {
                       return "#00FF00"
                   }
            active: voltageVal > 0 ? true : false
        }
    }//end Battery




    //Temperature
    Item {
        id: temperature

        Image {
            id: tempSymbol
            x: 733
            y: 86
            width: 50
            height: 50
            fillMode: Image.PreserveAspectFit
            source: "Gauge/water.png"

            Text {
                id: tempValue
                x: -9
                y: 158
                color: {
                    if (tempVal>= 208)
                        return "#FF0000"
                    else if (tempVal >= 194)
                        return "#FF6600"
                    else if (tempVal >= 180)
                        return "#FFFFFF"
                    else if (tempVal >= 167)
                        return "#CCEEFF"
                    else
                        return "#9999FF"
                }
                text: tempVal.toFixed()+" F"
                anchors.right: parent.right
                anchors.rightMargin: -9
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 50
            }
        }

        CircularGauge {
            id: tempGauge
            x: 642
            y: -69
            width: 300
            height: 300
            visible: true
            stepSize: 0.001
            minimumValue: 0
            maximumValue: 250
            rotation: -90

            value: tempVal

            style: CircularGaugeStyle {
                minimumValueAngle: -90
                maximumValueAngle: 0
                tickmarkStepSize: 50

                tickmarkLabel: Text {
                    text: styleData.value
                    color: styleData.value >= 200 ? "#FF0000" : "#FFFFFF"
                    rotation: 90
                }

            }
        }
    }//end Temperature


    //Speed
    Item {
        id: speed
        transformOrigin: Item.Center
        CircularGauge {
            id: speedGauge
            x: 150
            y: 210
            width: 500
            height: 500
            stepSize: 0.001

            style: CircularGaugeStyle {
                minimumValueAngle: -90
                maximumValueAngle: 90

                tickmarkLabel:Text {
                    text: styleData.value
                    font.pixelSize: Math.max(6, outerRadius * 0.1) //default size
                    color: if(styleData.value >= 30 && styleData.value <= 70){
                               if(styleData.value >= 45 && styleData.value <= 55){
                                   return "#00FF00"
                               }
                               else {
                                   return "#FFFF00"
                               }
                           }
                           else {
                               return "#FFFFFF"
                           }
                }
            }

            value: speedVal

            Text {
                id: speedValue
                x: 199
                y: -75
                color: {
                    "#FFFFFF"
                }
                text: speedVal.toFixed(0)+" MPH"
                anchors.right: parent.right
                anchors.rightMargin: 157
                horizontalAlignment: Text.AlignRight
                font.pixelSize: 50
            }
        }

        StatusIndicator {
            id: optimumSpeed
            x: 500
            y: 80
            width: 40
            height: 40

            color: (speedVal >= 45 && speedVal <= 55) ? "#00FF00" : "#FFFF00"
            active: (speedVal >= 30 && speedVal <= 70) ? true : false
        }
    }//end Speed


}//end ApplicationWindow

