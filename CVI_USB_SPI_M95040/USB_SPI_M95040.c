//==============================================================================
//
// Title:		USB_SPI_M95040.c
// Purpose:		A short description of the implementation.
//
// Created on:	2014/3/7 at 16:38:03 by Win7.
// Copyright:	Win7. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "ControlSPI.h"
#include <ansi_c.h>
#include <stdio.h>

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
int main (void)
{
	int ret,i;
	uint8_t write_buffer[1024]={0};
	uint8_t read_buffer[1024]={0};
	VSI_INIT_CONFIG SPI_Config;
    //Scan device(must call)
    ret = VSI_ScanDevice(1);
    if (ret <= 0)
    {
		printf("No device connect!\n");
		system("cmd /C pause");
        return ret;
    }
    //Open device(must call)
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("cmd /C pause");
        return ret;
    }
    //Initialize the device (host hardware half-duplex mode)
    SPI_Config.ControlMode = 1;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 4500000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error! %d\n",ret);
		system("cmd /C pause");
        return ret;
    }
	//Write Enable
	write_buffer[0] = 0x06;
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,1);
	if(ret != ERR_SUCCESS){
		printf("Flash write enable error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Write enable success!\n");
	}
	//Write data
	write_buffer[0] = 0x02;
	//Low 8 bit address, highest address is stored in write_buffer[0] of bit3
	write_buffer[1] = 0x00;
	for(i=0;i<16;i++)
	{
		write_buffer[i+2] = i;
	}
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,18);
	if(ret != ERR_SUCCESS){
		printf("Write data error!\n");
		system("cmd /C pause");
		return ret;
	}else{
		printf("Write 16 byte success!\n");
	}
	//Delay for a period of time, to ensure data to complete
	Sleep(20);
	//Read data
	write_buffer[0] = 0x03;
	//Low 8 bit address, highest address is stored in write_buffer[0] of bit3
	write_buffer[1] = 0x00;
	ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,write_buffer,2,read_buffer,16);
	if(ret != ERR_SUCCESS){
		printf("Read Data error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Read Data success\n");
	}
	for(i=0;i<16;i++){
		printf("%02X ",read_buffer[i]);
	}
	printf("\n");
	system("cmd /C pause");
	return 0;
}