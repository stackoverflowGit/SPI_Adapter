#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include "ControlSPI.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
	VSI_INIT_CONFIG SPI_Config;
    //ɨ���Ѿ����ӵ��豸
    ret = VSI_ScanDevice(1);
    if (ret <= 0)
    {
		printf("No device connect!\n");
		system("pause");
        return ret;
    }
    //���豸
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("pause");
        return ret;
    }
    //��ʼ���豸(����Ӳ����˫��ģʽ)
    SPI_Config.ControlMode = 1;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 36000000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("pause");
        return ret;
    }
    uint8_t write_buffer[10240];
    uint8_t read_buffer[10240];
    //SPI������˫��ģʽд8�ֽ�����
    for (int i = 0; i < 8; i++)
    {
        write_buffer[i] = i;
    }
    ret = VSI_WriteBytes(VSI_USBSPI, 0, 0, write_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("pause");
        return ret;
    }
    //SPI������˫��ģʽ��8�ֽ�����
    ret = VSI_ReadBytes(VSI_USBSPI, 0, 0, read_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
		printf("Read data error!\n");
		system("pause");
        return ret;
    }
    else
    {
        printf("Read data(Hex):");
        for (int i = 0; i < 8; i++)
        {
            printf("%02X ",read_buffer[i]);
        }
		printf("\n");
    }
    //SPI������˫��ģʽ���Ͷ�ȡ����,����5�ֽ����ݺ�������8�ֽ����ݣ����������ڼ�Ƭѡ�ź�һֱ��Ч
    for (int i = 0; i < 5; i++)
    {
        write_buffer[i] = i;
    }
    ret = VSI_WriteReadBytes(VSI_USBSPI, 0, 0, write_buffer, 5, read_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
		printf("WriteRead data error!\n");
		system("pause");
        return ret;
    }
    else
    {
        printf("Read data(Hex):");
        for (int i = 0; i < 8; i++)
        {
            printf("%02X ",read_buffer[i]);
        }
		printf("\n");
    }
    //��ʼ���豸(������˫��ģʽ)�����ģʽ��Ҳ֧��VSI_WriteBytes��VSI_ReadBytes��VSI_WriteReadBytes��3������
    //VSI_WriteBits,VSI_ReadBits,VSI_WriteReadBits����������������Ӳ����ģʽ��ʹ�ã��ڵ�����ģʽ��Ҳ��ʹ��
    SPI_Config.ControlMode = 2;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 100000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("pause");
        return ret;
    }
    //������ģʽ�������ݣ�����bit�����ɴ�10240bit
    ret = VSI_WriteBits(VSI_USBSPI, 0, 0, (uint8_t *)"10110100100101");//����14bit����
    if (ret != ERR_SUCCESS)
    {
		printf("Write bit error!\n");
		system("pause");
        return ret;
    }
    //������ģʽ��ȡ����,��ȡ���bit���ɴ�10240bit
	uint8_t read_buffer_bin[10240] = {0};
    ret = VSI_ReadBits(VSI_USBSPI, 0, 0, read_buffer_bin,19);//��ȡ19bit����
    if (ret != ERR_SUCCESS)
	{
		printf("Read bit error!\n");
		system("pause");
		return ret;
    }
    else
    {
        printf("Read bits:");
        printf("%s",read_buffer_bin);
		printf("\n");
    }
    //������ģʽ���Ͷ�ȡ����
    ret = VSI_WriteReadBits(VSI_USBSPI, 0, 0,(uint8_t *)"000011110101001",read_buffer_bin, 25);//��ȡ19bit����
    if (ret != ERR_SUCCESS)
    {
		printf("Write bit error!\n");
		system("pause");
        return ret;
    }
    else
    {
        printf("Read bits:");
        printf("%s\n",read_buffer_bin);
    }
    //��ʼ���豸(�ӻ�ģʽ)
    SPI_Config.ControlMode = 0; //�ӻ�ģʽ������Ӳ��ȫ˫��ģʽ��
    SPI_Config.MasterMode = 0;  //�ӻ�ģʽ
    SPI_Config.CPHA = 0;        //ʱ�Ӽ��Ժ���λ�������������һ��
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;    //ֻ֧��8bitģʽ
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("pause");
        return ret;
    }
    //�ӻ�ģʽд���ݣ����ݲ���������MISO���ŷ��ͳ�ȥ������Ҫ�ȵ���������Ƭѡ�źţ�CS����Ч��������ʱ�Ӻ�Ž�������λ���ͳ�ȥ
    //���ǵ��øú�������������أ����ݴ�ŵ��������ڴ滺������
    for (int i = 0; i < 8; i++)
    {
        write_buffer[i] = i;
    }
    ret = VSI_SlaveWriteBytes(VSI_USBSPI, 0, write_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("pause");
        return ret;
    }
    //�ӻ�ģʽ������,���һ������Ϊ�ȴ�ʱ�䣬��λΪ΢�룬�����ȴ�ʱ��󲻹��Ƿ��Ƿ�������ݺ�������������
    //���øú��������ͨ���ж϶������ֽ�����������жϽ����˶����ֽ�����
    //ע�⣺�ڵ���VSI_SlaveWriteBytes������������ȡ���ݵ�ͬʱ���ӻ�Ҳ����յ����ݣ��ӻ�������ȫ˫��ģʽ��
    //�������ڶ�ȡ���ݵ�ʱ����Ҫ�ӻ����յ������ݣ�������������ȡ������ɺ����VSI_SlaveReadBytes�������������ص�����
    int read_data_num = 0;
    ret = VSI_SlaveReadBytes(VSI_USBSPI, 0, read_buffer, &read_data_num,100);
    if (ret != ERR_SUCCESS)
    {
		printf("Read data error!\n");
		system("pause");
        return ret;
    }
    else
    {
        if (read_data_num > 0)
        {
            printf("Read data num: %d\n",read_data_num);
            printf("Read data(Hex):");
            for (int i = 0; i < read_data_num; i++)
            {
                printf("%02X ",read_buffer[i]);
            }

        }
        else
        {
            printf("No data!\n");
        }
	}
	system("pause");
	return 0;
}
