# Placeholder for strategy type shit

optimumSpeedLow = 40
optimumSpeedHigh = 60

def update():
    global optimumSpeedLow, optimumSpeedHigh

    if optimumSpeedLow >= 50:
        optimumSpeedLow = 0
        optimumSpeedHigh = 10
    else:
        optimumSpeedLow += 3
        optimumSpeedHigh += 5
