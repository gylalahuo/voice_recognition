#include <Message.h>
extern Serial pc;
Message::Message()
{
	http_content=NULL;
	result_data=NULL;
}
char* Message::Http_get_Message(char *message,char *def,int i)
{
    char * data=NULL;
	  const char  *json=NULL;
		MbedJSONValue demo;
	  if(message!=NULL)
		{
			json=message;
		}
		

    //parse the previous string and fill the object demo
    parse(demo, json);
	  std::string my_str;
	  switch (i)
		{
		case 1:  
			if(strcmp(demo["err_msg"].get<std::string>().c_str(),"success.")==0)
			{
//				my_str=  demo["result"].get<std::string>();
				data=(char*)demo["result"][0].get<std::string>().c_str();
				break ;
			}
			else
			{
				 data=NULL;
			}
			break; 
	   case 2:
			 my_str=  demo[def].get<std::string>(); 
		 	data=(char*)my_str.c_str();break;  
	 }
	 

	  return data;
}

char* Message::Http_get_content(char* message)
{    
			char *recv_message=NULL;
      if(message!=NULL)
			{
				int length=strlen(message);
				recv_message=message;
			}
	    while(recv_message)
			{
				if(*recv_message=='{')
				{
						return recv_message;
					    
				}
				else
				{
					recv_message=recv_message++;
				
				}
			
			}
			return NULL;

}

//char* Message::Http_get_content(char *uri,char *url,MXCHIPInterface wifi,TCPSocket tcpsocket)
//{   
//          printf("finish open()\r\n");
//  	printf("%s",url);
//    const char * host_ip1=wifi.gethostbyname(url);
//	  wifi.addre=host_ip1;
//    pc.printf("wifi.address=%s",host_ip1);
//    int value=tcpsocket.connect(host_ip1, 80);
//		if(!value)
//		{
//		}       
//    tcpsocket.send(SIMPLE_GET_REQUEST1, strlen(SIMPLE_GET_REQUEST1));
//    rcount = tcpsocket.recv(rbuffer, sizeof rbuffer);
//		tcpsocket.close();
//	   char message[600]={0};
//		 const char * host_ip=wifi.gethostbyname(url);
//		   wifi.addre=host_ip;
//	    int value1=tcpsocket.connect(host_ip, 80);
//		   
//		   if(!value1)
//		   {
//		   }
//		   tcpsocket.send(uri, strlen(uri));
//	     tcpsocket.recv(message, sizeof message);

//	    char* content;
//		  int length=strlen(message);
//			for(int i=0;i<length;i++)
//			{
//				  if(*(message+i)=='\r'&&*(message+1+i)=='\n'&&*(message+i+2)=='\r'&&*(message+i+3)=='\n')
//					{
//					    content=message+i;
//						  printf("%s\r\n",content);
//						  return content;					
//					}		
//			}
//		return NULL;
//}





