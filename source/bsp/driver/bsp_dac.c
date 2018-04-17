/*******************************************************************************
* Description  : Ϊ�Ƚ����ṩ�ο���ѹ
* Author       : 2018/4/13 ������, by redmorningcn
*******************************************************************************/
#include "bsp_dac.h"
#include "bsp.h"

/*****************************************************************************************************/
/* EXTERN VARIABLES*/


//============================================================================//

/*******************************************************************************
 * ��    �ƣ� DAC_gpio_config
 * ��    �ܣ� 
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� fouth peng
 * �������ڣ� 2017-05-11
 * ��    �ģ�
 *******************************************************************************/
void DAC_gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
	connected to the DAC converter. In order to avoid parasitic consumption, 
	the GPIO pin should be configured in analog */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	     //ʹ��DACʱ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
 * ��    �ƣ� DAC_gpio_config
 * ��    �ܣ� 
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� fouth peng
 * �������ڣ� 2017-05-11
 * ��    �ģ�
 *******************************************************************************/
void DAC_mode_config()
{
	DAC_InitTypeDef            DAC_InitStructure;  
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE);	     //ʹ��DACʱ��
	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude =  DAC_TriangleAmplitude_4095  ;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	/* DAC channel2 Configuration */
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
	automatically connected to the DAC converter. */
	DAC_Cmd(DAC_Channel_1, ENABLE);

	/* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
	automatically connected to the DAC converter. */
	DAC_Cmd(DAC_Channel_2, ENABLE);
	//DAC_SetDualChannelData(DAC_Align_12b_R, 1010, 129);
    DAC_SetDualChannelData(DAC_Align_12b_R, 4096/5, 4096/5);
}

/*******************************************************************************
 * ��    �ƣ� BSP_dac_init
 * ��    �ܣ� 
 * ��ڲ����� ��
 * ���ڲ����� ��
 * ���� ���ߣ� fouth peng
 * �������ڣ� 2017-05-11
 * ��    �ģ�
 *******************************************************************************/
void BSP_dac_init()
{
	DAC_gpio_config();
	DAC_mode_config();
}

/*******************************************************************************
* Description  : ���ñȽ����ıȽϵ�ѹֵ����ѹֵ��λ��mv��
* Author       : 2018/4/13 ������, by redmorningcn
*******************************************************************************/
void bsp_set_dacvalue(uint32 value)
{
    uint32  dac;
    dac = (value * 4096)/3300;
    DAC_SetDualChannelData(DAC_Align_12b_R, dac, dac);
}



