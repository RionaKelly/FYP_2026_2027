extends Node

var serial: GdSerial

func _ready():
	serial = GdSerial.new()
	
	# List available ports
	var ports: Dictionary = serial.list_ports()
	for i in ports:
		var info = ports[i]
		print("- ", info["port_name"], " (", info["device_name"], ")")
	
	serial.set_port("/dev/ttyACM0")#("COM3")
	serial.set_baud_rate(115200)
	
	var response = ""
	
	if serial.open():
		print("Serial Open")
		serial.writeline("Hello")
		await get_tree().create_timer(0.5).timeout
		while serial.bytes_available() > 0:
			response = serial.readline()
			if response.substr(0, 8) == "TO_GODOT":
				print("Response: ", response.substr(8))
		serial.close()
		print("Serial Close")
