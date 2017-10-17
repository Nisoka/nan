#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef linux
#include <unistd.h>
#include <dirent.h>
#endif
#ifdef WIN32
#include <direct.h>
#include <io.h>
#endif
#include <vector>
using namespace std;
/**
 * @function: 获取cate_dir目录下的所有文件名
 * @param: cate_dir - string类型
 * @result：vector<string>类型
*/
vector<string> getFiles(string cate_dir)
{
    vector<string> files;//存放文件名

#ifdef WIN32
    _finddata_t file;
    long lf;
    //输入文件夹路径
    if ((lf=_findfirst(cate_dir.c_str(), &file)) == -1) {
        cout<<cate_dir<<" not found!!!"<<endl;
    } else {
        while(_findnext(lf, &file) == 0) {
            //输出文件名
            //cout<<file.name<<endl;
            if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
                continue;
            files.push_back(file.name);
        }
    }
    _findclose(lf);
#endif

#ifdef linux
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(cate_dir.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        //strcmp 比较
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8)    ///file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            files.push_back(ptr->d_name);
        else if(ptr->d_type == 10)    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            continue;
        else if(ptr->d_type == 4)    ///dir
        {
            continue;
            files.push_back(ptr->d_name);
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
#endif

    //排序，按从小到大排序
//    sort(files.begin(), files.end());
    return files;
}


#ifdef WIN32
int main(void)
{
    char current_address[100];
    memset(current_address, 0, 100);
    getcwd(current_address, 100); //获取当前路径
    cout<<current_address<<endl;
    strcat(current_address, "\\*");

    vector<string> files=getFiles((string)current_address);
    for (int i=0; i<files.size(); i++)
    {
        cout<<files[i]<<endl;
    }

    //cout<<"Hello World"<<endl;

    cout<<"end..."<<endl;
    cin.get();
    return 0;
}
#endif

#ifdef linux
int main(void)
{
    DIR *dir;
    char basePath[100];

    ///get the current absoulte path
    memset(basePath, '\0', sizeof(basePath));
    getcwd(basePath, 999);
    printf("the current dir is : %s\n",basePath);


    cout<<endl<<endl;
    vector<string> files=getFiles(basePath);
    for (int i=0; i<files.size(); i++)
    {
        cout<<files[i]<<endl;
    }


    cout<<"end..."<<endl<<endl;
    return 0;
}
#endif
