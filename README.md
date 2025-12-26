# Freertos-temperature-monitoring-system

A real-time temperature monitoring system built on **STM32F746ZG** using **FreeRTOS**.
The system periodically samples temperature data via I2C through LM75, buffers recent measurements,
and triggers an alarm when a threshold is exceeded and alarm clears below a lower threshold. Buffered data is streamed to a host PC via UART and visualized in real time using Python. And an watchdog (IWDG) is integrated to improve system reliability.

---
## Bare-metal vs FreeRTOS Implementation
This project was first implemented as a bare-metal firmware using interrupts.
It was later redesigned using FreeRTOS to separate sensing, alarm handling,
and communication into independent tasks.

---

## System Architecture

**FreeRTOS Tasks**
- `TempTask`: reads temperature and pushes data to queue
- `AlarmTask`: flashes LED when threshold exceeded
- `SerialTask`: transmits buffered data to host

---

## Hardware
- STM32F746ZG Nucleo Board
- LM75 Temperature Sensor
- USB (Virtual COM Port)

---

## Software
- STM32CubeMX
- Keil MDK
- FreeRTOS (CMSIS-RTOS v2)
- Python 3
- pyserial, matplotlib

---

## Running the Project

### Firmware
1. Open the uvision file 
2. build and flash it to STM32F746ZG

### Host Visualization
```bash
python temperature_plot1.py
