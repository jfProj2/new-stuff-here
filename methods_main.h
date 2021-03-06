/**
PROJECT 2
**************

File: methods_main.h

Date: 3/27/2017

Authors: Ryan Joiner (810638856)
         Emma Fick   (810609119)

Prof. Michael Cotterell
**************

<NEED DESCRIPTION>

This is the methods main hub for Project2.

<NEED DESCRIPTION>
**/

#ifndef METHODS_MAIN_H
#define METHODS_MAIN_H

//########
//INCLUDES
//########
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <curses.h>
#include <ncurses.h>
#include <menu.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "methods1.h"
#include "methods3.h"
#include "methods22.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
typedef unsigned int uint;
//##########
//PROTOTYPES
//##########
using namespace std;

bool file_exists(string filename);
bool save_file(string filename, string data);
string load_file(const char * fileptr);
bool close_file(string filename);
string get_filename(const char * fileptr);

void run_editor(const char * fileptr);
void data_to_screen(string data, WINDOW * win, uint max_y, uint max_x);
string add_char(WINDOW *win, vector<string> vec,int &x , int &y , int key, bool &saved);
//string remove_char(string s);
vector<string> str_to_vector(string data, uint max_x);

int show_menu_window(int menu_h, int menu_w, int menu_y, int menu_x);
bool show_error_window(int code, int err_h, int err_w, int err_y, int err_x);
string show_open_window(int open_h, int open_w, int open_y, int open_x);
int show_save_window(int save_h, int save_w, int save_y, int save_x);
string show_saveas_window(int sa_h, int sa_w, int sa_y, int sa_x);

#endif
