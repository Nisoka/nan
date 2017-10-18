#ifndef BF527_SERIAL
#define BF527_SERIAL

#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#include <string.h>


#define RECVFLAG_OVER 1
#define RECVFLAG_CONT 2
#define RECVFLAG_ERR  3
#define RECVFLAG_OTEHER_CLEAR 4

#define NCC	8

//struct termio
//{	unsigned short  c_iflag;	/* 输入模式标志 */	
//	unsigned short  c_oflag;		/* 输出模式标志 */	
//	unsigned short  c_cflag;		/* 控制模式标志*/	
//	unsigned short  c_lflag;		/* local mode flags */	
//	unsigned char  c_line;		    /* line discipline */	
//	unsigned char  c_cc[NCC];    /* control characters */
//};

class CCom
{
	public:
		CCom();
		~CCom();
		bool OpenDev(char *name);
		void CloseDev();
		void set_speed(int speed);
		bool set_Parity(int databits,int stopbits,int parity);
        int ClearInBuffer();
        int Read_old(char *buf,int nSize);
        int ReadBytesProcess(char *bufRecv, int &readCnt, char* buf, int nSize, int other = 0);
        int Read(char *buf,int nSize);
		int Write(char *buf,int nSize);
	private:
		int m_fd;
		termios m_tagOldOpt;
};



#endif
