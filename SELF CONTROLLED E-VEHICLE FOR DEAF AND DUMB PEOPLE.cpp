

#include<pic.h>

 	CONFIG(0X3F72);
 

#define rs RD1 #define rw RD2 #define en RD3

#define relay_1 RC2 #define relay_2 RC3 #define relay_3 RC4 #define relay_4 RC5

#define C_LDR RA1 #define R_LDR RA2 #define L_LDR RA3

#define sw_1 RA5 #define sw_2 RE0 #define sw_3 RE1 #define sw_4 RE2

#define vp_1 RB1 #define vp_2 RB2 #define vp_3 RB3 #define IR RC0
 
unsigned int I;
unsigned int turn=0,turn_r=0;
// rfid_variables unsigned int j=0,i=0;
unsigned char d_v=0,d_v1=1,ser,ser1; unsigned char rfid_arr[15];

unsigned int T=0,Z=0,rf_data,ser2;// rf


// local variables
unsigned int dest_1=0, dest_2=0,dest_3=0,dest_h=0; unsigned int cut_1=0,cut_2=0,cut_3=0,cut_h=0; unsigned int dv_1=0,dv_2=0,dv_3=0,dv_4=0;

unsigned int ds_1s,ds_2s,ds_3s,ds_hs; unsigned int ip_g=0;

unsigned	int	cut_dv=0,	lcut_dv=0,	rcut_dv=0, cut_abt=0,cut_count=0;

unsigned int turn_v=0;




void delay(unsigned int X)
 
{
while(X--);
}



void lcd_command(unsigned int COM )
{
unsigned char temp; PORTD=COM&0Xf0;
rs=0; rw=0; en=1; delay(200); en=0;
temp=COM<<4; PORTD=temp&0Xf0; rs=0;
rw=0; en=1; delay(200); en=0;
}


void lcd_data(unsigned int data)
{
 
unsigned char val1; PORTD=data&0xf0; rs=1;
rw=0; en=1;
delay(1000); en=0; val1=data<<4;
PORTD=val1&0Xf0; rs=1;
rw=0; en=1;
delay(1000); en=0;
}


void lcd_init()
{
lcd_command(0X02); lcd_command(0x2c); lcd_command(0X06); lcd_command(0X0c); lcd_command(0X01);
}
 

void lcd_display(const unsigned char*m)
{
for(int I=0;m[I]!=0;I++)
{
lcd_data(m[I]);
}
}


// UART INTIALIZATION
void UART_init()
{ TXSTA=0X24; RCSTA=0X90; SPBRG=25; BRGH=1; GIE=1; PEIE=1; RCIE=1; INTE=1; INTEDG=1;
}


// interrupt function void interrupt rcx(void)
 
{ if(INTF==1)
{ INTF=0; T=1;
} if(RCIF==1)
{ RCIF=0;
ser=RCREG; d_v=1;
if((d_v==1)&&(i<15))
{
rfid_arr[i]=ser; i=i+1;
}
}
}

// RFID display function void rfid_display()
{
if(i>11)
{ i=0;
 
lcd_command(0x01); lcd_command(0x80); delay(1000); for(j=0;j<=11;j++)
{
ser1=(rfid_arr[j]); lcd_data(ser1); delay(600);
}
delay(3000); if((rfid_arr[7]=='2')&&(rfid_arr[8]=='1'))
{
dest_1=1; dest_2=0; dest_3=0; dest_h=0; delay(1000);
lcd_command(0x01); delay(100); lcd_command(0x80); delay(100);
lcd_display("1-DESTINATION"); delay(50000);
d_v=0;
}
 

if((rfid_arr[7]=='F')&&(rfid_arr[8]=='4'))
{
dest_1=0; dest_2=0; dest_3=0; dest_h=1; delay(1000);
lcd_command(0x01); delay(100); lcd_command(0x80); delay(100);
lcd_display("H-DESTINATION"); delay(50000);
delay(50000); d_v=0;
}
if((rfid_arr[7]=='4')&&(rfid_arr[8]=='1'))
{
dest_1=0; dest_2=1;
//dest_3=1; dest_h=0; delay(1000); lcd_command(0x01);
 
delay(100); lcd_command(0x80); delay(100);
lcd_display("2-DESTINATION"); delay(50000);
delay(50000); d_v=0;
}
}
}


void keypad()
{
ip_g=0; delay(1000); while(ip_g==0)
{
if(sw_1==1)
{
while(sw_1==1); ip_g=1; ds_1s=1; ds_2s=0; ds_hs=0; delay(10000);
 
lcd_command(0XC0); lcd_display("st-1 selected"); vp_2=0;
delay(10000); vp_2=1;
}
if(sw_2==1)
{
while(sw_2==1); ds_2s=1; ds_1s=0; ds_hs=0; ip_g=1; delay(10000);
lcd_command(0XC0); lcd_display("st-2 selected"); vp_3=0;
delay(10000); vp_3=1;
}
if(sw_3==1)
{
while(sw_3==1); ds_3s=1; ip_g=1;
 
delay(10000); lcd_command(0XC0); lcd_display("st-3 selected"); delay(10000);
}
if(sw_4==1)
{
while(sw_4==1); ds_hs=1; ip_g=1; delay(10000);
lcd_command(0XC0); lcd_display("st-H selected"); vp_1=0;
delay(10000); vp_1=1;
}
}
}
// RF data recieve function void rf_receive()
{ if(T==1)
{ T=0;
 
rf_data = PORTB; rf_data = rf_data>>4; rf_data=(rf_data&0X0F); Z=rf_data; lcd_command(0XCE); lcd_data(Z+0X30); delay(1000);
}
}

//************************

// LINE FOLLOW FUNCTION
//************************ void line_following()
{
lcd_command(0X80);
lcd_display(" LINE FOLLOWING "); if((R_LDR==0)&&(L_LDR==0)&&(C_LDR==1))
{
dv_4=1; lcd_command(0XC0); lcd_display(" FORWARD "); delay(200);
relay_1=1; relay_2=0;
 
relay_3=1; relay_4=0; delay(200); if(IR==0)
{
lcd_command(0X01); lcd_command(0X80);
lcd_display(" OBSTACLE DETECTED "); relay_1=0;
relay_2=0; relay_3=0; relay_4=0; while(IR==0); delay(1000); lcd_command(0X01); lcd_command(0X80);
lcd_display(" OBSTACLE CLEARED ");
}
} if((R_LDR==1)&&(L_LDR==0))
{
dv_4=1; lcd_command(0XC0); lcd_display("LEFT	"); delay(200);
 
while(R_LDR==1)
{
relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000); relay_1=1; relay_2=0; relay_3=1; relay_4=0; delay(2000); relay_1=0; relay_2=1; relay_3=1; relay_4=0; delay(10000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000);
}
}
 
if((R_LDR==0)&&(L_LDR==1))
{
dv_4=1; lcd_command(0XC0); lcd_display("RIGHT	"); delay(200); while(L_LDR==1)
{
relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000); relay_1=1; relay_2=0; relay_3=1; relay_4=0; delay(2000); relay_1=1; relay_2=0; relay_3=0; relay_4=1; delay(10000); relay_1=0; relay_2=0;
 
relay_3=0; relay_4=0; delay(30000);
}
} if((R_LDR==0)&&(L_LDR==0)&&(C_LDR==0))
{
relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(10000); cut_dv=1; delay(10000);
}
}
//************************
// LEFT TURN FUNCTION
//************************


void left_cut()
{
dv_2=1; delay(1000);
lcd_command(0X01);
 
lcd_command(0XC0); lcd_display("LEFT-TURN	"); relay_1=0;
relay_2=0; relay_3=0; relay_4=0; delay(10000);

relay_1=1;// frwd relay_2=0; relay_3=1; relay_4=0; delay(40000); relay_1=0;// left relay_2=1; relay_3=1; relay_4=0; delay(10000);

relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(10000); while(dv_2==1)
 
{
lcd_command(0XC0); lcd_display("LEFT	"); relay_1=0;
relay_2=0; relay_3=0; relay_4=0; delay(30000); relay_1=1;// frwd relay_2=0; relay_3=1; relay_4=0; delay(5000); relay_1=0;// left relay_2=1; relay_3=1; relay_4=0; delay(10000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000);
if((R_LDR==0)&&(L_LDR==0)&&(C_LDR==1))
{
 
lcd_command(0XC0); lcd_display("LEFT cut ok	"); delay(10000);
dv_2=0; delay(1000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(10000); lcut_dv=1; delay(10000); cut_h=0;
}
}
}
//************************
// RIGHT TURN FUNCTION


//************************ void right_cut()
{
dv_1=1; delay(1000); relay_1=1;
 
relay_2=0; relay_3=1; relay_4=0; delay(20000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(20000); lcd_command(0X01); lcd_command(0X80);
lcd_display("RIGHT CUT	"); delay(10000);
while(dv_1==1)
{
lcd_command(0XC0); lcd_display("RIGHT	"); relay_1=0;
relay_2=0; relay_3=0; relay_4=0; delay(30000); relay_1=1;// frwd relay_2=0; relay_3=1;
 
relay_4=0; delay(5000); relay_1=1;// right relay_2=0; relay_3=0; relay_4=1; delay(10000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000);
if((R_LDR==0)&&(L_LDR==0)&&(C_LDR==1))
{
rcut_dv=1; relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(20000); dv_1=0; delay(1000);
}
}
}
 






//************************
// ABOUT TURN FUNCTION
//************************ void about_turn()
{
delay(1000); relay_1=1;// right relay_2=0; relay_3=0; relay_4=1; delay(20000); lcd_command(0X01);
lcd_command(0XC0); lcd_display("REVERSE "); delay(10000);
relay_1=0;// reverse relay_2=1; relay_3=0; relay_4=1; delay(20000);
 
relay_1=1;// right relay_2=0; relay_3=0; relay_4=1; delay(10000);
lcd_command(0XC0); lcd_display(" FORWARD "); delay(200);
relay_1=1; relay_2=0; relay_3=1; relay_4=0; delay(2000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(10000); while(cut_abt==0)
{
lcd_command(0XC0); lcd_display("RIGHT	"); relay_1=0;
relay_2=0; relay_3=0;
 
relay_4=0; delay(30000); relay_1=1;// frwd relay_2=0; relay_3=1; relay_4=0; delay(5000); relay_1=1;// right relay_2=0; relay_3=0; relay_4=1; delay(15000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000);
if((R_LDR==0)&&(L_LDR==0)&&(C_LDR==1))
{
lcd_command(0XC0); lcd_display("TURN OK	"); relay_1=0;
relay_2=0; relay_3=0; relay_4=0;
 
delay(10000); cut_abt=1; delay(1000);
}
}
}




//************************
// PATH1 FUNCTION
//************************ void path_1()
{
while(cut_1==0)
{
line_following(); delay(100); if(cut_dv==1)
{
cut_dv=0; right_cut(); delay(1000);
 
}
if(rcut_dv==1)
{
cut_1=1; delay(1000); cut_dv=0; rcut_dv=0; delay(1000);
}
}
if(cut_1==1)
{
while(dest_1==0)
{
rfid_display(); delay(1000); line_following(); delay(100); if(dest_1==1)
{
relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(1000);
 
}
void path_2()
{
while(cut_2==0)
{
line_following(); delay(100);
if((cut_dv==1)&&(dv_4==1))
{
cut_count=cut_count+1; delay(10000); cut_dv=0;
dv_4=0; lcd_command(0X01); lcd_command(0XCA); lcd_display("C="); lcd_data(cut_count+0x30); relay_1=1;// frwd relay_2=0;
relay_3=1; relay_4=0; delay(8000);
}
if(cut_count==2)
{
 
cut_count=0; right_cut(); delay(1000);
}
if(rcut_dv==1)
{
cut_2=1; delay(1000); cut_dv=0; rcut_dv=0; delay(1000);
}
}
if(cut_2==1)
{
while(dest_2==0)
{
rfid_display(); delay(1000); line_following(); delay(100); if(dest_2==1)
{
delay(30000);
}
 
}
}
// HOME RETURN FUNCTION
//************************ void return_home()
{
lcd_command(0X01); lcd_command(0X80); lcd_display("HOME..."); delay(10000); about_turn();
relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(10000); cut_abt=0;

while(cut_h==0)
{
lcd_command(0X80);
lcd_display(" LINE FOLLOWING "); if((R_LDR==0)&&(L_LDR==0)&&(C_LDR==1))
{
lcd_command(0XC0);
 
lcd_display(" FORWARD "); delay(200);
relay_1=1; relay_2=0; relay_3=1; relay_4=0; delay(200);
} if((R_LDR==1)&&(L_LDR==0))
{
lcd_command(0XC0); lcd_display("LEFT	"); delay(200); while(R_LDR==1)
{
relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000); relay_1=1; relay_2=0; relay_3=1; relay_4=0; delay(2000);
 
relay_1=0; relay_2=1; relay_3=1; relay_4=0; delay(10000); relay_1=0; relay_2=0; relay_3=0; relay_4=0; delay(30000);
}
}


if((R_LDR==0)&&(L_LDR==1))
{
lcd_command(0XC0); lcd_display("RIGHT	"); delay(200); while(L_LDR==1)
{
relay_1=0; relay_2=0;
if((R_LDR==0)&&(L_LDR==0)&&(C_LDR==0))
{
relay_1=0;
 
relay_2=0; relay_3=0; relay_4=0; delay(10000); cut_dv=1; delay(10000);
}
if(cut_dv==1)
{
relay_1=0;relay_2=0;relay_3=0;relay_4=0; delay(1000);
cut_h=1; delay(1000); cut_abt=0; delay(1000); cut_dv=0; delay(1000);
}
}
while(cut_h==1)
{
left_cut(); delay(10000); if(lcut_dv==1)
{
 
cut_h=0; delay(10000); lcut_dv=0; delay(10000);
}
}
if((cut_h==0)&&(dv_2==0))
{
while(dest_h==0)
{
rfid_display(); delay(1000); line_following(); delay(1000); if(dest_h==1)
{
delay(1000); relay_1=0; lcd_command(0X01); lcd_command(0X80);
lcd_display("Home_Reached"); delay(50000);
}
}
void main()
 
{
ADCON0=0X40;	//ANALOG COMPILER ON ADCON1=0X8E;
TRISD=0X00; PORTD=0X00; TRISC=0X81; PORTC=0X00; TRISA=0XFE; PORTA=0X00; TRISB=0XF1; PORTB=0X00; TRISE=0X07; PORTE=0X00;
lcd_init(); delay(1000); UART_init();
if((dest_h==1)&&(Z==1))
{
if(turn_v==1)
{
about_turn(); delay(1000);
}
turn_v=0; path_1();
 
delay(1000); if(dest_1==1)
{
cut_1=0; delay(30000); lcd_command(0X01); lcd_command(0X80);
lcd_display("Enter destination..."); delay(10000);
}
keypad(); delay(10000); if(ds_hs==1)
{
ds_hs=0; return_home(); delay(10000); turn=1;
Z=0;
}
if(ds_2s==1)
{
ds_hs=0; return_home(); delay(10000);
 
about_turn(); delay(10000); path_2(); delay(1000); if(dest_2==1)
{
cut_2=0; delay(20000);delay(10000); lcd_command(0X01); lcd_command(0X80); lcd_display("DEST-2_Reached...");
} Z=0;
delay(1000);
}
}
} if(Z==0x02)
{
if(dest_h==0)
{
return_home(); delay(1000);
}
if((dest_h==1)&&(Z==2))
 
{
if(turn_v==1)
{
about_turn(); delay(1000);
}
turn_v=0; path_2(); delay(1000);

if(dest_2==1)
{
cut_2=0; delay(20000);delay(10000); lcd_command(0X01); lcd_command(0X80); lcd_display("Enter destination..."); delay(50000);
}
keypad(); delay(10000); if(ds_hs==1)
{
ds_hs=0; return_home();
 
delay(10000); Z=0;
}


if(ds_1s==1)
{
return_home(); delay(10000); about_turn(); delay(10000); path_1(); delay(1000); if(dest_1==1)
{
cut_1=0; delay(20000);delay(10000); lcd_command(0X01); lcd_command(0X80); lcd_display("DEST-1_Reached..."); delay(50000);
} Z=0;
delay(1000);
}
}
 
}
delay(1000);delay(1000); if(Z==0x04)
{
if(dest_h==1)
{
lcd_command(0X01); lcd_command(0X80); lcd_display("Home_Reached"); delay(50000);
}
if(dest_h==0)
{
return_home(); delay(50000);
}
}
}
}
