  /*
  ******************************************************************************
  * @file     : USB_GPIO_Test.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 11:53
  * @brief    : USB_GPIO_Test demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
#include "stdafx.h"
#include "ControlGPIO.h"


int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
    // Scan connected device
    ret = VGI_ScanDevice(1);
    if (ret <= 0)
    {
        printf("No device connect!\n");
        return ret;
    }
    // Open device 
    ret = VGI_OpenDevice(VGI_USBGPIO, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    // Set GPIO_7 and GPIO_8 to output 
    ret = VGI_SetOutput(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin output error!\n");
        return ret;
    }
    // Set GPIO_7 and GPIO_8 
    ret = VGI_SetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin high error!\n");
        return ret;
    }
    // Reset GPIO_7 and GPIO_8 
    ret = VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin low error!\n");
        return ret;
    }
    // Set GPIO_4 and GPIO_5 to input 
    ret = VGI_SetInput(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin input error!\n");
        return ret;
    }
    // Get GPIO_4 and GPIO_5 status 
    uint16_t pin_value = 0;
    ret = VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5, &pin_value);
    if (ret != ERR_SUCCESS)
    {
        printf("Get pin data error!\n");
        return ret;
    }
    else
    {
        if ((pin_value & VGI_GPIO_PIN4) != 0)
        {
            printf("GPIO_4 is high-level!\n");
        }
        else
        {
            printf("GPIO_4 is low-level!\n");
        }
        if ((pin_value & VGI_GPIO_PIN5) != 0)
        {
            printf("GPIO_5 is high-level!\n");
        }
        else
        {
            printf("GPIO_5 is low-level!\n");
        }
    }
    //将GPIO_4和GPIO_5引脚设置成开漏模式（需加上拉电阻，可当双向口）
    ret = VGI_SetOpenDrain(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin open drain error!\n");
        return ret;
    }
    //将GPIO_4和GPIO_5输出高电平
    ret = VGI_SetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin high error!\n");
        return ret;
    }
    //将GPIO_4和GPIO_5输出低电平
    ret = VGI_ResetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin high error!\n");
        return ret;
    }
    //获取GPIO_4和GPIO_5引脚状态
    ret = VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN4 | VGI_GPIO_PIN5, &pin_value);
    if (ret != ERR_SUCCESS)
    {
        printf("Get pin data error!\n");
        return ret;
    }
    else
    {
        if ((pin_value & VGI_GPIO_PIN4) != 0)
        {
            printf("GPIO_4 is high-level!\n");
        }
        else
        {
            printf("GPIO_4 is low-level!\n");
        }
        if ((pin_value & VGI_GPIO_PIN5) != 0)
        {
            printf("GPIO_5 is high-level!\n");
        }
        else
        {
            printf("GPIO_5 is low-level!\n");
        }
    }
    //关闭设备
    ret = VGI_CloseDevice(VGI_USBGPIO, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Close device error!\n");
        return ret;
    }
	return 0;
}
