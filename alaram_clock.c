#include<xc.h>                 
int flag = 0;
//WORKING latest
void delay_ms(int ms);
void delay_us(int us);
void Data(int Value);           /*  LCD Data Function Declaration                        */
void Cmd(int Value);            /*  LCD Command Function Declaration                     */
void Send2Lcd(const char Adr, const char *Lcd);


void __interrupt() adcint()
{
	 GIE   = 0;
	if (ADIF == 1)
    {
        flag=1;
        ADIF=0;        
    }
	GIE   = 1;
}
void ky()
{
    TRISA=0x00;
    PORTA=0X00;
    TRISB=0X1F;
    nRBPU=0;
    PCFG3=0;PCFG2=1;PCFG1=1;PCFG0=0;
}
void lcd()
{
    TRISC=0X00;                /*  (RC1,RC0 ->O/P Setting by Zero)                      */
    TRISD=0X00;                /*  PORTD (0 - 7)pins Config as Output                   */
    TRISB=0x00;
    PORTB=0x00;
    PORTC=0x00;
    delay_ms(15);              /*  Minimum Delay To Power On LCD Module To Recieve Mode */
     Cmd(0X30);	delay_ms(5);    /*  LCD Specification Commands                           */
     Cmd(0X30);	delay_ms(1);    /*  LCD Specification Commands                           */
     Cmd(0X30);	delay_ms(2);    /*  LCD Specification Commands                           */
     Cmd(0X38);                 /*  LCD Double Line Display Command                      */
     Cmd(0X06);                 /*  LCD Auto Increment Location Address Command          */
     Cmd(0X01);                 /*  LCD Display Clear Command                            */
     Cmd(0X0C);                 /*  LCD Display ON Command                               */
}
void Cmd(int Value)
{
     PORTD=Value;
     RC1=0;                     /*  RC1=0(RS=0)		[Command Registr Selection])    */
     RC0=0;                     /*  RC0=0(R/W=0)	[Write Process])                */
     RC2=1;                     /*  RC2=1(Enable=1)	[Enable Line ON]                */
     delay_ms(4);               /*  Minimun Delay For Hold On Data                  */
     RC2=0;                     /*  RC2=0(Enable=0)	[Enable Line OFF]               */
}	
void Data(int Value)
{
     PORTD=Value;
     RC1=1;                     /*  RC1=1(RS=1)		[Data Registr Selection])       */
     RC0=0;                     /*  RC0=0(R/W=0)	[Write Process])                */
     RC2=1;                     /*  RC2=1(Enable=1)	[Enable Line ON]                */
     delay_ms(4);               /*  Minimun Delay For Hold On Data                  */
     RC2=0;                     /*  RC2=0(Enable=0)	[Enable Line OFF]               */
}
void Send2Lcd(const char Adr, const char *Lcd)
{
     Cmd(Adr);
     while(*Lcd!='\0')	{	Data(*Lcd);		Lcd++;	}
     
}
void delay_ms(int ms)
{
	int i,count;
	
	for(i=1;i<=ms;i++)
	{
		count=1000;
		while(count!=1)
		{
			count--;
		}
	}
}
void main()
{
    unsigned int hr=11,min=18,sec=45,i,hra,hrb,ma,mb,sa,sb;
    unsigned int sh=11,sm=19,ss=0;
    lcd();
    Send2Lcd(0x84,"WELCOME");	/*  LCD Data Outing Function                     */
    delay_ms(3);
    delay_ms(1000);
    Cmd(0x01);
    Send2Lcd(0x80,"MCA mini proj");
     delay_ms(3);
     delay_ms(1);
     Send2Lcd(0xC0,"Digital Clock ");
     delay_ms(3);
     delay_ms(1000);
     Cmd(0x01);
     Send2Lcd(0x80,"Today's Date ");
     delay_ms(3);
     delay_ms(1);
     Send2Lcd(0xC0,"14th June 2023");
     delay_ms(3);
     delay_ms(1000);
     Cmd(0x01);
    Send2Lcd(0x80,"Current Time: ");
    while(1)
    {
        sec++;
        if(sec==60)
        {
            sec = 0;
            min++;
        }
        if(min==60)
        {
            min = 0;
            hr++;
        }
        if(hr==12)
            hr=0;
        hra = hr/10;
        hrb = hr%10;
        ma = min/10;
        mb = min%10;
        sa = sec/10;
        sb = sec%10;
        Cmd(0xC0);
        Data(hra + '0');
        Data(hrb + '0');
        Send2Lcd(0xC2,":");
//        Data(":",'0');
        Data(ma + '0');
        Data(mb + '0');
        Send2Lcd(0xC5,":");
//        Data(":",'0');
        Data(sa + '0');
        Data(sb + '0');
        delay_ms(500);
        ky();
//       
        if((sh==hr)&&(sm==min)&&(ss==sec))
        {
            TRISE = 0X00;
            PORTE = 0XFF;
            delay_ms(500);
            delay_ms(500);
            PORTE=0X00;
            RB7=1;
            RB6=1;
            RB5=0;
            if(RB1==0)
                PORTE = 0X00;
            
        }
    }
}
