#include <stdint.h>
#include "TM4C123.h"
#include "TM4C123NEW.h"

void PORTF_Init()
{
	SYSCTL_RCGCGPIO_R |= 0x20;
	while((SYSCTL_PRGPIO_R& 0x20)==0){};
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R=0x1F;
	GPIO_PORTF_AFSEL_R=0;
	GPIO_PORTF_PCTL_R=0;		
	GPIO_PORTF_AMSEL_R=0;
	GPIO_PORTF_DIR_R=0x0E;
	GPIO_PORTF_DEN_R=0x1F;
	GPIO_PORTF_PUR_R=0x11;
}

void PORTB_Init()
{
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R& 0x02)==0){};
	GPIO_PORTB_LOCK_R=0x4C4F434B;
	GPIO_PORTB_CR_R=0xFF;
	GPIO_PORTB_AFSEL_R=0;
	GPIO_PORTB_PCTL_R=0;		
	GPIO_PORTB_AMSEL_R=0;
	GPIO_PORTB_DIR_R=0xFF;
	GPIO_PORTB_DEN_R=0xFF;
}

void PORTA_Init()
{
	SYSCTL_RCGCGPIO_R |= 0x01;
	while((SYSCTL_PRGPIO_R& 0x01)==0){};
	GPIO_PORTA_LOCK_R=0x4C4F434B;
	GPIO_PORTA_CR_R=0xFF;
	GPIO_PORTA_AFSEL_R=0;
	GPIO_PORTA_PCTL_R=0;		
	GPIO_PORTA_AMSEL_R=0;
	GPIO_PORTA_DIR_R=0xFF;
	GPIO_PORTA_DEN_R=0xFF;
}

void PORTE_Init()
{
	SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_PRGPIO_R& 0x10)==0){};
	GPIO_PORTE_LOCK_R=0x4C4F434B;
	GPIO_PORTE_CR_R=0xFF;
	GPIO_PORTE_AFSEL_R=0;
	GPIO_PORTE_PCTL_R=0;		
	GPIO_PORTE_AMSEL_R=0;
	GPIO_PORTE_DIR_R=0x02;
	GPIO_PORTE_DEN_R=0xFF;
}
void SYSTICK_Init()
	
{
	NVIC_ST_CTRL_R=0;
	NVIC_ST_RELOAD_R=0x00FFFFFF;
	NVIC_ST_CURRENT_R=0;
	NVIC_ST_CTRL_R=0x05;
}

void SYSWait(uint32_t delay){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = delay-1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x05;
	while( (NVIC_ST_CTRL_R&0X00010000) == 0){}
	
}
void SYSWait_10MS(uint32_t delay){
		uint32_t i;
		for(i=0 ; i<delay; i++){
			SYSWait(500000);
		}
}

void Ultrasonic_init(){
	GPIO_PORTE_DATA_R =0;
	SYSWait(200);
		GPIO_PORTE_DATA_R |= 0x02;
		SYSWait(200);
		GPIO_PORTE_DATA_R &= ~0x02;
	
}
	double TimeValue = 0;
	double distance = 0;
	double x;
	int echoflag = 0;
int start =0;
int end = 0;
int ones, tens, hundreds;
void main(void)
{
  PORTE_Init();
  PORTA_Init();
	PORTB_Init();
	PORTF_Init();
	SYSTICK_Init();
	while(1)
	{
		ones=0;tens=0;hundreds=0;
		Ultrasonic_init();
		SYSTICK_Init();
		while((GPIO_PORTE_DATA_R&0x08) != 0x08){};
		start = NVIC_ST_CURRENT_R&0x00FFFFFF;
		while((GPIO_PORTE_DATA_R&0x08) == 0x08){};
		end=NVIC_ST_CURRENT_R&0x00FFFFFF;
		TimeValue = start-end;
		TimeValue=(TimeValue/2)/50000000.0;
		distance=TimeValue*34300;
		ones = (int)distance%10;
		tens = ((int)distance%100-ones)/10;
	if(distance>=100)	hundreds = ((int)distance/100);
		GPIO_PORTB_DATA_R = ones;
		GPIO_PORTB_DATA_R |= (tens<<4);
		GPIO_PORTA_DATA_R = hundreds<<4;	
		SYSWait_10MS(200);
	}
	
}