/*
 * GccApplication1.c
 */  


 #define F_CPU 1000000
 #define D4 eS_PORTD4
 #define D5 eS_PORTD5
 #define D6 eS_PORTD6
 #define D7 eS_PORTD7
 #define RS eS_PORTD2
 #define EN eS_PORTD3
 #define BAUD 9600                                   // define baud
 #define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
 #define digital_read(x,y) (x & (1<<y))
 
 #include <stdlib.h>
 #include <avr/io.h>
 #include <util/delay.h>
 #include "lcd.h"
 #include<string.h>
 #include <avr/interrupt.h>
 
 
 
 
 void checkTempHeart();
 //void Setup(void);
// void Loop(void);
 //void serialSetup(void);
// void serialSend(char* sendString);
 //int sendMSG(int z,int k);

 //Global variables.
// char ar[]= "hello";
  
// void usartinit();
 uint16_t ReadADC(uint8_t ch);
 void InitADC();
 
 unsigned char rxvalue(void);
 unsigned char rxdata,a,cmd,b;
 unsigned int z;
 unsigned char message[15];
 
	 
	 uint16_t adc_result0;
	 int8_t heart_rate = 0;
	 int8_t temperature = 0;
	 
int readHeartRate();
int time =4;
//char buffer[10];
int c=0;



	 


 int main(){
	 DDRA &= ~(1<<PINA1);
	 DDRC=0b11111101;
	 DDRB=0x10;
	 DDRD=0xFF;
	 
     PORTB = 0;
	 int c=0;
	 //char buffer[10];
	 
	 
	 
	 
	 Lcd4_Init();
	 Lcd4_Clear();
	 Lcd4_Set_Cursor(1,1);
	 Lcd4_Write_String("PATIENT ");
	 Lcd4_Set_Cursor(2,1);
	 Lcd4_Write_String("MONITORING      ");
	 Lcd4_Set_Cursor(3,1);
	 Lcd4_Write_String("SYSTEM");
	 _delay_ms(1000);
	 _delay_ms(1000);
	 Lcd4_Clear();
	 InitADC();
	 _delay_ms(100);
	 
	 checkTempHeart();
	 
	 
}
	 
	 
	
	 
 
 void InitADC()
 {
	 ADMUX=(1<<REFS0);                       // For Aref=AVcc;
	 ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescalar div factor =128
 }

 uint16_t ReadADC(uint8_t ch)
 {
	 //Select ADC Channel ch must be 0-7
	 ch=ch&0b00000111;
	 ADMUX|=ch;
	 //Start Single conversion
	 ADCSRA|=(1<<ADSC);
	 //Wait for conversion to complete
	 while(!(ADCSRA & (1<<ADIF)));
	 //Clear ADIF by writing one to it
	 //Note you may be wondering why we have write one to clear it
	 //This is standard way of clearing bits in io as said in data sheets.
	 //The code writes '1' but it result in setting bit to '0' !!!
	 ADCSRA|=(1<<ADIF);
	 return(ADC);
 }
 
 
 
 unsigned char rxvalue(void){
	 while(!(UCSRA&(1<<RXC)));
	 {
		 rxdata=UDR;
		 return rxdata;
	}
 }
 

//heart rate	
int readHeartRate()
{
	int time = 0;
    //return 5;
	//wait for the start of the pulse
	/*if(digital_read(PINB,PB0)){
    //return 3;
		while(digital_read(PINB,PB0))
		_delay_ms(0.1);
	}*/
	
	if(!digital_read(PINB,PB0)){
    //return 2;
		while(!digital_read(PINB,PB0))
		_delay_ms(0.1);
	}
	
	//start counting the time of pulse
	count_start:
	
	if(digital_read(PINB,PB0) )
	{
        //return 1;
		while(digital_read(PINB, PB0))
		{
			time++;
			_delay_ms(0.1);
		}
		//find correct value instead of 100
         //return time/180;
      
		return time/150;
       
        
	}
	else
	{
		//wait till the next pulse
		while(!digital_read(PINB,PB0))
		_delay_ms(1);
		goto count_start;
	}
	
}


void uart_init (void)
{
	UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
	UBRRL = BAUDRATE;                           // set baud rate
	UCSRB|= (1<<TXEN)|(1<<RXEN);                // enable receiver and transmitter
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}
void uart_transmit_char (unsigned char data)
{
	while (!( UCSRA & (1<<UDRE)));                // wait while register is free
	UDR = data;                                   // load data in the register
}
void uart_transmit_string (const unsigned char* st)
{
	
	
		while(*st)
		uart_transmit_char(*st++);
	                         // load data in the register
}
	
	
	 void checkTempHeart(){
		 
		  char t[5];
          int c=0;
          char buffer[10];
		  Lcd4_Set_Cursor(1,1);
		  Lcd4_Write_String("Temperature:    ");
		  _delay_ms(1000);
		  _delay_ms(1000);
		  Lcd4_Set_Cursor(3,1);
		  Lcd4_Write_String("HEART RATE  ");
		  _delay_ms(1000);
		  _delay_ms(1000);
		  
		  while(1){
	
		 
		  
			  InitADC();
			  adc_result0=ReadADC(0);
			  dtostrf(adc_result0/1024.0*500,5,2,t);
			  Lcd4_Set_Cursor(2,0);
			  Lcd4_Write_String(t);
			  Lcd4_Set_Cursor(2,6);
			  Lcd4_Write_Char(223);
			  Lcd4_Set_Cursor(2,7);
			  Lcd4_Write_String("C");
			  Lcd4_Write_String("        ");
			  _delay_ms(1000);
			  
	
		
			  //InitADC();
              //int heart_rate=ReadADC(0);
			  int heart_rate = readHeartRate();
			  sprintf(buffer, "%d", heart_rate);
			 // itoa(heart_rate,buffer,10);
			  //itoa(c,buffer,10);
              Lcd4_Set_Cursor(4,1);
			  Lcd4_Write_String(buffer);
			  //int k = heart_rate;
			  _delay_ms(1000);
			  _delay_ms(1000);
			  //Lcd4_Clear();
			  /*while (bit_is_clear(PINB,0))
				{
					++c;
					_delay_ms(500);
		 
				}
			  /*PORTB=0x00;
	 
			  _delay_ms(1000);
	 
	 
			}*/
			  
			  
				
			  if(adc_result0/1024.0*500>37.2||heart_rate>80)//if temp is larger or heartbeat is larger
			  
					{
			  
						PORTC|=1<<PINC0;
						sendMSG();
						
					}
			  
			  if(bit_is_set(PINC,1))
					{
				  //Lcd4_Clear();
				  PORTC&=~(1<<PINC0);
				  
					}

			
		  }
		  
		  
		 } 
 

			
	 
int count =0;
char buffer1[10];
ISR(INT2_vect)
{
	
	if (bit_is_set(PINB,2))
	{
		count++;
		_delay_ms(900);
	}
	Lcd4_Set_Cursor(1,1);
	itoa(count,buffer1,10);
	Lcd4_Write_String(buffer1);
	
}

void sendMSG(){

		//itoa(z,value1,10);
		//itoa(k,value2,10);

	 Lcd4_Clear();
	 Lcd4_Set_Cursor(1,1);
	 Lcd4_Write_String("Sending MSG");
	 _delay_ms(1000);
	 Lcd4_Clear();
	 Lcd4_Write_String("Setting UART....");
	 uart_init();
	 uart_transmit_string("AT+CMGF=1");
	 uart_transmit_char((char)13);
	 _delay_ms(1000);
	 uart_transmit_string("AT+CMGS=\"+94718724671\"\r");
	 _delay_ms(1000);
	 uart_transmit_string("Your patient is in a trouble");
	

	 _delay_ms(1000);
	 uart_transmit_char((char)26);
	 _delay_ms(1000);
	 //Lcd4_Write_String("O");
	 _delay_ms(1000);
	 _delay_ms(1000);
	 Lcd4_Clear();
	 checkTempHeart();

}


