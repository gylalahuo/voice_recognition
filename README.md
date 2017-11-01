# voice_recognition
using LPC54114 collects pcm data，and stores in SPI FLASH. when detecting the voice collect finish,send the voice data the baidu voice api 
byWi-Fi module.

There are some folders in this project。
1、Dmic ：  implementating the  operations  about collecting PCM data
2、Flash ： implementating the  operations  about spi Flash
3、Message ： get corresponding data from http message which  send  baidu voice  api
4、MbedJSONValue ： convert string to json format or  json format to string
5、mxchip-wifi-at-driver： wifi dirver


