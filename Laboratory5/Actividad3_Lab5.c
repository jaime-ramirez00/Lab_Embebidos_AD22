/*******************************************************************************
*
*   i2c.c
*
*   Copyright (c) 2013 Shahrooz Shahparnia (sshahrooz@gmail.com)
*
*   Description:
*   i2c is a command-line utility for executing i2c commands with the 
*   Broadcom bcm2835.  It was developed and tested on a Raspberry Pi single-board
*   computer model B.  The utility is based on the bcm2835 C library developed
*   by Mike McCauley of Open System Consultants, http://www.open.com.au/mikem/bcm2835/.
*
*   Invoking spincl results in a read or write I2C transfer.  Options include the
*   the I2C clock frequency, read/write, address, and port initialization/closing
*   procedures.  The command usage and command-line parameters are described below
*   in the showusage function, which prints the usage if no command-line parameters
*   are included or if there are any command-line parameter errors.  Invoking i2c 
*   requires root privilege.
*
*   This file contains the main function as well as functions for displaying
*   usage and for parsing the command line.
*
*   Open Source Licensing GNU GPLv3
*
*   Building:
* After installing bcm2835, you can build this 
* with something like:
* gcc -o i2c i2c.c -l bcm2835
* sudo ./i2c
*
* Or you can test it before installing with:
* gcc -o i2c -I ../../src ../../src/bcm2835.c i2c.c
* sudo ./i2c
*
*   History:
*   11/05    VERSION 1.0.0: Original
*
*      User input parsing (comparse) and showusage\
*      have been adapted from: http://ipsolutionscorp.com/raspberry-pi-spi-utility/
*      mostly to keep consistence with the spincl tool usage.
*
*      Compile with: gcc -o i2c i2c.c bcm2835.c
*
*      Examples:
*
*           Set up ADC (Arduino: ADC1015)
*           sudo ./i2c -s72 -dw -ib 3 0x01 0x44 0x00 (select config register, setup mux, etc.)
*           sudo ./i2c -s72 -dw -ib 1 0x00 (select ADC data register)
*
*           Bias DAC (Arduino: MCP4725) at some voltage
*           sudo ./i2c -s99 -dw -ib 3 0x60 0x7F 0xF0 (FS output is with 0xFF 0xF0)
*           Read ADC convergence result
*           sudo ./i2c -s72 -dr -ib 2 (FS output is 0x7FF0 with PGA1 = 1)
*  
*      In a DAC to ADC loop back typical results are:
*
*      DAC    VOUT   ADC
*      7FFh   1.6V   677h                    Note ratio is FS_ADC*PGA_GAIN/FS_DAC = 4.096/3.3 = 1.23
*      5FFh   1.2V   4DCh
*      8F0h   1.8V   745h
*      9D0h   2V     7EAh
*      000h   10mV   004h
*
********************************************************************************/

#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MODE_READ 0
#define MODE_WRITE 1

#define MAX_LEN 32

char wbuf[MAX_LEN];

typedef enum {
    NO_ACTION,
    I2C_BEGIN,
    I2C_END
} i2c_init;


uint8_t  init = NO_ACTION;
uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148;
uint8_t slave_address_TC74 = 77;
uint8_t slave_address_RTC = 104;
uint32_t len = 0;
uint8_t  mode = MODE_READ;

char buf[MAX_LEN];
char buf1[MAX_LEN];
int i;
uint8_t data;


int main() {
    
    printf("Running ... \n");
    
    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }
    
    init = I2C_BEGIN;
    // I2C begin if specified    
    if (init == I2C_BEGIN)
    {
      if (!bcm2835_i2c_begin())
      {
        printf("bcm2835_i2c_begin failed. Are you running as root??\n");
	return 1;
      }
    }

    
    bcm2835_i2c_setSlaveAddress(slave_address_RTC);
    bcm2835_i2c_setClockDivider(2500);  
    
    mode = MODE_WRITE;
    len = 8;
    wbuf[0] = 0x00;
    wbuf[1] = 0x00;
    wbuf[2] = 0x00;
    wbuf[3] = 0x12;
    wbuf[4] = 0x01;
    wbuf[5] = 0x01;
    wbuf[6] = 0x01;
    wbuf[7] = 0x01; 
    if (mode == MODE_WRITE) {
    	data = bcm2835_i2c_write(wbuf, len);
    	//printf("Write Result = %d\n", data);
    } 
    
    bcm2835_i2c_setSlaveAddress(slave_address_TC74);
    bcm2835_i2c_setClockDivider(2500);  
    
    mode = MODE_WRITE;
    len = 2;
    wbuf[0] = 0x01;
    wbuf[1] = 0x40;
    if (mode == MODE_WRITE) {
    	data = bcm2835_i2c_write(wbuf, len);
    	/*printf("Write Result = %d\n", data);*/
    } 
    
    mode = MODE_WRITE;
    len = 1;
    wbuf[0] = 0x00;
    if (mode == MODE_WRITE) {
    	data = bcm2835_i2c_write(wbuf, len);
    	/*printf("Write Result = %d\n", data);*/
    } 
    
    int cont = 0;
    
        
    while(1){
        FILE *f = fopen("Datalog.txt", "w");
        if (f == NULL)
        {
            fprintf(stderr,"Error opening file!\n");
            exit(1);
        }
        for(cont=0; cont<3; cont++){
            
            bcm2835_i2c_setSlaveAddress(slave_address_RTC);
            bcm2835_i2c_setClockDivider(2500);
        
            mode = MODE_WRITE;
            len = 1;
            wbuf[0] = 0x00;
            if (mode == MODE_WRITE) {
                data = bcm2835_i2c_write(wbuf, len);
                /*printf("Write Result = %d\n", data);*/
            }
            
            mode = MODE_READ;
            len = 7;
            if (mode == MODE_READ) {
                for (i=0; i<MAX_LEN; i++) buf[i] = 'n';
                data = bcm2835_i2c_read(buf, len);
                /*printf("Read Result = %d\n", data);   
                for (i=0; i<MAX_LEN; i++) {
                    if(buf[i] != 'n') printf("Read Buf[%d] = %x\n", i, buf[i]);
                } */   
            }
        
            bcm2835_i2c_setSlaveAddress(slave_address_TC74);
            bcm2835_i2c_setClockDivider(2500); 
            
            mode = MODE_READ;
            len = 1;
            if (mode == MODE_READ) {
                for (i=0; i<MAX_LEN; i++) buf1[i] = 'n';
                data = bcm2835_i2c_read(buf1, len);
                /*printf("Read Result = %d\n", data);   
                for (i=0; i<MAX_LEN; i++) {
                    if(buf1[i] != 'n') printf("Read Buf[%d] = %x\n", i, buf1[i]);
                }*/    
            }
            
            switch(buf[3]){
                case 1 : {
                    fprintf(f, "RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Mon %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    printf("RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Mon %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    break;
                }
                case 2: {
                    fprintf(f, "RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Tue %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    printf("RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Tue %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    break;
                }
                case 3: {
                    fprintf(f, "RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Wed %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    printf("RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Wen %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    break;
                }
                case 4: {
                    fprintf(f, "RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Thu %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    printf("RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Thu %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    break;
                }
                case 5: {
                    fprintf(f, "RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Fri %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    printf("RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Fri %02x:%02x:%02x PM\n",cont + 1, buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    break;
                }
                case 6: {
                    fprintf(f, "RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Sat %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    printf("RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Sat %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    break;
                }
                case 7: {
                    fprintf(f, "RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Sun %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    printf("RECEIVER %d> Temperature: %02xC, %02x/%02x/%02x Sun %02x:%02x:%02x PM\n",cont + 1,buf1[0],buf[4],buf[5],buf[6],buf[2],buf[1],buf[0]);
                    break;
                }
                
            }
            
            delay(10000);
        }
        printf("\n");
        fclose(f);
        delay(1000);
    }
    
    init = I2C_END;
    // This I2C end is done after a transfer if specified
    if (init == I2C_END) bcm2835_i2c_end();   
    bcm2835_close();
    printf("... done!\n");
    return 0;
}

