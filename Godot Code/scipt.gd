extends Node

var serial: GdSerial

func _ready():
	serial = GdSerial.new()
	
	# List available ports
	var ports: Dictionary = serial.list_ports()
	for i in ports:
		var info = ports[i]
		print("- ", info["port_name"], " (", info["device_name"], ")")
	
	serial.set_port("COM3")
	serial.set_baud_rate(115200)
	
	if serial.open():
		print("Serial Open")
		serial.writeline("Hello")
		serial.writeline("0")
		await get_tree().create_timer(4).timeout
		if serial.bytes_available() > 0:
			print("Response: ", serial.readline())
		serial.close()
		print("Serial Close")
