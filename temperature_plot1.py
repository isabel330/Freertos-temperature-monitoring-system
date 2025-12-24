import serial
import matplotlib
matplotlib.use('TkAgg')  # ensures plot window works on Windows
import matplotlib.pyplot as plt

# -----------------------------
# CONFIGURE SERIAL PORT
# -----------------------------
COM_PORT = 'COM3'       # set your STM32 COM port
BAUD_RATE = 115200
TIMEOUT = 1             # seconds

ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=TIMEOUT)

# -----------------------------
# PARAMETERS
# -----------------------------
BUFFER_LEN = 60          # store up to 60 readings
buffer = []

# -----------------------------
# READ SERIAL DATA
# -----------------------------
print("Reading temperatures... waiting for alarm > 28°C")

while True:
    line = ser.readline().decode(errors='ignore').strip()
    
    # skip empty lines
    if line == '':
        continue

    temp = float(line)
    buffer.append(temp)

    # keep only the last BUFFER_LEN readings
    if len(buffer) > BUFFER_LEN:
        buffer.pop(0)

    print(f"Temp: {temp:.2f} °C")

    # Trigger alarm plot if temp > 28
    if temp > 28.0:
        print("\n=== ALARM TRIGGERED! Plotting last readings ===")

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
