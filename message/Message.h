#ifndef MESSAGE_H
#define MESSAGE_H
#include "mbed.h"
#include "MXCHIPInterface.h"
#include "MbedJSONValue.h"
#include "TCPSocket.h"
#include <string>
//extern TCPSocket tcpsocket;
//extern MXCHIPInterface wifi;
static int d_num=0;
 static char *message_content;
class Message
{
	private:
		char* http_content;
		char* result_data;
	public: 
		Message();
		char* Http_get_content(char* content);
	  char* Http_get_Message(char *message,char * def,int i);

};
#endif