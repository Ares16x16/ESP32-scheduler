import socket

ESP_IP = '192.168.1.79'  # Replace with the IP address of your ESP32
ESP_PORT = 1234
cmd_list = ["add", "remove", "remove_all", "enable", "disable", "enable_all", "disable_all", "instant_fire", "instant_fire_all", "instant_no_fire", "instant_no_fire_all"]
# cmd (<command> <number>) number is a must for all command, even though no index is needed. Just input random number
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Get command from the user and send it to the ESP32
while True:
    command = input("Enter command: ")
    command += "?"
    command_parts = command.split()
    if len(command_parts) == 2 and command_parts[0] in cmd_list:
        try:
            command_bytes = command.encode()
            sock.sendto(command_bytes, (ESP_IP, ESP_PORT))
        except ValueError:
            print("Invalid task number")
    elif command == "exit":
        break
    else:
        print("Invalid command")

sock.close()