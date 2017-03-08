//==============================================================================
//
// Title:		USB_SPI_AT45DB041D.c
// Purpose:		A short description of the implementation.
//
// Created on:	2014/3/7 at 16:21:25 by Win7.
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
#define FLASH_CHREAD		0x0B
#define FLASH_CLREAD		0x03
#define FLASH_PREAD			0xD2

#define FLASH_BUFWRITE1		0x84
#define FLASH_IDREAD		0x9F
#define FLASH_STATUS		0xD7
#define PAGE_ERASE			0x81
#define PAGE_READ			0xD2
#define MM_PAGE_TO_B1_XFER	0x53					// �����洢����ָ��ҳ���ݼ��ص���һ������
#define BUFFER_2_WRITE		0x87					// д��ڶ�������
#define BUFFER_1_READ		0xD4					// �����һ������
#define B2_TO_MM_PAGE_PROG_WITH_ERASE		0x86	// ���ڶ�������������д�����洢��������ģʽ��
#define B2_TO_MM_PAGE_PROG_WITHOUT_ERASE	0x89	// ���ڶ�������������д�����洢�����ǲ���ģʽ��
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
    SPI_Config.ClockSpeed = 1125000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("cmd /C pause");
        return ret;
    }
	//JEDEC ID
	write_buffer[0] = FLASH_IDREAD;
	ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,write_buffer,1,read_buffer,4);
	if(ret != ERR_SUCCESS){
		printf("Read flash ID error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Flash ID = 0x%08X\n",(read_buffer[0]<<24)|(read_buffer[1]<<16)|(read_buffer[2]<<8)|(read_buffer[3]));
	}
	//Wait Busy
	do{
		write_buffer[0] = FLASH_STATUS;
		ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,write_buffer,1,read_buffer,1);
	}while(((read_buffer[0]&0x80)==0)&&(ret == ERR_SUCCESS));
	if(ret != ERR_SUCCESS){
		printf("Sector Erase error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Sector erase success!\n");
	}
	//Page Earse
	write_buffer[0] = PAGE_ERASE;
	write_buffer[1] = 0x00;
	write_buffer[2] = 0x00;
	write_buffer[3] = 0x00;
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,4);
	if(ret != ERR_SUCCESS){
		printf("Sector Erase start error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Sector erase start success!\n");
	}
	//Wait Busy
	do{
		write_buffer[0] = FLASH_STATUS;
		ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,write_buffer,1,read_buffer,1);
	}while(((read_buffer[0]&0x80)==0)&&(ret == ERR_SUCCESS));
	if(ret != ERR_SUCCESS){
		printf("Sector Erase error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Sector erase success!\n");
	}
	//Write data to buffer
	write_buffer[0] = BUFFER_2_WRITE;
	write_buffer[1] = 0x00;
	write_buffer[2] = 0x00;
	write_buffer[3] = 0x00;
	for(i=0;i<256;i++){
		write_buffer[4+i] = (char)i;
	}
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,256+4);
	if(ret != ERR_SUCCESS){
		printf("Write data to buffer error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Write data to buffer success!\n");
	}
	//Buffer to Main Memory Page Program without Built-in Erase
	write_buffer[0] = B2_TO_MM_PAGE_PROG_WITHOUT_ERASE;
	write_buffer[1] = 0x00;
	write_buffer[2] = 0x00;
	write_buffer[3] = 0x00;
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,4);
	if(ret != ERR_SUCCESS){
		printf("Write data to main memory error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Write data to main memory success!\n");
	}
	//Wait Busy
	do{
		write_buffer[0] = FLASH_STATUS;
		ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,write_buffer,1,read_buffer,1);
	}while(((read_buffer[0]&0x80)==0)&&(ret == ERR_SUCCESS));
	//Main Memory Page Read
	write_buffer[0] = PAGE_READ;
	write_buffer[1] = 0x00;
	write_buffer[2] = 0x00;
	write_buffer[3] = 0x00;
	write_buffer[4] = 0x00;
	ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,write_buffer,8,read_buffer,256);
	if(ret != ERR_SUCCESS){
		printf("Read data to buffer error :%d\n",ret);
		system("cmd /C pause");
		return ret;
	}else{
		printf("Read data to buffer success!\n");
		for(i=0;i<256;i++){
			printf("%02X ",read_buffer[i]);
			if(((i+1)%16)==0){
				printf("\n");
			}
		}
		printf("\n");
	}
	system("cmd /C pause");
	return 0;
}
