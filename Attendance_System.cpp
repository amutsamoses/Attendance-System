#include <iostream>
#include <conio.h>
#includegit <reg51>  
#include<string>  
  
//0000 to 7FFF  
  
sbit RS = P0^7;  
sbit EN = P0^6;  
  
sbit SDA = P1^0;  
sbit SCL = P1^1;  
sbit RELAY = P1^2;  
  
code unsigned char RFID_1[] = "34006C9C04C0"; //34006C9C04+NULL  
code unsigned char RFID_2[] = "34006C549C90";  
code unsigned char RFID_3[] = "1300F8FAC1D0";  
code unsigned char RFID_4[] = "34006CD5AD20";  
code unsigned char RFID_5[] = "420061231E1E";  
  
code unsigned char name_1[] = "SANJAY JAIN";  
code unsigned char name_2[] = "SHEKHAT HARSH";  
code unsigned char name_3[] = "DHOLARIYA RAKSHIT";  
code unsigned char name_4[] = "DIVYANG SINGH ";  
code unsigned char name_5[] = "NAKUL JAIN";  
  
unsigned char rs[15];  
  
unsigned int no_of_records;  
  
void delay()  
{  
    int j;  
    for(j=0;j<500;j++);  
}  
  
void long_delay()  
{  
    unsigned int j;  
    for(j=0;j<65000;j++);  
}  
  
void idelay()  
{  
    unsigned int j;  
    for(j=0;j<10000;j++);  
}  
  
void lcd_command(char lc)  
{  
    P2 = lc;  
    RS = 0;  
    EN = 1;  
    delay();  
    EN = 0;  
}  
  
void lcd_data(char ld)  
{  
    P2 = ld;  
    RS = 1;  
    EN = 1;  
    delay();  
    EN = 0;  
}  
  
void lcd_init()  
{  
    lcd_command(0x38);  
    lcd_command(0x0E);  
    lcd_command(0x01);  
}  
  
void serial_init()  
{  
     TMOD = 0x20;  
    SCON = 0x50;  
    TH1 = 0xFD;  
    TR1 = 1;  
}  
  
void transmit(unsigned char tx)  
{  
    SBUF = tx;  
    while(TI==0);  
    TI = 0;  
}  
  
void send_string(unsigned char *str)  
{  
    int j;  
    for(j=0;str[j]!='\0';j++)  
        transmit(str[j]);     
}  
  
unsigned char receive()  
{  
    char rx;  
    while(RI==0);  
    RI = 0;  
    rx = SBUF;  
    return(rx);  
}  
  
void lcd_string(char add,char *str)  
{  
    int j;  
    lcd_command(add);  
    for(j=0;str[j]!='\0';j++)  
        lcd_data(str[j]);     
}  
  
void start()  
{  
    SDA = 1;  
    SCL = 1;  
    SDA = 0;  
}  
  
void stop()  
{  
    SDA = 0;  
    SCL = 1;  
    SDA = 1;  
}  
  
void write(unsigned char w)  
{  
    int j;  
    SCL = 0;  
    for(j=0;j<8;j++)  
    {  
        if((w & 0x80)==0)  
            SDA = 0;  
        else  
            SDA = 1;  
        SCL = 1;  
        SCL = 0;  
        ww = w << 1;  
    }  
    SCL = 1;  
    SCL = 0;  
}  
  
unsigned char read()  
{  
    int j;  
    unsigned char r = 0x00;  
    SDA = 1;  
  
    for(j=0;j<8;j++)  
    {  
        SCL = 1;  
        rr = r << 1;  
        if(SDA == 1)  
            rr = r | 0x01;  
        SCL = 0;  
    }  
    return(r);  
}  
  
void ack()  
{  
    SDA = 0;  
    SCL = 1;  
    SCL = 0;  
}  
  
void nack()  
{  
    SDA = 1;  
    SCL = 1;  
    SCL = 0;  
}  
  
void rtc_read()  
{  
        unsigned char ss,mm,hh,day,mn,date,yr;  
        start();  
        write(0xD0);  
        write(0x00);          
        stop();  
        start();  
        write(0xD1);  
        ss = read();  
        ack();  
        mm = read();  
        ack();  
        hh = read();  
        ack();  
        day = read();  
        ack();  
        date = read();  
        ack();  
        mn = read();  
        ack();  
        yr = read();  
        nack();  
        stop();  
  
        rs[0] = hh/0x10 + 48;  
        rs[1] = hh%0x10 + 48;  
        rs[2] = ':';  
        rs[3] = mm/0x10 + 48;  
        rs[4] = mm%0x10 + 48;  
        rs[5] = ',';  
        rs[6] = date/0x10 + 48;  
        rs[7] = date%0x10 + 48;  
        rs[8] = '/';  
        rs[9] = mn/0x10 + 48;  
        rs[10] = mn%0x10 + 48;  
        rs[11] = '/';  
        rs[12] = yr/0x10 + 48;  
        rs[13] = yr%0x10 + 48;  
        rs[14] = '\0';  
}  
  
void rtc_init()  
{  
    start();  
    write(0xD0);  
    write(0x00);  
    write(0x00);  
    write(0x00);  
    write(0x13);  
    write(0x05);  
    write(0x12);  
    write(0x04);  
    write(0x12);  
    stop();  
  
}  
  
void write_records(unsigned char *str);  
void read_records();  
  
void main()  
{  
    unsigned char rec_data[13],i,t;  
  
    RELAY = 0;  
  
    lcd_init();  
    serial_init();  
    rtc_init();  
    idelay();  
    start();  
    write(0xA0);  
    write(0x7F);  
    write(0xFF);  
    stop();  
    start();  
    write(0xA1);  
    no_of_records = read();  
    nack();  
    stop();  
  
//  no_of_records = 0;  
  
    while(1)  
    {  
    start:  
        lcd_command(0x01);  
        lcd_string(0x80,"RFID ATTENDANCE");  
        lcd_string(0xC5,"SYSTEM");  
  
        j = 0;  
        while(1)  
        {  
            if(RI==1)  
            {  
                RI = 0;  
                t = receive();  
                if(t == '+')  
                {  
                    read_records();  
                    goto start;  
                }  
                else  
                {  
                    rec_data[j] = t;  
                    for(j=1;j<12;j++)  
                        rec_data[j] = receive();  
                    rec_data[j] = '\0';   
                    break;  
                }  
            }  
        }  
  
        j = strcmp(RFID_1,rec_data);    //match => j = 0  
  
        lcd_command(0x01);  
  
        if(j==0)  
        {  
            RELAY = 1;  
            lcd_string(0x80,name_1);  
            rtc_read();  
            lcd_string(0xC0,rs);  
            long_delay();  
            write_records(name_1);  
            RELAY = 0;  
            goto start;       
        }  
  
//  
        j = strcmp(RFID_2,rec_data);    //match => j = 0  
  
        if(j==0)  
        {  
            RELAY = 1;  
            lcd_string(0x80,name_2);  
            rtc_read();  
            lcd_string(0xC0,rs);  
            long_delay();  
            write_records(name_2);  
            RELAY = 0;  
            goto start;       
        }  
  
//  
        j = strcmp(RFID_3,rec_data);    //match => j = 0  
  
        if(j==0)  
        {  
            RELAY = 1;  
            lcd_string(0x80,name_3);  
            rtc_read();  
            lcd_string(0xC0,rs);  
            long_delay();  
            write_records(name_3);  
            RELAY = 0;  
            goto start;       
        }  
  
        j = strcmp(RFID_4,rec_data);    //match => j = 0  
  
        if(j==0)  
        {  
            RELAY = 1;  
            lcd_string(0x80,name_4);  
            rtc_read();  
            lcd_string(0xC0,rs);  
            long_delay();  
            write_records(name_4);  
            RELAY = 0;  
            goto start;       
        }  
  
        j = strcmp(RFID_5,rec_data);    //match => j = 0  
  
        if(j==0)  
        {  
            RELAY = 1;  
            lcd_string(0x80,name_5);  
            no_of_records = 0;  
            start();  
            write(0xA0);  
            write(0x7F);  
            write(0xFF);  
            write(0x00);  
            stop();  
            lcd_string(0xC0,"MEMORY CLEARED");  
            long_delay();  
            RELAY = 0;  
            goto start;       
        }  
        lcd_string(0x80,"ERROR");  
        lcd_string(0xC0,rec_data);  
        long_delay();  
    }  
}  