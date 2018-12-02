# Placeholder for strategy type shit

speed   = 0
optimumSpeedLow = 40
optimumSpeedHigh = 60

def update():
    global speed, optimumSpeedLow, optimumSpeedHigh

    if speed >= 100:
        speed = 0
    else:
        speed += .1

    if optimumSpeedLow >= 50:
        optimumSpeedLow = 0
        optimumSpeedHigh = 10
    else:
        optimumSpeedLow += 3
        optimumSpeedHigh += 5
