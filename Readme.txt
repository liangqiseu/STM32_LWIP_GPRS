version 1.0
author£ºliangqiseu
2016-5-10

This project is using LWIP-1.4.1 to connect GPRS modem ,and it has done well on SIM808. The MCU is STM32F103. 
It is important to note that the project has no RTOS now, so it must using RAW-API. If you have a TCP server, you can use 
this project to connect it easily. Note that the server must have a public IP address. 

Something in the project is debug items, such as Jlink settings ,Jlink logs and so on , you can ignore them.