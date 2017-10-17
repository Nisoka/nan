#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

time_t getFileProperty_ctime(char* filename){
    struct stat buf;
    int result;

    //获得文件状态信息
    result = stat(filename, &buf );

    if( result != 0 ){
        printf("stat file property failed !! \n");
    }else{
//        printf("文件大小: %d\n", buf.st_size);
//        printf("文件创建时间: %s", ctime(&buf.st_ctime));
//        printf("访问日期: %s", ctime(&buf.st_atime));
//        printf("最后修改日期: %s", ctime(&buf.st_mtime));
    }
    return buf.st_ctime;
}

bool getFiles_lastModified(char* cate_dir, char* lastTimeFilePath){
    DIR *dir;
    struct dirent *ptr;
    char fileFullPath[200];
    memset(fileFullPath, '\0', 200);
    time_t last_ctime;
    time_t tmp_ctime;
    bool hasFile = false;
    printf("default time is %s", ctime(&last_ctime));

    if ((dir=opendir(cate_dir)) == NULL)
    {
        return false;
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0){    ///current dir OR parrent dir
            continue;
        }else if(ptr->d_type == 8){                     ///file
            sprintf(fileFullPath, "%s%s", cate_dir, ptr->d_name);
            printf("%s\n", fileFullPath);
            tmp_ctime = getFileProperty_ctime(fileFullPath);

            hasFile = true;
            if(last_ctime < tmp_ctime){
                last_ctime = tmp_ctime;
                memcpy(lastTimeFilePath, fileFullPath, 200);
            }

        }else if(ptr->d_type == 10){                    ///link file
            continue;
        }else if(ptr->d_type == 4){                     ///dir
            continue;
        }
    }

    closedir(dir);
    printf("last ctime is %s, file is %s\n", ctime(&last_ctime), lastTimeFilePath);
    if(hasFile)
        return true;
    else
        return false;
}



int main(int argc, char* argv[]){
    char targetFile[200];
    memset(targetFile,'\0', 200);
    getFiles_lastModified("/mnt/hgfs/VWork/gith/nan/C_learn/files/", targetFile);
}
