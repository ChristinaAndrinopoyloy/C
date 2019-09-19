#include "error_handling.h"

void error_handling(int error_no)
{
    switch (error_no)
    {
    case 1:
        printf("Error: app command: argument without value\n"); // getopt from main
        exit(1);
        break;
    case 2:
        printf("Error: app command: unknown argument\n"); // getopt from main
        exit(1);
        break;
    case 3:
        printf("Error: app command: input directory does not exist\n"); // from checking_arguments
        exit(1);
        break;
    case 4:
        printf("Error: app command: mirror directory exists\n"); // from checking_arguments
        exit(1);
        break;
    case 5:
        printf("Error: app command: buffer size must be > 0\n"); // from checking_arguments
        exit(1);
        break;
    case 6:
        printf("Error: app: create file failed at updating common directory\n"); // from update_common_directory
        break;
    case 7:
        printf("Error: app: update common directory failed\n"); // from main
        break;
    case 8:
        printf("Error: app: write file failed at updating common directory\n"); // from update_common_directory
        break;
    case 9:
        printf("Error: app: client file at common already exists (a client runs with the same ID)\n"); // from update_common_directory
        exit(1);
        break;
    case 10:
        printf("Error: app: remove directory failed\n"); // from main
        break;
    case 11:
        printf("Error: app: remove file failed\n"); // from main
        break;
    case 12:
        printf("Error: app: checking common directory failed\n"); // from communication
        break;
    case 13:
        printf("Error: app: communication failed\n"); // from main
        break;
    case 14:
        printf("Error: app: cannot get subdirs and files\n"); // from communication preprocess
        break;
    case 15:
        printf("Error: app: communication preprocess failed\n"); // from communication
        break;
    case 16:
        printf("Error: app: get subdirectories and files failed\n"); // from files
        break;
    case 17:
        printf("Error: app: initialize buffer from pipe failed\n"); // from communication
        break;
    case 18:
        printf("Error: app: set mirror failed\n"); // from communication
        break;
    default:
        break;
    }
}