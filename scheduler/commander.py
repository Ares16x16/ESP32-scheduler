import serial  #pip install pyserial

ESP_PORT = 'COM4'
ESP_BAUDRATE = 115200
cmd_list = ["add", "remove", "remove_all", "enable", "disable", "enable_all", "disable_all", "instant_fire", "instant_fire_all", "instant_no_fire", "instant_no_fire_all", "tasklist"]

s = serial.Serial(ESP_PORT, ESP_BAUDRATE)

while True:
    command = input("Enter command: ")
    command += "?"
    command_parts = command.split()
    if len(command_parts) == 2 and command_parts[0] in cmd_list:
        try:
            s.write(command.encode())
        except ValueError:
            print("Invalid task number")
    elif command == "exit":
        break
    else:
        print("Invalid command")

s.close()