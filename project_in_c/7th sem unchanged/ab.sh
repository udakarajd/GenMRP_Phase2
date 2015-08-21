#!/bin/sh
gcc -o /home/udakarajd/Desktop/project_in_c/ga  /home/udakarajd/Desktop/project_in_c/ga.c `mysql_config --cflags --libs` -std=c99 
chmod 777 /home/udakarajd/Desktop/project_in_c/ga
/home/udakarajd/Desktop/project_in_c/ga
