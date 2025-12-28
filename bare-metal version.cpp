#include "mbed.h"
#include "stdint.h" //This allow the use of integers of a known width
#define LM75_REG_TEMP (0x00) // Temperature Register
#define LM75_REG_CONF (0x01) // Configuration Register
#define LM75_ADDR     (0x90) // LM75 address

#define LM75_REG_TOS (0x03) // TOS Register
#define LM75_REG_THYST (0x02) // THYST Register

I2C i2c(I2C_SDA, I2C_SCL);

DigitalOut blue(LED2);
DigitalOut myled(LED1);

InterruptIn lm75_int(D7); // Make sure you have the OS line connected to D7

Ticker cycle_ticker;
float cycle_time_interval = 1;

Ticker led_ticker;
float led_time_interval = 0.7;

Serial pc(SERIAL_TX, SERIAL_RX);

int16_t i16; // This variable needs to be 16 bits wide for the TOS and THYST conversion to work - can you see why?
char data_write[3];
char data_read[3];

float tos=28; // TOS temperature
float data[60];
volatile int count1 = 0; 
volatile bool recording=true;
volatile bool alarm=false;
volatile bool readtemp = false;

void onledTicker(void)
{
       blue=!blue;
}


void blue_flip()
{
   // stop the recording
   recording=false;
   alarm=true;

// Avoid using printf in interrupts anyway as it takes too long to execute.
// pc.printf("Interrupt triggered!\r\n");
}

void add_value(float v)
{

    if (count1 < 60) {
        data[count1] = v;
        count1++;
    }
    else {
        for (int i = 0; i < 60 - 1; i++) {
            data[i] = data[i + 1];
        }
        data[60 - 1] = v;
    }
}

void onCycleTicker(void)
{
      // Read temperature register
       readtemp = true;
         
}



int main()
{ 

/* Configure the Temperature sensor device STLM75:
    - Thermostat mode Interrupt
    - Fault tolerance: 0
    - Interrupt mode means that the line will trigger when you exceed TOS and stay triggered until a register is read - see data sheet*/
data_write[0] = LM75_REG_CONF;
data_write[1] = 0x02;
int status = i2c.write(LM75_ADDR, data_write, 2, 0);

        if (status != 0)
        { // Error
                while (1)
                {
                        myled = !myled;
                        wait(0.2);
                }
        }
        // This section of code sets the TOS register
        data_write[0]=LM75_REG_TOS;
        i16 = (int16_t)(tos*256) & 0xFF80;
        data_write[1]=(i16 >> 8) & 0xff;
        data_write[2]=i16 & 0xff;
        i2c.write(LM75_ADDR, data_write, 3, 0);

        cycle_ticker.attach(onCycleTicker, cycle_time_interval);
  

        // This line attaches the interrupt.
        // The interrupt line is active low so we trigger on a falling edge
        lm75_int.fall(&blue_flip);

       bool ledstarted = false;
       bool datasent = false;
       while(1){
                 
             if (readtemp==true and recording==true){    
                data_write[0] = LM75_REG_TEMP;
                i2c.write(LM75_ADDR, data_write, 1, 1); // no stop
                i2c.read(LM75_ADDR, data_read, 2, 0);

                // Calculate temperature value in Celcius
                int16_t i16 = (data_read[0] << 8) | data_read[1];
                // Read data as twos complement integer so sign is correct
                float temp = i16 / 256.0;
              
                     add_value(temp);
                readtemp=false;
                }
    
       if(alarm==true and ledstarted==false){
       led_ticker.attach(onledTicker, led_time_interval);
       ledstarted=true;
       }

        if(recording==false and datasent==false){
            int i=0;
            while(i< count1){
                pc.printf("%f\r\n", data[i]);
                i++;
            }
            datasent=true;
        }

}

}
