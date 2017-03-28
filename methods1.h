/**
PROJECT 2
**************

File: methods1.h

Date: 3/27/2017

Authors: Ryan Joiner (810638856)
         Emma Fick   (810609119)

Prof. Michael Cotterell
**************

<NEED DESCRIPTION>

This is the supporting method file for Project2.

<NEED DESCRIPTION>
**/

#ifndef METHODS1_H
#define METHODS1_H

//########
//INCLUDES
//########
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <string>
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

#include "methods_main.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//##########
//PROTOTYPES
//##########
using namespace std;

bool file_exists(string filename);
bool save_file(string filename, string data);
string load_file(const char * fileptr);
bool close_file(int &fd);
string get_filename(const char * fileptr);

//###################
//METHOD DESCRIPTIONS
//###################

//MIGHT NOT BE NEEDED
//NEEDS JAVADOC
//[ENOENT] O_CREAT is not set and the named file does not exist.

bool file_exists(string filename){
  struct stat buf;
  return((stat(filename.c_str(), &buf)) == 0);
}


//NEEDS JAVADOC
bool save_file(string filename, string s){
  bool success = true;
  int fd = open(filename.c_str(), O_RDWR| O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  if(fd < 0){
    success = false;
  } else {
    string data = s;
    int n = s.size();
    char * buffer = new char[n];
    buffer = (char*)data.c_str();
    if(write(fd, buffer, n)< 0){
      success = false;
    }
  }
  return success;
}


//MORE FUNCTIONALITY WITH ERRORS
//NEEDS JAVADOC
string load_file(string filename){
  int fd = open(filename.c_str(), O_RDWR | O_APPEND);
  if(fd < 0){
    int open_err_num = errno;
    if(filename == ""){
      return "";
    } else {
    }
  }
  else{
    const int BUFF_SIZE = 1024;
    char buffer[BUFF_SIZE];
    int n = 0;
    string data = "";
    while((n = read(fd, buffer, BUFF_SIZE)) > 0){
      for(int i = 0; i < n; i++){
        data += buffer[i];
      }
    }
    if(n < 0){
      int read_err_num = errno;
      //err window
      return "";
    }else {
    return data;
    }
  }
  return "";
}

//ERROR CHECK FUNCTIONALITY
//NEEDS JAVADOC
bool close_file(string filename){
  return((close(*filename.c_str())) != -1);
}


//MIGHT NOT BE NEEDED
//NEEDS JAVADOC
string get_filename(const char * fileptr){
  if (fileptr == nullptr){
    return "Untitled";
  }
  else {
    string fn = fileptr;
    return fn;
  }
}


#endif
