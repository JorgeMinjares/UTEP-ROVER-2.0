import machine
import time
import ssd1306


class Bluetooth(object):
    def __init__(self, uart_num:int, baudrate:int) -> None:
        if uart_num == 1:
            self.uart = machine.UART(uart_num, baudrate=baudrate, tx=machine.Pin(8), rx=machine.Pin(9))
        elif uart_num == 0:
            self.uart = machine.UART(uart_num, baudrate=baudrate, tx=machine.Pin(0), rx=machine.Pin(1))
        else:
            print("Not a valid uart selection!!!")
        return
    
    def write(self, msg:str):
        self.uart.write(msg)
        return
    
    def read(self, bytes:int):
        return self.uart.read(bytes)

    def read_char(self):
        return self.uart.read(1)

class Button(object):
    def __init__(self,pin:int, up:int) -> None:
        if up:
            self.button = machine.Pin(pin, machine.Pin.IN, machine.Pin.PULL_UP)
        else:
            self.button = machine.Pin(pin, machine.Pin.IN, machine.Pin.PULL_DOWN)
        return

    def read(self):
        return self.button.value()

class Joystick(object):
    def __init__(self, x:int, y:int, button:int) -> None:
        self.x = machine.ADC(x)
        self.y = machine.ADC(y)
        self.button = Button(button, 1)
        return

    def read_x_axis(self):
        return self.x.read_u16()

    def read_y_axis(self):
        return self.y.read_u16()

    def read(self):
        return (self.read_x_axis(), self.read_y_axis())

    def read_button(self):
        return self.button.read()

class Led(object):
    def __init__(self, pin:int) -> None:
        self.led = machine.Pin(pin, machine.Pin.OUT)
        self.state = 0
        return

    def on(self):
        self.led.value(1)
        self.state = 1
        return

    def off(self):
        self.led.value(0)
        self.state = 0
        return

    def toggle(self):
        self.state =  0 if self.state else 1
        self.led.value(self.state)
        return

class RCMessage():
    # uart messages
    JOYSTICK_UP_MSG = "j-up\r\n"
    JOYSTICK_DOWN_MSG = "j-down\r\n"
    JOYSTICK_LEFT_MSG = "j-left\r\n"
    JOYSTICK_RIGHT_MSG = "j-right\r\n"
    JOYSTICK_BUTTON_MSG = "j-button\r\n"
    # Button message
    BUTTON_UP_MSG = "b-up\r\n"
    BUTTON_DOWN_MSG = "b-down\r\n"
    BUTTON_LEFT_MSG = "b-left\r\n"
    BUTTON_RIGHT_MSG = "b-right\r\n"

def convert_to_volts(adc_value):
    return adc_value * ( (3.3) /65535)

# set battery adc pin
battery = machine.ADC(28)

# set i2c configuration for OLED
i2c = machine.I2C(0, sda=machine.Pin(16), scl=machine.Pin(17))
display = ssd1306.SSD1306_I2C(128, 32, i2c)


# Display intial screen
display.text('RP2040', 40, 0)
display.text('RemoteController', 0, 15)
display.show()
# sleep for 2 seconds
time.sleep(2)

display.fill(0)
display.text('Senior 1', 40, 0)
display.text('Hardware init()', 0, 10)
display.text('Please wait.', 0, 20)
display.show()

# create buttons 
# 0: up, 1: down, 2: left, 3: right
button = [Button(10,0), Button(12,0), Button(13,0), Button(11,0)]

# create leds
led = [Led(18), Led(19), Led(20), Led(21)]

# set joy stick
# button joystick will trigger sensor probe
joystick = Joystick(x=27,y=26,button=22)

# set bluetooh
bluetooh = Bluetooth(uart_num = 1, baudrate=9600)

# set onboard led
onboard = Led(25)
onboard.off()
time.sleep(3)

# test leds
for i in led:
    i.on()
    time.sleep(.1)
    i.off()
# test leds 
for i in reversed(led):
    i.on()
    time.sleep(.1)
    i.off()

# test leds
for i in led:
    i.on()
    time.sleep(.1)
    i.off()
# test leds 
for i in reversed(led):
    i.on()
    time.sleep(.1)
    i.off()

# Set center x and y and add offset
print('Set center x and y')
offset = 1000 # use offset to avoid dummy data
center_x = (1 << 16)/2 + offset # add offset
center_y = (1 << 16)/2 + offset # add offset

temp  = 0.0

print(f"Center x: {center_x}, Center y: {center_y}")
time.sleep(0.5)

while True:
    # main page for OLED
    display.fill(0)
    display.text('RP-RC', 40, 0)
    display.text("State: ", 0, 16)
    # Battery text
    display.text("Battery: ", 0, 25)

    # read battery
    voltage = round(convert_to_volts(battery.read_u16()), 2)
    if temp != voltage:
        temp = voltage
        bat_msg = str(voltage) + 'v'
    # display battery voltage
    display.text(bat_msg, 70, 25)
    x,y = 0, 0
    # store joystick readings
    x,y = joystick.read()
    
    print(f"X: {x}, Y: {y}")
    print(f"Center x: {center_x}, Center y: {center_y}")

    # joystick down
    if x < center_x and y < center_y/2:
        display.text(RCMessage.JOYSTICK_DOWN_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.JOYSTICK_DOWN_MSG)
        print("DOWN")
    # joystick down
    elif x < center_x and y > center_y:
        display.text(RCMessage.JOYSTICK_UP_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.JOYSTICK_UP_MSG)
        print("UP")
    # joystick left
    elif x < center_x/2 and y < center_y:
        display.text(RCMessage.JOYSTICK_LEFT_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.JOYSTICK_LEFT_MSG)
        print("LEFT")
    # joystick right
    elif x > center_x and y < center_y:
        display.text(RCMessage.JOYSTICK_RIGHT_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.JOYSTICK_RIGHT_MSG)
        print("RIGHT")
    # joystick button
    elif joystick.read_button() == 0:
        display.text(RCMessage.JOYSTICK_BUTTON_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.JOYSTICK_BUTTON_MSG)
        print("Joystick button has been pressed!!")
    else:
        print("Joystick centered")

    buttonId = -1
    # check up
    if button[0].read():
        buttonId = 0
        print("First button press")
        display.text(RCMessage.BUTTON_UP_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.BUTTON_UP_MSG)
    # check down
    elif button[1].read():
        buttonId = 1
        print("Second button press")
        display.text(RCMessage.BUTTON_DOWN_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.BUTTON_DOWN_MSG)
    # check left
    elif button[2].read():
        buttonId = 2
        print("Third button press")
        display.text(RCMessage.BUTTON_LEFT_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.BUTTON_LEFT_MSG)
    # check right
    elif button[3].read():
        buttonId = 3
        print("Fourth button press")
        display.text(RCMessage.BUTTON_RIGHT_MSG.strip('\r\n'), 50, 16)
        bluetooh.write(RCMessage.BUTTON_RIGHT_MSG)
    else:
        print("No button has been pressed!")
    
    # turn on led
    if buttonId is not -1:
        led[buttonId].on()
    
    # show oled screen
    display.show()
    time.sleep(.5)
    onboard.toggle()
    
    # turn off led
    if buttonId is not -1:
        led[buttonId].off()