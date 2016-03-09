// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// SHT25
// This code is designed to work with the SHT25_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=SHT25_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// SHT25 I2C address is 0x40(64)
#define Addr 0x40

float humidity = 0.0, cTemp = 0.0, fTemp = 0.0;
void setup() 
{
    // Set variable
    Particle.variable("i2cdevice", "SHT25");
    Particle.variable("humidity", humidity);
    Particle.variable("cTemp", cTemp);
  
    // Initialise I2C communication as MASTER 
    Wire.begin();
    // Initialise serial communication, set baud rate = 9600
    Serial.begin(9600);
    delay(300);
}

void loop() 
{
    // Begin transmission with given device on I2C bus
    Wire.beginTransmission(Addr);
    // Send humidity measurement command, NO HOLD master
    Wire.write(0xF5);
    // Stop I2C transmission on the device
    Wire.endTransmission();
    delay(500);

    // Request 2 bytes of data from the device
    Wire.requestFrom(Addr, 2);

    // Read 2 bytes of data
    // humidity msb, humidity lsb
    if(Wire.available() == 2)
    {
        int MSB = Wire.read();
        int LSB = Wire.read();
        
        // Convert the data
        humidity = ((((MSB * 256.0) + LSB) * 125.0) / 65536.0) - 6; 
        
        // Output data to dashboard
        Particle.publish("Relative Humidity    :  ",  String(humidity));
    }

    // Begin transmission with given device on I2C bus
    Wire.beginTransmission(Addr);
    // Send temperature measurement command, NO HOLD master           
    Wire.write(0xF3);
    // Stop I2C transmission on the device
    Wire.endTransmission();
    delay(500);
    
    // Request 2 bytes of data from the device
    Wire.requestFrom(Addr,2);

    // Read 2 bytes of data
    // temp msb, temp lsb
    if(Wire.available() == 2)
    {
        int MSB = Wire.read();
        int LSB = Wire.read();
        
        // Convert Temperature data
        cTemp = ((((MSB * 256.0) + LSB) * 175.72) / 65536.0) - 46.85;
        fTemp = (cTemp * 1.8) + 32;
        
        // Output data to dashboard
        Particle.publish("Temperature in Celsius:  ",  String(cTemp));
        Particle.publish("Temperature in Farhenheit:  ", String(fTemp));
  } 
  delay(1000);
}
