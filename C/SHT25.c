// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// SHT25
// This code is designed to work with the SHT25_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Humidity?sku=SHT25_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, SHT25 I2C address is 0x40(64)
	ioctl(file, I2C_SLAVE, 0x40);

	// Send humidity measurement command, NO HOLD master
	char config[1] = {0};
	config[0] = 0xF5;
	write(file, config, 1);
	sleep(1);

	// Read 2 bytes of data
	// humidity msb, humidity lsb
	char data[2] = {0};
	if(read(file, data, 2) != 2)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
		
		//Output data to screen
		printf("Relative Humidity : %.2f RH \n", humidity);
	}
	
	// Send temperature measurement command, NO HOLD master
	config[0] = 0xF3;
	write(file, config, 1);
	sleep(1);

	// Read 2 bytes of data
	// temp msb, temp lsb
	if(read(file, data, 2) != 2)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		float cTemp = (((((data[0] & 0xFF) * 256) + (data[1] & 0xFF)) * 175.72) / 65536.0) - 46.85;
		float fTemp = (cTemp * 1.8) + 32;
		
		//Output data to screen
		printf("Temperature in Celsius : %.2f C \n", cTemp);
		printf("Temperature in Fahrenheit : %.2f F \n", fTemp);
	}
}