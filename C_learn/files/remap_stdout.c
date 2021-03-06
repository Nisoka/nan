#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <fcntl.h>

int save_fd;
int log_fd;

#ifndef USE_MY_PRINTF
#define printf app_printf
int app_printf(const char *, ...)
{
//    直接将printf 输出到任意文件.
    return 0;
}
#endif

void remap_stdout_log(){
    QDate cur_date = QDate::currentDate();
    QTime cur_time = QTime::currentTime();
    char  log_file[200] = "";
    sprintf(log_file, "/opt/user/app/QAM/log/%d-%d-%d_%d-%d.txt", cur_date.year(), cur_date.month(),cur_date.day(),\
                                cur_time.hour(), cur_time.minute());


    fflush(stdout);
    setvbuf(stdout,NULL,_IONBF,0);
    printf("remap_stdout_log >>> stdout\n");

    // 保存标准输出 文件描述符
    //注:这里一定要用 dup 复制一个文件描述符. 不要用 = 就像是Winodws下的句柄.
    save_fd = dup(STDOUT_FILENO);

    log_fd = open(log_file,(O_RDWR | O_CREAT), 0644);
    //dup2 替换文件句柄
    dup2(log_fd, STDOUT_FILENO);
    printf("remap_stdout_log >>> logfile\n");
}

int main(int argc, char* argv[]){
    //将printf 直接重定向到某个文件.
    remap_stdout_log();

    printf("");

}
