speed = 0

def update():
        global speed

        if speed >= 100:
                speed = 0
        else:
                speed += .05
