#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/fcntl.h>


void parse_argv(int argc, char* argv[])
{
    int c;
    for (;;) {
        c = getopt(argc, argv, "i:p:t");
        if (c < 0)
            break;
        switch (c) {
        case 'i':
            free(ctrl_iface);
            ctrl_iface = strdup(optarg);
            break;
        case 'p':
            free(ctrl_iface_dir);
            ctrl_iface_dir = strdup(optarg);
            break;
        case 't':
            startInTray = true;
            break;
        }
    }
}


int main(int argc, char* argv[]){
    parse_argv(argc, argv);
}
