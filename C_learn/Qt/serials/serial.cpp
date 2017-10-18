#include "serial.h"

bool need_process;

CCom::CCom()
{
	
}

CCom::~CCom()
{
	
}

bool CCom::OpenDev(char *name)
{
    need_process = false;

    m_fd=open(name,O_RDWR);
	if(-1==m_fd)
	{
		perror("不能打开串口设备");
		return false;
	}
	tcgetattr(m_fd,&m_tagOldOpt);
    //fcntl(m_fd,F_SETFL,FNDELAY);  //no block
    //fcntl(m_fd,F_SETFL,0);          //block
    printf("open com ok !\n");
	return true;
}

void CCom::CloseDev()
{
	tcsetattr(m_fd,TCSANOW,&m_tagOldOpt);
	close(m_fd);
}
void CCom::set_speed(int speed)
{
    unsigned int i;
    int status;
    struct termios Opt;
    int speed_arr[] = { B115200,B38400, B19200, B9600, B4800, B2400, B1200, B300,
            B38400, B19200, B9600, B4800, B2400, B1200, B300, };
    int name_arr[] = { 115200,38400, 19200, 9600, 4800, 2400, 1200, 300, 38400, 19200,
            9600, 4800, 2400, 1200, 300, };
    tcgetattr(m_fd, &Opt);
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if (speed == name_arr[i])
        {
            tcflush(m_fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);

            status = tcsetattr(m_fd, TCSANOW, &Opt);
            if (status != 0)
            {
                perror("tcsetattr fd1");
                return;
            }
            tcflush(m_fd, TCIOFLUSH);
            break;
        }
    }
}

bool CCom::set_Parity(int databits,int stopbits,int parity)
{
    struct termios options;
    if	( tcgetattr( m_fd,&options)  !=  0)
    {
        perror("SetupSerial 1");
        return false;
    }
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;   /*Output*/
    options.c_iflag &= ~(IXON | IXOFF | IXANY | IGNCR | INLCR | ICRNL);

    switch (databits) /*设置数据位数*/
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unsupported data size\n");
            return (false);
    }
    switch (parity)
    {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;   /* Clear parity enable */
            options.c_iflag &= ~INPCK;	   /* Enable parity checking */
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
            options.c_iflag |= INPCK;			  /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
            options.c_cflag |= PARENB;	   /* Enable parity */
            options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
            options.c_iflag |= INPCK;		/* Disnable parity checking */
            break;
        case 'S':
        case 's':  /*as no parity*/
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported parity\n");
            return false;
        }
    /* 设置停止位*/
    switch (stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
           break;
        default:
             fprintf(stderr,"Unsupported stop bits\n");
             return false;
    }

    tcflush(m_fd,TCIFLUSH);
    options.c_cc[VTIME] = 150;
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
    if (tcsetattr(m_fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return false;
    }
    return true;
}


int CCom::ClearInBuffer()
{
    char buf[1024] = {0};

    int len,fs_sel;
    fd_set fs_read;

    struct timeval time;

    FD_ZERO(&fs_read);
    FD_SET(m_fd, &fs_read);

    time.tv_sec = 0;
    time.tv_usec = 50000;

    //使用select实现串口的多路通信
    fs_sel = select(m_fd+1, &fs_read, NULL, NULL, &time);

    if(fs_sel > 0)
    {
        len = read(m_fd, buf, 1024);
        return len;
    }

    return 0;
}


int CCom::Read_old(char *buf,int nSize)
{
#if 0
    return read(m_fd,buf,nSize);
#else
    int len,fs_sel;
    fd_set fs_read;

    struct timeval time;

    FD_ZERO(&fs_read);
    FD_SET(m_fd, &fs_read);

    time.tv_sec = 1;
    time.tv_usec = 50000;

    //使用select实现串口的多路通信
    fs_sel = select(m_fd+1, &fs_read, NULL, NULL, &time);
    if(fs_sel > 0)
    {
        len = read(m_fd, buf, nSize);
//        printf("read %d bytes :\n", len);
//        for(int i=0; i<len; i++)
//        {
//            printf(" 0x%x ", buf[i]);
//        }
//        printf("\n");
        return len;
    }
    else if(fs_sel == 0)
    {
//        printf("timeout !\n");
        return -1;
    }
    else
    {
//        printf("select error !\n");
        return -1;
    }
#endif
}


int CCom::ReadBytesProcess(char *bufRecv, int &readCnt, char *buf, int nSize, int other)
{
    static int curState = 0;

    static bool recvOverFlag = false;
    static char len1 = 0;
    static char len2 = 0;
    static int len = 0;

    static char cmd = 0xFF;

    static int recvCnt = 0;
    static char bufRead[200] = {0x00};

    if(other == RECVFLAG_OTEHER_CLEAR){
        curState = 0;
        recvOverFlag = false;
        len = 0;
        cmd = 0xFF;
        recvCnt = 0;
        return RECVFLAG_OTEHER_CLEAR;
    }

    recvOverFlag = false;

    for(int i = 0; i < nSize; i++){

        bufRead[recvCnt++] = buf[i];

        switch(curState){
        case 0:
            if(buf[i] == 0x55){
                curState = 1;
            }else{
                recvCnt = 0;
                curState = 0;
                printf(" >>>>>>>>>>> recv is not standard! 0x%x\n", buf[i]);
            }
            break;

        case 1:
            if(buf[i] == 0xAA){
                curState = 2;
            }else{
                recvCnt = 0;
                curState = 0;
            }
            break;
        case 2:
            if(buf[i] == 0x01){
                curState = 3;
            }else{
                recvCnt = 0;
                curState = 0;
            }
            break;
        case 3:
            len1 = buf[i];
            curState = 4;
            break;
        case 4:
            len2 = buf[i];
            curState = 5;
            len = 0;
            len = len1 << 8 | len2;
            break;
        case 5:     //data
            if(recvCnt == len-1){
                curState = 6;
            }
            break;
        case 6:
            curState = 0;

            char verify = 0x00;
            for(int i = 0; i < len; i++){
                verify += bufRead[i];
            }
            if(verify == 0x00){
                recvOverFlag =  true;
            }
            goto RECVOVER;
            break;
        }
    }

RECVOVER:
    readCnt = recvCnt;


    if(len != 0 && recvCnt == len){
        recvCnt = 0;
        if(recvOverFlag){
            memcpy(bufRecv, bufRead, len);
            return RECVFLAG_OVER;       // Recv Over
        }else{
//            printf("RECVFLAG ERR ====== len is %d \n", len);
//            for(int i=0; i<len; i++)
//                printf("0x%x ", bufRead[i]);
//            printf("\nRECVFLAG ERR\n");
            return RECVFLAG_ERR;  //CEHCK FAIL
        }
    }else{
        return RECVFLAG_CONT;
    }
}

int CCom::Read(char *buf,int nSize)
{
    int len,fs_sel;
    fd_set fs_read;
    struct timeval time;

    FD_ZERO(&fs_read);
    FD_SET(m_fd, &fs_read);

    time.tv_sec = 1;
    time.tv_usec = 50000;

    int recvFlag = RECVFLAG_CONT;
    char tempBuf[200] = {0};
    int recvLen = 0;
    int maxIter = 0;

    while(recvFlag == RECVFLAG_CONT && maxIter++ < 5){
        fs_sel = select(m_fd+1, &fs_read, NULL, NULL, &time);
        if(fs_sel > 0){
            len = read(m_fd, tempBuf, nSize);
            recvFlag = ReadBytesProcess(buf, recvLen, tempBuf, len);
        }

        if(recvFlag == RECVFLAG_CONT){
            printf("RECV CONTINUING..................\n");
        }
        if(recvFlag == RECVFLAG_ERR){
            printf("\n\n XXXXXXXXXXXXXX RECV ERROR XXXXXXXXXXX %d \n\n", len);
        }
    }

    if(maxIter >= 4){
        printf("\n\n XXXXXXXXXXXXXX RECV ERROR MAXITER XXXXXXXXXXX \n\n");
        //need process
        need_process = true;
    }

    ReadBytesProcess(buf, len, tempBuf, 0, RECVFLAG_OTEHER_CLEAR);
    if(recvFlag == RECVFLAG_OVER){
            len = recvLen;
    }

    if(recvFlag == RECVFLAG_CONT){
        len = -1;
    }

    if(recvFlag == RECVFLAG_ERR){
        len = -1;
    }


    return len;
}

int CCom::Write(char *buf,int nSize)
{
#if 1
//    int cnt = 0;
//    while(cnt++ < nSize)
//        printf("0x%x ", buf[cnt]);
//    printf(" =========== \n");
    return write(m_fd,buf,nSize);
#else
    int len = 0;
    len = write(m_fd, buf, nSize);
    if (len == nSize )
    {
        //printf("write %d bytes !\n", len);
        return len;
    }
    else
    {
        printf("write com failed !\n");
        tcflush(m_fd, TCOFLUSH);
        return -1;
    }
#endif
}

