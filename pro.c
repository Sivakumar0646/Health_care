#include<pic.h>
#include<stdio.h>
__CONFIG(0X3F72);

#define rs RD1
#define rw RD2
#define en RD3

void heart_beat();
void transmit();
void temp();
void lcd_display(const unsigned char *m);
void lcd_data(unsigned char data);
void lcd_init();
void delay(unsigned long int x);
void lcd_cmd(unsigned char com);

unsigned long int r1;
unsigned char ad1[5];
char data[5]="T:";

unsigned char i,COUNT;
unsigned int CNT,val1,val2,val3=0,val4,val5=0,VAL1,VAL2,val6,val7;
unsigned int val22,val33=0,val44,val55=0,val8;
unsigned char a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,z;
unsigned int VAL7=0,VAL5=0,VAL3=0,VAL33=0,VAL8=0,VAL6,VAL88=0,COUNT1;

void main()
{
ADCON1=0XCE;//digital_port_E
TRISA=0X07;
PORTA=0X00;
TRISC=0X80;
PORTC=0X20;
TRISD=0X00;
PORTD=0X00;

TXSTA=0X24;
RCSTA=0X90;
SPBRG=25;

lcd_init();

lcd_cmd(0x80);
lcd_display("Temp:");
lcd_cmd(0x87);
lcd_data(0xdf);
lcd_data('c');
lcd_cmd(0xC0);
lcd_display("HB:");
delay(10000);
while(1)
{
	temp();
	delay(100);
	heart_beat();
	delay(100);
	transmit();
	delay(10000);
}

}

void delay(unsigned long int x)
{
while(x--);
}

void lcd_cmd(unsigned char com)
{
unsigned char temp;
PORTD=com&0xf0;
rs=0;
rw=0;
en=1;
delay(100);
en=0;
temp=com<<4;
PORTD=temp&0xf0;
rs=0;
rw=0;
en=1;
delay(100);
en=0;
}

void lcd_data(unsigned char data)
{
unsigned char val1;
PORTD=data&0xf0;
en=1;
rs=1;
rw=0;
delay(10);
en=0;
val1=data<<4;
PORTD=val1&0xf0;
en=1;
rs=1;
rw=0;
delay(10);
en=0;
}

void lcd_init()
{
lcd_cmd(0x02);
lcd_cmd(0x2c);
lcd_cmd(0x06);
lcd_cmd(0x0c);
lcd_cmd(0x01);
}

void lcd_display(const unsigned char *m)
{
for(int n=0;m[n]!=0;n++)
{	
lcd_data(m[n]);
}
}

void temp()
{

	
	CHS2=0;
	CHS1=0;
	CHS0=0;
	delay(1000);
	ADON=1;
	ADCON0=ADCON0|0X04;
	while(ADIF==0);
	ADIF=0;
	r1=ADRESL|ADRESH<<8;
	r1=r1/2.048;
	sprintf(ad1,"%02lu",r1);
    lcd_cmd(0x85);
	for(int i=0;ad1[i]!='\0';i++)
	lcd_data(ad1[i]);
}


void transmit()
{
	TXREG='*';
	while(TXIF==0);
	TXIF=0;
		
	for(char i=0;data[i]!='\0';i++)
	{
	TXREG=data[i];
	while(TXIF==0);
	TXIF=0;
	}	

	for(char i=0;ad1[i]!='\0';i++)
	{
	TXREG=ad1[i];
	while(TXIF==0);
	TXIF=0;
	}
	
	TXREG=0xdf;
	while(TXIF==0);
	TXIF=0;

	TXREG='c';
	while(TXIF==0);
	TXIF=0;

	TXREG='H';
	while(TXIF==0);
	TXIF=0;

	TXREG=(val44+0X30);
	while(TXIF==0);
	TXIF=0;

	TXREG=(val22+0X30);
	while(TXIF==0);
	TXIF=0;
	
	for(char i=0;i<3;i++)
	{
	TXREG=' ';
	while(TXIF==0);
	TXIF=0;
	}

	TXREG='#';
	while(TXIF==0);
	TXIF=0;

/*	
	for(char i=0;i<15;i++)
	{
	TXREG=data[i];
	while(TXIF==0);
	TXIF=0;
*/

/*
	unsigned int s;
	for(s=0;data[s]!=0;s++)
	{
	while(!TXIF)
	{

	}
	//delay2();
	OERR=0;


	TXREG=data[s];

	for(int i=0;i<1200;i++);

*/
	}
void heart_beat()
{
while(CNT<=6)
{
TMR1ON=1;	

TMR1CS=0;     // TIMER MODE
T1CKPS1=1;    // SETTING  PRESCALAR VALUE  AS 8
T1CKPS0=1;
TMR1H=0x0B;   
TMR1L=0xDB;

while(TMR1IF==0)
{
if((RB0==1)&&(VAL8==0))
{
VAL8=1;
COUNT++;
delay(10000);
}
if(RB0==0)
{
VAL8=0;
}

}

if(TMR1IF==1)
{
TMR1IF=0;
CNT=CNT+1;
TMR1ON=0;
}
}

COUNT1=COUNT;
CNT=0;
if(COUNT<6)
{
COUNT1=COUNT1*12;
}
if(COUNT>6)
{
COUNT1=COUNT1*10;
}
if(COUNT==0)
{
COUNT1=0;
}
  val22=COUNT1%10;//UNIT DIGIT
  val33=COUNT1/10;
  val44=val33%10;    // tens digit
  val55=val33/10;
  
  // hundred digit 
lcd_cmd(0xC3);
lcd_data(val44+0x30);
lcd_data(val22+0x30);

//COUNT1=(((val44-48)*10)+(val22-48));
COUNT1=90;
if(COUNT1>0)
{
if((COUNT1<100)&&(COUNT1>59))
{
lcd_cmd(0xC8);
lcd_display("HB   NORMAL ");
delay(50000);delay(50000);delay(50000);
}
else
{
lcd_cmd(0xC8);
lcd_display("HB ABNORMAL ");
delay(50000);delay(50000);delay(50000);
}
}
 }