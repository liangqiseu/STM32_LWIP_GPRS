#include "lwip/sio.h"
#include "usart1.h"	
#include "usart2.h"
#include "delay.h"


/* The following functions can be defined to something else in your cc.h file
   or be implemented in your custom sio.c file. */


/**
 * Opens a serial device for communication.
 * 
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
	return 0;
}



/**
 * Sends a single character to the serial device.
 * 
 * @param c character to send
 * @param fd serial device handle
 * 
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
	//return 0;
}



/**
 * Receives a single character from the serial device.
 * 
 * @param fd serial device handle
 * 
 * @note This function will block until a character is received.
 */
u8_t sio_recv(sio_fd_t fd)
{
	return 0;
}



/**
 * Reads from the serial device.
 * 
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 * 
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
	return 0;
}



/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 * 
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
	return 0;
}



/**
 * Writes to the serial device.
 * 
 * @param fd serial device handle
 * @param data pointer to data to send
 * @param len length (in bytes) of data to send
 * @return number of bytes actually sent
 * 
 * @note This function will block until all data can be sent.
 */
u32_t sio_write(sio_fd_t fd, u8_t *data, u32_t len)
{
	u8 t;
	u8_t *data1;
	fd=fd;  
	data1=data;
	
	///////////////////////////////////////////
	//先发送给PC
	//////////////////////////
	for(t=0;t<len;t++)
	{
			USART1->DR=*data1;
			data1++;
			while((USART1->SR&0X40)==0);//等待发送结束
	}	
	for(t=0;t<3;t++)
	{
			USART1->DR=0XAA;
			while((USART1->SR&0X40)==0);//等待发送结束
	}	
	///////////////////////////////////
	//再发送给SIM900
	///////////////////////////
	for(t=0;t<len;t++)
	{
			USART2->DR=*data;
			data++;
			while((USART2->SR&0X40)==0);//等待发送结束
	}	
	return t;
}


/**
 * Aborts a blocking sio_read() call.
 * 
 * @param fd serial device handle
 */
void sio_read_abort(sio_fd_t fd)
{
//	return 0;
}


