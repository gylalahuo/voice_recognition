#include "mbed.h"
#include "Message.h"
#include "TCPSocket.h"
#include "MXCHIPInterface.h"
#include "MbedJSONValue.h"
#include "fsl_usart.h"
#include  "dmic_test.h"
#include  "Flash.h"
#include <string>
#define SIMPLE_POST_REQUEST2 \
	"POST /server_api?lan=zh&cuid=30-10-B3-4A-D9-13&token=24.31aea138d109b14113de97b7769c5552.2592000.1511531377.282335-9731315 HTTP/1.1\r\n" \
   	 "Host:vop.baidu.com\r\n" \
    	"Content-Type: audio/pcm;rate=8000;\r\n"\
	"Content-Length:%d\r\n" \
	"Connection:Keep-Alive:9000\r\n" \
    	"\r\n" \

#define SIMPLE_GET_REQUEST1 \
	"GET https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=CvKkS689K2PCzhQPY4AbWXHy&client_secret=804ba5c62e2ad6fec6e3de25414b76c2 HTTP/1.1\r\n" \
    	"Host: https://openapi.baidu.com\r\n" \
	"Content-Type:application/json;\r\n"\
	"\r\n" \	
		
MXCHIPInterface wifi(D1,D0,921600);


DigitalOut led1(LED1); 
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
Serial pc(USBTX,USBRX,1000000);
Message  r_message;
dmic_test dm;
TCPSocket tcpsocket;

int speech_length;
int length;
int b_length;
uint16_t dmic_buffer[AUDIO_SAMPLES_STEP_ALGO*2]={0};
int times;
int flag=1;
int time1=1;
int flag1=0;
int flag3=0;
volatile char a[1024];
int num=0;
SPI_MX25R spi_mx25r(P0_20,P0_18,P0_19,P1_2);
bool c=true;
Ticker ticker;
Ticker ticker1;
DigitalIn button(SW3);	
int button1=0;
int rcount;
char req[300]="0";
char rbuffer[1024]="0";
uint8_t buffer[2048]="0";
unsigned char *speech_data=NULL;
char *content=NULL;
void rdRange(unsigned long rd_start_address, unsigned long rd_end_address)
{			  
	unsigned int i, j ;
	unsigned long current_address = rd_start_address;	
	for (i = rd_start_address ; current_address < rd_end_address ; i++ ) {    
		speech_data=spi_mx25r.read2K(current_address,16384);
		current_address = current_address + 16384;			
		for(j=0;j<16;j++)
		{
			tcpsocket.send(speech_data+j*1024,1024);
		}		
		delete []spi_mx25r.data1;
		spi_mx25r.data1=NULL;       
         }				
}

						
void  detect1()
{
	dm.DoVoiceTriggerProcess();
}
void detect()
{
	char *a="hello";
	if(time1==0)
	{
		num++;
	}
	else {
		num=0;	
		time1=0;
	}
	if(num>2&&times>20)
	{
		c=0;
	}
		
}

void save_data()
{
	if(flag1==1)
	{
		for(int i=0;i<1024;i++)
		 {
			buffer[i*2+1]=( (dmic_buffer[i]>>8) &0x00ff );
			buffer[i*2+0]=( dmic_buffer[i] &0x00ff );
		 }
		spi_mx25r.writeEnable();
		while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,buffer,256);
		while((spi_mx25r.readStatus()&0x03));
		times++;
		spi_mx25r.writeEnable();
		while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[256],256);
		while((spi_mx25r.readStatus()&0x03));			 
		times++;
		spi_mx25r.writeEnable();
		while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[512],256);
		while((spi_mx25r.readStatus()&0x03));
		times++;
		spi_mx25r.writeEnable();
		while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[768],256);
		while((spi_mx25r.readStatus()&0x03));			 
		times++;	
			 
		spi_mx25r.writeEnable();
		while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1024],256);
		while((spi_mx25r.readStatus()&0x03));			 
		times++;
		spi_mx25r.writeEnable();
		while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1280],256);
		while((spi_mx25r.readStatus()&0x03));
		times++;
		spi_mx25r.writeEnable();
		while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1536],256);
	  while((spi_mx25r.readStatus()&0x03));			 
		times++;	
		spi_mx25r.writeEnable();
		 while(!(spi_mx25r.readStatus()&0x02));
		spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1792],256);
		while((spi_mx25r.readStatus()&0x03));
		times++;			
		}
		else if(flag1==2)
		{			
			 for(int i=0;i<1024;i++)
			 {
					buffer[i*2+1]=( (dmic_buffer[AUDIO_SAMPLES_STEP_ALGO+i]>>8) &0x00ff );
				  buffer[i*2+0]=( dmic_buffer[AUDIO_SAMPLES_STEP_ALGO+i] &0x00ff );
			 }
			 spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,buffer,256);
			 while((spi_mx25r.readStatus()&0x03));
			 times++;
			 spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[256],256);
			 while((spi_mx25r.readStatus()&0x03));			 
			 times++;
			spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[512],256);
			 while((spi_mx25r.readStatus()&0x03));
			 times++;
			 spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[768],256);
			 while((spi_mx25r.readStatus()&0x03));			 
			 times++;	
			 
			 spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1024],256);
			 while((spi_mx25r.readStatus()&0x03));			 
			 times++;
			 spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1280],256);
			 while((spi_mx25r.readStatus()&0x03));
			 	times++;
			 spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1536],256);
			 while((spi_mx25r.readStatus()&0x03));			 
			 times++;	
			 spi_mx25r.writeEnable();
			 while(!(spi_mx25r.readStatus()&0x02));
			 spi_mx25r.programPage(0+times*256,(uint8_t *)&buffer[1792],256);
			 while((spi_mx25r.readStatus()&0x03));
			times++;			 
			}		
			flag1=0;		
}

void control_led(char *a)
{
	int tmp=0;
	uint8_t *b=NULL;
	uint8_t  c[40]={0};
	b=c;
	int i=0;
	while(*a)
	{
		tmp=(int)*a;
		c[i]=tmp;
		i=i+1;
		a=a+1;
	}
	while(*b)
	{
		if(*b==191&&*(b+1)==170&&*(b+2)==181&&*(b+3)==198)
		{
			pc.printf("success");
		
		}
		else
		{
			b=b+2;
		}
	}
}


int connect(char *uri)
{
		tcpsocket.open(&wifi);	
    const char * host_ip1=wifi.gethostbyname(uri);
	  pc.printf("host_ip=%s\r\n",host_ip1);
	  wifi.addre=host_ip1;  
//	host_ip1="111.13.105.93";
	
    int value=tcpsocket.connect(host_ip1, 80);
	  return value;
}


int main( void )
{   
	  led1=1;	
		led2=1;
	  led3=1;	
	  led4=1;
		USART_EnableInterrupts(USART2, kUSART_RxLevelInterruptEnable|kUSART_RxErrorInterruptEnable);
    EnableIRQ(FLEXCOMM2_IRQn);
	  while(wifi.connect("wifi", "916754393"));		 
    /***** get token 	 
		char uri[20]="openapi.baidu.com";
		connect(uri);
		tcpsocket.send(SIMPLE_GET_REQUEST1, strlen(SIMPLE_GET_REQUEST1));
		int length= sizeof rbuffer;
		rcount = tcpsocket.recv(rbuffer, sizeof rbuffer);
		tcpsocket.close();
		printf("Done\r\n");
		char* content1=NULL;
		content1=r_message.Http_get_content(rbuffer);
		pc.printf("%s",content1);
		char *token_data="access_token";
		token=r_message.Http_get_Message(content1,token_data,2);
	  tcpsocket.close();
		*******/ 
		char uri1[20]="vop.baidu.com";
   
	  dm.DMIC_init1();
		dm.DoVoiceTriggerProcess();
	  detect1();
		while(1)
		{		
			c=1;
			ticker.attach(&detect,0.2);		
		  times=0;			
			for(int i=0;i<3;i++)
			{
       	  spi_mx25r.writeEnable();
					spi_mx25r.blockErase(0+64*1024*i);
			}
			pc.printf("erase\r\n");	
			num=0;	
			flag3=0;
			while(c)
			{	 
        if(flag3)
				{					
					save_data();	
				}		
			
				wait_ms(5);				
			} 
			if(times>480)
			{
				break; //the voice recode exceed 15s discard it. 
		
			}
			ticker.detach();
			Timer t;		
			t.start();
			sprintf(req,SIMPLE_POST_REQUEST2,256*times);
			
			while(connect(uri1)!=0);
			tcpsocket.send(req, strlen(req));
			rdRange(0,256*(times));
			rcount = tcpsocket.recv(rbuffer, sizeof rbuffer);
			pc.printf("recv=%s\r\n",rbuffer);
			if(strstr(rbuffer,"OK"))
			{
				content=r_message.Http_get_content(rbuffer); 
				if(content!=NULL)
				{			
					char *data="result";
					char *result=NULL;
					char str2[ ][3]={0};				
					result=r_message.Http_get_Message(content,data,1);
					pc.printf("t=%d\r\n",t.read_ms());
					t.stop();
					if(result)
					{
						int len=strlen(result);
						for(int i=0;i<strlen(result);i++)
						{
							str2[i][2]='\0';
						}
						for(int i=0;i<len;i++)
						{
							sprintf(str2[i],"%x", result[i]); // closed  e5 85 b3   e7 81 af  badeng dakai	  open e5 bc 80					
						}					
						for(int i=0;i<len-2;i++)
						{
							if(strcmp(str2[i],"e5")==0&&strcmp(str2[i+1],"85")==0&&strcmp(str2[i+2],"b3")==0)
							{						
									led1=1;
									break;
							}
							else if(strcmp(str2[i],"e5")==0&&strcmp(str2[i+1],"bc")==0&&strcmp(str2[i+2],"80")==0)
							{						
									led1=0;
									break;
							}				
						}
					}	
				}
		   }
		memset(req,0,300);
		memset(rbuffer,0,2048);
		tcpsocket.close();
	}		
}

