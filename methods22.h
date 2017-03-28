/**
PROJECT 2
**************

File: methods2.h

Date: 3/27/2017

Authors: Ryan Joiner (810638856)
         Emma Fick   (810609119)

Prof. Michael Cotterell
**************

<NEED DESCRIPTION>

This is the supporting method file for Project2.

<NEED DESCRIPTION>
**/

#ifndef METHODS22_H
#define METHODS22_H

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

#include "methods_main.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
typedef unsigned int uint;

//##########
//PROTOTYPES
//##########
using namespace std;

void run_editor(const char * fileptr);
void data_to_screen(string data, WINDOW * win, uint max_y, uint max_x);
string add_char(WINDOW *win, vector<string> vec, uint &x , uint &y , int key , bool &saved);
//string remove_char(string s);
vector<string> str_to_vector(string data, uint max_x);
//NEEDS JAVADOC
void run_editor(const char *fileptr){
  WINDOW *term_win;
  WINDOW *edit_win;

  vector<string> globalVec;

  string filename = get_filename(fileptr);
  //bool exists = file_exists(fileptr);
  bool saved = false;
  bool adding = true;
  initscr();
  cbreak();
  noecho();
  int term_h, term_w;
  getmaxyx(stdscr, term_h, term_w);
  int menu_h = term_h/2;
  int menu_w = term_w/2;
  int menu_x = (term_w / 4), menu_y = (term_h / 4);
  int edit_x = 1, edit_y = 1;
  int edit_h = (term_h * 0.90);
  int edit_w = (term_w * 0.90);
  term_win = newwin(term_h, term_w, 0, 0);
  edit_win = derwin(term_win, edit_h, edit_w, edit_x, edit_y);

  //int ex = 2;
  //int ey = 2;
  //FIGURE OUT PADS?
  //edit_win = newpad(edit_h,edit_w);

  touchwin(edit_win);
  keypad(term_win, true);
  keypad(edit_win, true);
  refresh();
  mvwprintw(term_win, 0, 0, "F1: Menu/Cancel");
  mvwprintw(term_win, 0, (term_w/2)-8, "CSCI 1730 Editor");
  mvwprintw(term_win, (term_h-1), 0, filename.c_str());

  uint max_x, max_y;
  getmaxyx(edit_win, max_y, max_x);
  box(edit_win, 0, 0);
  string text = load_file(filename);
  globalVec = str_to_vector(text, max_x);
  data_to_screen(text, edit_win, max_y, max_x);
  wrefresh(term_win);
  wrefresh(edit_win);
  int key = 1;
  bool running = true;
  string appended = "";
  //Maybe create another set of x,y positions to keep track of
  //where the user has moved the cursor to... for the else if where
  //the user enters a valid key to add to the string we check if the
  //xy pair are the same and if not edit at the new location and set ex/ey
  //to new location
  uint cur_x, cur_y;
  
  while(running){
    cur_x = 2;
    cur_y = 2;
    globalVec = str_to_vector("TESTING\nHERE\nNOW", max_x);
    text = add_char(edit_win, globalVec, cur_x, cur_y, 63, saved);
    move(cur_y, cur_x);
    //getyx(edit_win, ey, ex);
    //wmove(edit_win,ey, ex);
    //file_to_screen("", edit_win, edit_h, edit_w); //maybe
    wclear(edit_win);
    box(edit_win, 0, 0);
    refresh();
    wrefresh(term_win);
    touchwin(term_win);
    data_to_screen(text, edit_win, max_y, max_x);
    refresh();
    wrefresh(edit_win);
    touchwin(term_win);
    wrefresh(term_win);
    wrefresh(edit_win);

    getch();
    getch();
    if(saved){
      wclear(term_win);
      box(edit_win, 0, 0);
      mvwprintw(term_win, 0, 0, "F1: Menu/Cancel");
      mvwprintw(term_win, 0, (term_w/2)-8, "CSCI 1730 Editor");
      mvwprintw(term_win, (term_h-1), 0, (filename + " (saved)").c_str());
      refresh();
      data_to_screen(text, edit_win, max_y, max_x);
      wrefresh(edit_win);
    }
    else{
      wclear(term_win);
      box(edit_win, 0, 0);
      refresh();
      mvwprintw(term_win, 0, 0, "F1: Menu/Cancel");
      mvwprintw(term_win, 0, (term_w/2)-8, "CSCI 1730 Editor");
      mvwprintw(term_win, (term_h-1), 0, filename.c_str());
      refresh();
      data_to_screen(text, edit_win, max_y, max_x);
      wrefresh(edit_win);
    }
    refresh();
    //bool saved = false;
    key = wgetch(edit_win);
    getmaxyx(edit_win, max_y, max_x);
    switch(key){
    case KEY_F(1):
      {
      int selection = show_menu_window(menu_h, menu_w, menu_y, menu_x);
      wrefresh(term_win);
      if(selection == 0){
        wrefresh(term_win);
        wrefresh(edit_win);
      }
      else if(selection == 1){
        int savePrompt = 0;
        if(!saved){
          savePrompt = show_save_window(menu_h, menu_w, menu_y, menu_x);
          if(savePrompt == 1){
            save_file(filename, text);
            saved = true;
            savePrompt = 0;
          }
        }
        if(savePrompt == 0){
          string new_filename = "";
          new_filename = show_open_window(menu_h, menu_w, menu_y, menu_x);
          if(new_filename == ""){
            saved = false;
            break;
          }
          saved = true;
          wclear(edit_win);
          box(edit_win, 0, 0);
          refresh();
          wrefresh(term_win);
          text = load_file(new_filename);
          data_to_screen(text, edit_win, max_y, max_x);
          mvwprintw(term_win, (term_h-1), 0, new_filename.c_str());
          filename = new_filename;
          //wmove(edit_win, ey, ex);
          refresh();
          wrefresh(edit_win);
          touchwin(term_win);
        }

        else{  //Cancel, return to current edit
          wclear(edit_win);
          box(edit_win, 0, 0);
	  refresh();
          wrefresh(term_win);
          touchwin(term_win);
          data_to_screen(text, edit_win, max_y, max_x);

        }
      }
      else if(selection == 2){
        if(strcmp(filename.c_str(), "<N/A>") == 0){
          filename = "";
          filename += show_saveas_window(menu_h, menu_w, menu_y, menu_x);
        }
        save_file(filename, text);
        saved = true;
        data_to_screen(text, edit_win, max_y, max_x);

        wclear(edit_win);
        box(edit_win, 0, 0);

        refresh();
        wrefresh(term_win);
        touchwin(term_win);
        mvwprintw(term_win, (term_h-1), 0, filename.c_str());
        data_to_screen(text, edit_win, max_y, max_x);
        refresh();
        wrefresh(edit_win);
        touchwin(term_win);
      }
      else if(selection == 3){
        filename = "";
        filename += show_saveas_window(menu_h, menu_w, menu_y, menu_x);
        save_file(filename, text);
        saved = true;
        data_to_screen(text, edit_win, max_y, max_x);
        wclear(edit_win);
        box(edit_win, 0, 0);

        refresh();
        wrefresh(term_win);
        touchwin(term_win);
        data_to_screen(text, edit_win,max_y, max_x);
        mvwprintw(term_win, (term_h-1), 0, filename.c_str());
        refresh();
        wrefresh(edit_win);
        touchwin(term_win);
      }
      else if(selection == 4){
        if(saved){
          running = false;
        } else {
          int savePrompt = show_save_window(menu_h, menu_w, menu_y, menu_x);
          if(savePrompt == 0){
            running = false;
          }
          else if(savePrompt == 1){
            if(strcmp(filename.c_str(), "<N/A>") == 0){
              filename = "";
              filename += show_saveas_window(menu_h, menu_w, menu_y, menu_x);
            }
            save_file(filename, text);
            saved = true;
            running = false;
          }
          else {
            wclear(edit_win);
            box(edit_win, 0, 0);
            refresh();
            wrefresh(term_win);
            touchwin(term_win);
            data_to_screen(text, edit_win, max_y, max_x);
          }
        }
      }
      break;
      }
      case KEY_DOWN:
	{
	if(cur_y < max_y){
	  cur_y++; //move cursor down the screen
	  //checl for vector size and move cur_x to right spot
	}
	else if(cur_y == max_y){
	  //move scroll_offset + 1 
	}
	adding = false;
        break;
	}
      case KEY_UP:
	{
	if(cur_y > 2){
	  cur_y--;
	  //check for vector size and move cur_x to right spot
	}
	else if(cur_y == 2){
	  //move scroll_offset -1
	}
	adding = false;
	break;
	}
      case KEY_LEFT:
	{
	if(cur_x > 2){
	  cur_x--;
	}
	else if(cur_x == 2){
	  //cur_y -1
	  //move cur_x to end of the previous line
	  //if cur_y == 2 then check if there is a scroll_offset
	}
	adding = false;
	break;
	}
      case KEY_RIGHT:
	{
	if(cur_x < max_x){
	  cur_x++;
	}
	else if(cur_x == max_x){
	  //cur_y +1
	  //move cur_x to beginning of next line
	  //if cur_y == max_y then check if there is a scroll_offset

	}
	adding = false;
	break;
	}
    }

    if(adding){
      text = add_char(edit_win, globalVec, cur_x, cur_y, key, saved);
    }
    else
      adding = true;

    move(cur_y, cur_x);
    //getyx(edit_win, ey, ex);
    //wmove(edit_win,ey, ex);
    //file_to_screen("", edit_win, edit_h, edit_w); //maybe
    wclear(edit_win);
    box(edit_win, 0, 0);
    refresh();
    wrefresh(term_win);
    touchwin(term_win);
    data_to_screen(text, edit_win,max_y, max_x);
    refresh();
    wrefresh(edit_win);
    touchwin(term_win);
    wrefresh(term_win);
    wrefresh(edit_win);
  }
  delwin(edit_win);
  delwin(term_win);;
  endwin();
}

string add_char(WINDOW *win, vector<string> vec, uint &x, uint &y, int key, bool &saved){
  uint max_x, max_y;
  getmaxyx(win, max_y, max_x);
  saved = false;
  string retStr = "";
  for(uint i = 0; i < vec.size(); i++){
    for(uint j = 0; j < vec.at(i).size(); j++){
      retStr += vec.at(i).at(j);
      //wmvprintw(win, i+1, j+1, vec.at(i).at(j).c_str();)
    }
  }
  //REMOVE THIS AFTER TESTING
  return retStr;
  if(vec.size() > y){
    uint lines_to_add = vec.size() - y;
    if(key == 10){
      y++;
      if(vec.at(y-1).size() == x){
	retStr += "\n";
      }
      else {
	retStr += "\n";
	retStr += vec.at(y-1).substr(x);
      }
      x = 2;
      for(uint i = y+1; i < vec.size(); i++){
	for(uint j = 0; j < vec.at(i).size(); j++){
	  retStr += vec.at(i).at(j);
	}
      }
    }
    else if(key == KEY_BACKSPACE || key == 127){
      if(x == 2 && y > 2){
	retStr += vec.at(y);
	retStr += "\n";
	for(uint i = y+1; i < vec.size(); i++){
	  for(uint j = 0; j < vec.at(i).size(); j++){
	    retStr += vec.at(i).at(j);
	  }
	}
      }
      else {
	retStr += vec.at(y).substr(0, x-1);
	if(vec.at(y).size() != x-1){
	  retStr += vec.at(y).substr(x+1);
	}
	for(uint i = y+1; i < vec.size(); i++){
	  for(uint j = 0; j < vec.at(i).size(); j++){
	    retStr += vec.at(i).at(j);
	  }
	}
      }
    }
    else {
      if(vec.at(y).size() < max_x){
	retStr += vec.at(y).substr(0,x);
	retStr += (char) key;
	retStr += vec.at(y).substr(x+1);
	for(uint i = y+1; i < vec.size(); i++){
	  for(uint j = 0; j < vec.at(i).size(); j++){
	    retStr += vec.at(i).at(j);
	  }
	}
	x++;
      }
      else{
	y++;
	x=2;
	retStr += char (key);
	for(uint i = y; i < vec.size(); i++){
	  for(uint j = 0; j < vec.at(i).size(); j++){
	    retStr += vec.at(i).at(j);
	  }
	}
      }
    }
    if(vec.size()-1 == y){
      if(key == 10){
	y++;
	if(vec.at(y-1).size() == x){
	  retStr += "\n";
	}
	else {
	  retStr += "\n";
	  retStr += vec.at(y-1).substr(x);
	}
	x = 2;
      }
	//for(int i = y+1; i < vec.size(); i++){
	//for(int j = 0; j < vec.at(i).size(); j++){
	//retStr += vec.at(i).at(j);
	//}
	//}
       else if(key == KEY_BACKSPACE || key == 127){
	  if(x == 2 && y > 2){
	    retStr += vec.at(y);
	    retStr += "\n";
	    //for(int i = y+1; i < vec.size(); i++){
	    //for(int j = 0; j < vec.at(i).size(); j++){
	    //retStr += vec.at(i).at(j);
	    //}
	    //}
	  }
	  else {
	    retStr += vec.at(y).substr(0, x-1);
	    if(vec.at(y).size() != x-1){
	      retStr += vec.at(y).substr(x+1);
	    }
	    //for(int i = y+1; i < vec.size(); i++){
	    //for(int j = 0; j < vec.at(i).size(); j++){
	    //retStr += vec.at(i).at(j);
	    //}
	    //}
	  }
       }
	else {
	  if(vec.at(y).size() < max_x){
	    retStr += vec.at(y).substr(0,x);
	    retStr += (char) key;
	    retStr += vec.at(y).substr(x+1);
	    //for(int i = y+1; i < vec.size(); i++){
	    //for(int j = 0; j < vec.at(i).size(); j++){
	    //retStr += vec.at(i).at(j);
	      }
	    }
	    x++;
	  }
	  else{
	    y++;
	    x=2;
	    retStr += char (key);
	    //for(int i = y; i < vec.size(); i++){
	    //for(int j = 0; j < vec.at(i).size(); j++){
	    //retStr += vec.at(i).at(j);
	    //}
	    //}
	  }
  }
  move(y, x);
  refresh();
  return retStr;
}

vector<string> str_to_vector(string data, uint max_x){
  vector<string> vec;
  //int y = 0;
  uint x = 0;
  string textLine = "";
  char ch;
  for(uint i = 0; i<data.size(); i++){
    ch = data[i];
    if(ch == '\n' || x == max_x-1){
      textLine += "\n";
      vec.push_back(textLine);
      textLine = "";
      x = 0;
    }
    else{
      x++;
      textLine += data[i];
    }
  }
  return vec;
}

void data_to_screen(string data, WINDOW * win, uint max_x, uint max_y){
  int x = 1;
  int y = 1;
  for(unsigned int i = 0; i < data.size(); i++){
    char ch = data[i];
    mvwprintw(win, y, x, "%c", ch);
    if(x == (max_x - 1) || ch == '\n'){
      y++;
      x = 0;
      //mvwprintw(win, x, y,"%c", '\n');
    }
    x++;
  }
  //wmove(win, y, x);
  refresh();
  wrefresh(win);
  touchwin(win);

  //mvwprintw(win, x, y, "%c", '\0');
}

#endif
