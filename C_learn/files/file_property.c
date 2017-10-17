#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

void getFiles(char *cate_dir);

void main( void )
{
    struct stat buf;
    int result;

    //获得文件状态信息

    getFiles("/mnt/hgfs/VWork/gith/nan/C_learn/files/");
    result = stat( "file_property.c", &buf );

    //显示文件状态信息

   if( result != 0 )
       perror( "显示文件状态信息出错" );//并提示出错的原因，如No such file or directory（无此文件或索引）

    else
    {

        printf("文件大小: %d\n", buf.st_size);
        printf("文件创建时间: %s", ctime(&buf.st_ctime));
        printf("访问日期: %s", ctime(&buf.st_atime));
        printf("最后修改日期: %s", ctime(&buf.st_mtime));

    }
}


void getFiles(char *cate_dir){
    DIR *dir;
    struct dirent *ptr;
    char fileFullPath[200];
    memset(fileFullPath, '\0', 200);

    if ((dir=opendir(cate_dir)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        //strcmp 比较
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8){    ///file
//            printf("d_name :%s \n",ptr->d_name);
            sprintf(fileFullPath, "%s%s", cate_dir, ptr->d_name);
            printf("%s\n",fileFullPath);
//            files.push_back(ptr->d_name);
        }else if(ptr->d_type == 10){    ///link file
            printf("d_name: %s\n",ptr->d_name);
            continue;
        }else if(ptr->d_type == 4)    ///dir
        {
            continue;
//            files.push_back(ptr->d_name);
            /*
                memset(base,'\0',sizeof(base));
                strcpy(base,basePath);
                strcat(base,"/");
                strcat(base,ptr->d_nSame);
                readFileList(base);
            */
        }
    }
    closedir(dir);
}
