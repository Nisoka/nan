#ifndef BF527_SERIAL
#define BF527_SERIAL

#include     <stdio.h>      /*��׼�����������*/
#include     <stdlib.h>     /*��׼�����ⶨ��*/
#include     <unistd.h>     /*Unix ��׼��������*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      /*�ļ����ƶ���*/
#include     <termios.h>    /*PPSIX �ն˿��ƶ���*/
#include     <errno.h>      /*����Ŷ���*/
#include <string.h>


#define RECVFLAG_OVER 1
#define RECVFLAG_CONT 2
#define RECVFLAG_ERR  3
#define RECVFLAG_OTEHER_CLEAR 4

#define NCC	8

//struct termio
//{	unsigned short  c_iflag;	/* ����ģʽ��־ */	
//	unsigned short  c_oflag;		/* ���ģʽ��־ */	
//	unsigned short  c_cflag;		/* ����ģʽ��־*/	
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
