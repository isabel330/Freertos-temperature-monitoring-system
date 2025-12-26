import serial
import matplotlib
matplotlib.use('TkAgg')  # ensures plot window works on Windows
import matplotlib.pyplot as plt

COM_PORT = 'COM3'       # STM32 COM port on PC
BAUD_RATE = 9600
TIMEOUT = 1             # seconds

ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=TIMEOUT)

BUFFER_LEN = 60    
buffer = []

print("Reading temperatures... waiting for alarm")

while True:
    line = ser.readline().decode(errors='ignore').strip()
    
    if line == '': # skip empty lines
        continue

    temp = float(line)
    buffer.append(temp)

    if len(buffer) > BUFFER_LEN:
        buffer.pop(0)

    print(f"Temp: {temp:.2f} °C")

    if temp > 28.0:     # Trigger  plot if temp > 28
        print("\n ALARM TRIGGERED!")

        plt.figure(figsize=(8,4))
        plt.plot(buffer, marker='o')
        plt.xlabel("Readings ago")
        plt.ylabel("Temperature (°C)")
        plt.title("Temperature readings before alarm")
        plt.grid(True)
        plt.tight_layout()
        plt.show()  # blocks until window closed

        print("Plot done. Exiting.")
        break  # exit the loop and script

# close serial port
ser.close()


