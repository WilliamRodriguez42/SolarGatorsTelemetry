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

    //Strategy additions
    property real optimumSpeedLowVal            : 0
    property real optimumSpeedHighVal           : 1

    property real dCL_Low_SOC_val               : 100
    property real dCL_High_Cell_Resistance_val  : 100
    property real dCL_Temperature_val           : 100
    property real dCL_Low_Cell_Voltage_val      : 100
    property real dCL_Low_Pack_Voltage_val      : 100
    property real dCL_Voltage_Failsafe_val      : 100

    property real cCL_High_SOC_val              : 100
    property real cCL_High_Cell_Resistance_val  : 100
    property real cCL_Temperature_val           : 100
    property real cCL_High_Cell_Voltage_val     : 100
    property real cCL_High_Pack_Voltage_val     : 100
    property real cCL_Charger_Latch_val         : 100

    property int discharge_relay_disabled_val       : 0
    property int charge_relay_disabled_val          : 0
    property int charger_safety_disabled_val        : 0
    property int diagnostic_trouble_code_active_val : 1
    property int always_on_power_status_val         : 1
    property int is_ready_power_status_val          : 1
    property int is_charging_power_status_val       : 1

    property real pack_Amp_Hours_val        : 100
    property real high_Temperature_val      : 100
    property real low_Temperature_val       : 100
    property real pack_Current_val          : 100
    property real pack_Instant_Voltage_val  : 100
    property real state_Of_Charge_val       : 100
    property real relay_Status_val          : 100
    property real watt_Hours_val            : 100


    /*
        QML JavaScript Updater
        This updates the data in QML from its slot

    */
    function updateDCL(dCL_Low_SOC, dCL_High_Cell_Resistance, dCL_Temperature, dCL_Low_Cell_Voltage, dCL_Low_Pack_Voltage, dCL_Voltage_Failsafe) {
        dCL_Low_SOC_val                 = dCL_Low_SOC;
        dCL_High_Cell_Resistance_val    = dCL_High_Cell_Resistance;
        dCL_Temperature_val             = dCL_Temperature;
        dCL_Low_Cell_Voltage_val        = dCL_Low_Cell_Voltage;
        dCL_Low_Pack_Voltage_val        = dCL_Low_Pack_Voltage;
        dCL_Voltage_Failsafe_val        = dCL_Voltage_Failsafe;
    }

    function updateCCL(cCL_High_SOC, cCL_High_Cell_Resistance, cCL_Temperature, cCL_High_Cell_Voltage, cCL_High_Pack_Voltage, cCL_Charger_Latch) {
        cCL_High_SOC_val                = cCL_High_SOC;
        cCL_High_Cell_Resistance_val    = cCL_High_Cell_Resistance;
        cCL_Temperature_val             = cCL_Temperature;
        cCL_High_Cell_Voltage_val       = cCL_High_Cell_Voltage;
        cCL_High_Pack_Voltage_val       = cCL_High_Pack_Voltage;
        cCL_Charger_Latch_val           = cCL_Charger_Latch;
    }

    function updateRelays(discharge_relay_disabled, charge_relay_disabled, charger_safety_disabled, diagnostic_trouble_code_active, always_on_power_status, is_ready_power_status, is_charging_power_status) {
        discharge_relay_disabled_val        = discharge_relay_disabled;
        charge_relay_disabled_val           = charge_relay_disabled;
        charger_safety_disabled_val         = charger_safety_disabled;
        diagnostic_trouble_code_active_val  = diagnostic_trouble_code_active;
        always_on_power_status_val          = always_on_power_status;
        is_ready_power_status_val           = is_ready_power_status;
        is_charging_power_status_val        = is_charging_power_status;

        clockText.set();
    }

    function updatePack(pack_Amp_Hours, high_Temperature, low_Temperature, pack_Current, pack_Instant_Voltage, state_Of_Charge, relay_Status, watt_Hours) {
        pack_Amp_Hours_val          = pack_Amp_Hours;
        high_Temperature_val        = high_Temperature;
        low_Temperature_val         = low_Temperature;
        pack_Current_val            = pack_Current;
        pack_Instant_Voltage_val    = pack_Instant_Voltage;
        state_Of_Charge_val         = state_Of_Charge;
        relay_Status_val            = relay_Status;
        watt_Hours_val              = watt_Hours;
    }

    function updateSpeed(speed, optimumSpeedLow, optimumSpeedHigh){
        speedVal            = speed;
        optimumSpeedLowVal  = optimumSpeedLow;
        optimumSpeedHighVal = optimumSpeedHigh;
        speedGauge.redraw() //redraws gauge with new optimumSpeeds
    }

    /*
        QML Slot
        This takes the data from the Python code
    */

    function onDCLUpdate(dCL_Low_SOC, dCL_High_Cell_Resistance, dCL_Temperature, dCL_Low_Cell_Voltage, dCL_Low_Pack_Voltage, dCL_Voltage_Failsafe) {
        updateDCL(dCL_Low_SOC, dCL_High_Cell_Resistance, dCL_Temperature, dCL_Low_Cell_Voltage, dCL_Low_Pack_Voltage, dCL_Voltage_Failsafe)
    }

    function onCCLUpdate(cCL_High_SOC, cCL_High_Cell_Resistance, cCL_Temperature, cCL_High_Cell_Voltage, cCL_High_Pack_Voltage, cCL_Charger_Latch) {
        updateCCL(cCL_High_SOC, cCL_High_Cell_Resistance, cCL_Temperature, cCL_High_Cell_Voltage, cCL_High_Pack_Voltage, cCL_Charger_Latch)
    }

    function onRelaysUpdate(discharge_relay_disabled, charge_relay_disabled, charger_safety_disabled, diagnostic_trouble_code_active, always_on_power_status, is_ready_power_status, is_charging_power_status) {
        updateRelays(discharge_relay_disabled, charge_relay_disabled, charger_safety_disabled, diagnostic_trouble_code_active, always_on_power_status, is_ready_power_status, is_charging_power_status)
    }

    function onPackUpdate(pack_Amp_Hours, high_Temperature, low_Temperature, pack_Current, pack_Instant_Voltage, state_Of_Charge, relay_Status, watt_Hours) {
        updatePack(pack_Amp_Hours, high_Temperature, low_Temperature, pack_Current, pack_Instant_Voltage, state_Of_Charge, relay_Status, watt_Hours)
    }

    function onSpeedUpdate(speed, optimumSpeedLow, optimumSpeedHigh){
        updateSpeed(speed, optimumSpeedLow, optimumSpeedHigh)
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


    //DCL
    Item {
        id: dcl
        width: 1
        height: 1

        Text {
            id: dLowSOC
            x: 34
            y: 188
            color: "#ffffff"
            text: dCL_Low_SOC_val.toFixed(2) + ' %'
            anchors.right: parent.right
            anchors.rightMargin: -184
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: dHighCellResistance
            x: 34
            y: 236
            color: "#ffffff"
            text: dCL_High_Cell_Resistance_val.toFixed(2) + ' Ω'
            anchors.right: parent.right
            anchors.rightMargin: -175
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: dLowCellVoltage
            x: 34
            y: 284
            color: "#ffffff"
            text: dCL_Low_Cell_Voltage_val.toFixed(2) + ' V'
            anchors.right: parent.right
            anchors.rightMargin: -171
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: dLowPackVoltage
            x: 34
            y: 332
            color: "#ffffff"
            text: dCL_Low_Pack_Voltage_val.toFixed(2) + ' V'
            anchors.right: parent.right
            anchors.rightMargin: -171
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: dVoltageFailsafe
            x: 34
            y: 380
            color: "#ffffff"
            text: dCL_Voltage_Failsafe_val.toFixed(2) + ' V'
            anchors.right: parent.right
            anchors.rightMargin: -171
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: dTemperature
            x: 34
            y: 428
            color: "#ffffff"
            text: dCL_Temperature_val.toFixed(2) + ' C'
            anchors.right: parent.right
            anchors.rightMargin: -171
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }
    } //end DCL

    //CCL
    Item {
        id: ccl
        width: 1
        height: 1

        Text {
            id: cHighSOC
            x: 645
            y: 188
            color: "#ffffff"
            text: cCL_High_SOC_val.toFixed(2) + ' %'
            anchors.right: parent.right
            anchors.rightMargin: -795
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: cHighCellResistance
            x: 645
            y: 236
            color: "#ffffff"
            text: cCL_High_Cell_Resistance_val.toFixed(2) + ' Ω'
            anchors.right: parent.right
            anchors.rightMargin: -786
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }


        Text {
            id: cHighCellVoltage
            x: 647
            y: 284
            color: "#ffffff"
            text: cCL_High_Cell_Voltage_val.toFixed(2) + ' V'
            anchors.right: parent.right
            anchors.rightMargin: -784
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: cHighPackVoltage
            x: 647
            y: 332
            color: "#ffffff"
            text: cCL_High_Pack_Voltage_val.toFixed(2) + ' V'
            anchors.right: parent.right
            anchors.rightMargin: -784
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: cChargerLatch
            x: 647
            y: 380
            color: "#ffffff"
            text: cCL_Charger_Latch_val.toFixed(2)
            anchors.right: parent.right
            anchors.rightMargin: -752
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: cTemperature
            x: 647
            y: 428
            color: "#ffffff"
            text: cCL_Temperature_val.toFixed(2) + ' C'
            anchors.right: parent.right
            anchors.rightMargin: -784
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }
    } //end CCL

    //Status
    Item {
        id: statusIndicators
        width: 10
        height: 10

        Image {
            id: dtc
            x: 375
            y: 75
            width: 50
            height: 50
            fillMode: Image.PreserveAspectFit
            source: "Gauge/error.png"

            visible: diagnostic_trouble_code_active_val
        }

        StatusIndicator {
            id: dischargeRelay
            x: 14
            y: 87

            active: discharge_relay_disabled_val
        }

        StatusIndicator {
            id: chargeRelay
            x: 44
            y: 87

            active: charge_relay_disabled_val
        }

        StatusIndicator {
            id: chargerSafety
            x: 74
            y: 87

            active: charger_safety_disabled_val
        }

        Image {
            id: alwaysOnPower
            x: 712
            y: 75
            width: 50
            height: 50
            fillMode: Image.PreserveAspectFit
            source: "Gauge/always.png"

            visible: always_on_power_status_val
        }

        Image {
            id: readyPower
            x: 758
            y: 69
            width: 50
            height: 50
            fillMode: Image.PreserveAspectFit
            source: "Gauge/ready.png"

            visible: is_ready_power_status_val
        }

        Image {
            id: chargingPower
            x: 656
            y: 75
            width: 50
            height: 50
            fillMode: Image.PreserveAspectFit
            source: "Gauge/solar.png"

            visible: is_charging_power_status_val
        }
    } //end Status



    //Pack
    Item {
        id: pack
        width: 1
        height: 1

        Text {
            id: ampHours
            x: 215
            y: 119
            color: "#ffffff"
            text: pack_Amp_Hours_val.toFixed(2) + ' Ah'
            anchors.right: parent.right
            anchors.rightMargin: -372
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: wattHours
            x: 215
            y: 147
            color: "#ffffff"
            text: watt_Hours_val.toFixed(2) + ' kWh'
            anchors.right: parent.right
            anchors.rightMargin: -400
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: current
            x: 216
            y: 176
            color: "#ffffff"
            text: pack_Current_val.toFixed(2) + ' A'
            anchors.right: parent.right
            anchors.rightMargin: -353
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: instantVoltage
            x: 216
            y: 205
            color: "#ffffff"
            text: pack_Instant_Voltage_val.toFixed(2) + ' V'
            anchors.right: parent.right
            anchors.rightMargin: -353
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: pSOC
            x: 429
            y: 119
            color: "#ffffff"
            text: state_Of_Charge_val.toFixed(2) + ' %'
            anchors.right: parent.right
            anchors.rightMargin: -579
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: relayStatus
            x: 429
            y: 147
            color: "#ffffff"
            text: relay_Status_val.toFixed(2)
            anchors.right: parent.right
            anchors.rightMargin: -534
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: highTemperature
            x: 429
            y: 176
            color: "#ffffff"
            text: high_Temperature_val.toFixed(2) + ' C'
            anchors.right: parent.right
            anchors.rightMargin: -566
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: lowTemperature
            x: 429
            y: 205
            color: "#ffffff"
            text: low_Temperature_val.toFixed(2) + ' C'
            anchors.right: parent.right
            anchors.rightMargin: -566
            font.pixelSize: 35
            horizontalAlignment: Text.AlignRight
        }
    } //end Pack

    //Speed
    Item {
        id: speed
        transformOrigin: Item.Center
        CircularGauge {
            id: speedGauge
            x: 190
            y: 256
            width: 420
            height: 409
            stepSize: 0.001

            signal redraw() //Signal for external update

            style: CircularGaugeStyle {
                minimumValueAngle: -90
                maximumValueAngle: 90

            //Fills in gauge tickmarks to show optimum speed range
                function degreesToRadians(degrees) {
                    return degrees * (Math.PI / 180);
                }

                background: Canvas {
                    id: canvas

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.reset();

                        ctx.beginPath();
                        ctx.strokeStyle = "#00FF00";
                        ctx.lineWidth = outerRadius * 0.025;

                        ctx.arc(outerRadius, outerRadius, outerRadius - ctx.lineWidth / 2,
                                degreesToRadians(valueToAngle(optimumSpeedLowVal) - 90), degreesToRadians(valueToAngle(optimumSpeedHighVal) - 90));
                        ctx.stroke();
                    }

                    //Slot for external update
                    Connections {
                        target : speedGauge
                        onRedraw : canvas.requestPaint()
                    }

                }

                //Changes gauge tickmark color for better contrast
                minorTickmark: Rectangle {
                    visible: true
                    implicitWidth: outerRadius * 0.01
                    implicitHeight: outerRadius * 0.03
                    color: (styleData.value >= optimumSpeedLowVal && styleData.value <= optimumSpeedHighVal) ? "#000000" : "#FFFFFF"
                }
            }

            value: speedVal

        }

        StatusIndicator {
            id: optimumSpeed
            x: 385
            y: 444
            width: 30
            height: 33

            //If speed is within 10% of optimumSpeedHigh, display green; else display yellow
            color: (speedVal >= (optimumSpeedHighVal * 0.90) && speedVal <= optimumSpeedHighVal) ? "#00FF00" : "#FFFF00"
            active: (speedVal >= optimumSpeedLowVal && speedVal <= optimumSpeedHighVal) ? true : false
        }
    } //end Speed


}//end ApplicationWindow

