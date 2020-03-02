#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdlib.h"
char str[49];
uint16_t Adc_Data;

void delay(int a)
{
	while(a) a--;
}

uint16_t Adc_Read()
 {
	 ADC_SoftwareStartConvCmd(ADC1 ,ENABLE);
	 while(!ADC_GetFlagStatus(ADC1 ,ADC_FLAG_EOC));
	 return ADC_GetConversionValue(ADC1);
 }
 
 void UsartPut(USART_TypeDef* USARTx ,volatile char *s)
 {
	 while(*s)
	 {
		 while(!(USARTx->SR & USART_SR_TC));
		 USART_SendData(USARTx ,*s);
		 *s++;
	 }
 }
 
 void Adc_Int()
 {
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1 ,ENABLE);
	 RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	 
	 GPIO_InitTypeDef GPIO_IntStr;
	 ADC_InitTypeDef ADC_IntStr;
	 
	 GPIO_IntStr.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_IntStr.GPIO_Pin = GPIO_Pin_0;
	 GPIO_Init(GPIOA ,&GPIO_IntStr);
	 
	 ADC_IntStr.ADC_ContinuousConvMode = DISABLE;
	 ADC_IntStr.ADC_DataAlign = ADC_DataAlign_Right;
	 ADC_IntStr.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	 ADC_IntStr.ADC_Mode = ADC_Mode_Independent;
	 ADC_IntStr.ADC_NbrOfChannel = 1;
	 ADC_IntStr.ADC_ScanConvMode = DISABLE;
	 ADC_Init(ADC1 ,&ADC_IntStr);
	 
	 ADC_Cmd(ADC1 ,ENABLE);
	 
	 ADC_ResetCalibration(ADC1);
	 while(ADC_GetResetCalibrationStatus(ADC1));
	 ADC_StartCalibration(ADC1);
	 while(ADC_GetCalibrationStatus(ADC1));
	 
	 ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_13Cycles5);
	 
 }
 
 void Usart_Int()
 {
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 ,ENABLE);
	 
	 GPIO_InitTypeDef GPIO_IntStr;
	 USART_InitTypeDef USART_IntStr;
	 
	 GPIO_IntStr.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_IntStr.GPIO_Pin = GPIO_Pin_9;
	 GPIO_IntStr.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA ,&GPIO_IntStr);
	 
	 USART_IntStr.USART_BaudRate = 9600;
	 USART_IntStr.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	 USART_IntStr.USART_Mode = USART_Mode_Tx;
	 USART_IntStr.USART_Parity = USART_Parity_No;
	 USART_IntStr.USART_StopBits = USART_StopBits_1;
	 USART_IntStr.USART_WordLength = USART_WordLength_8b;
	 USART_Init(USART1 ,&USART_IntStr);
	 
	 USART_Cmd(USART1 ,ENABLE);
	 
 }
 
 int main()
{
	Adc_Int();
	Usart_Int();
	for(;;)
	{
		Adc_Data = Adc_Read();
		sprintf(str ,"%d\n" ,Adc_Data);
		UsartPut(USART1 ,str);
		delay(50000);
	}
}
