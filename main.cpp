//Coded by Monsterduty

#include <X11/Xlib.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#define I3IPC_IMPLEMENTATION
#include "i3ipc.h"

using std::system;
using std::cout;
using std::endl;
using std::string;

// this function return a string with the id of all windows in all desktops
string checkWindows(){

  FILE *activeWindows = popen("xprop -root _NET_CLIENT_LIST | sed 's/.*#//'", "r"); // get all windows id provided by xprop

  char buffer[50];
  string windowsList = "";

  memset(buffer, 50, '\0');

  while ( fgets(buffer, sizeof buffer, activeWindows) != NULL ) // copying the FILE data type to a string for return
  {

      windowsList += buffer;

  };

  pclose(activeWindows);

  for ( int i = 0; i < size(windowsList); i++ ) // filter the result and adding endlines
  {

      if( windowsList[i] == ',' ){ windowsList[i] = '\n'; };

  };

  return windowsList;

};

// this function recive a string with all windows IDs and return the current focused window id
string checkFocused(string list){
  
  int i = 0; // this variable is to iterate through the list of IDs until it reaches \0

  while ( true )
  {

    string buffer = "xprop -id ", target = "";
    char command[70];
    int f = 10;
    int g = 0;

      while ( true )
      {
        
        if ( list[i] == '\n' ){ i++; break; }; // separating IDs

        buffer += list[i];
        if ( list[i] != ' ' ){ target += list[i]; };
      
        i++;

      };

      buffer += " _NET_WM_STATE";

      FILE *checking = popen(buffer.c_str(), "r");

      memset(command, std::size(command), '\0');

      buffer = "";

      while ( fgets(command, std::size(command), checking) != NULL ) // copying FILE datatype info to an string for check the result
      {

        buffer += command;

      };

      pclose(checking);

      if ( buffer.find("_NET_WM_STATE_FOCUSED") != std::string::npos ) // checking focused state
      {
        return target;
      };

    if ( list[i] == '\0' ){ break; };
  };

  return "NULL";
};

// this function check the dimensions of the current focused window and send the split command
void determineDirection(string id){

  string propertiesList = "";
  char command[70], invert = 'Z' + 2;
  memset(command, std::size(command), '\0');
  string preCommand = "xwininfo -id " + id + " | grep 'Height"; // in this case, we use xwininfo for this task
  preCommand.push_back(invert);
  preCommand += "|Width'";

  FILE *properties = popen(preCommand.c_str(), "r");

  memset(command, std::size(command), '\0');

  while ( fgets(command, sizeof command, properties) ) // copying FILE datatype info to a string for get window resolution
  {

    propertiesList += command;

  };

  pclose(properties);

  bool Height = false;
  string sWidth = "", sHeight = "";

  for ( auto look : propertiesList ){ // filtering the result for convert string data to integrer

    if ( look == 'H' ){ Height = true; };
    if ( isdigit(look) && !Height ){ sWidth += look; }
    if ( isdigit(look) && Height ){ sHeight += look; };

  };

  int width = stoi(sWidth), height = stoi(sHeight); // converting string to integrer

  if ( width >= height ) // sending the split command
  {
    i3ipc_run_command("split h");
    
  }
  else
  {
    i3ipc_run_command("split v");
  };

};

int main(int argc, char **argv){

  string listOfWindows = "", focusedWindow = "";

  Display* display = XOpenDisplay(NULL); // creating a conection with X server

  Window allWIndows = RootWindow(display, 0); // getting the root window

  XSelectInput(display, allWIndows, EnterWindowMask | StructureNotifyMask );	// selecting the type of event to check

  while (True) {

    XEvent event;

    XNextEvent(display, &event);

    switch(event.type){

      case EnterNotify:

        listOfWindows = checkWindows();
        focusedWindow = checkFocused(listOfWindows);
        if ( focusedWindow == "NULL" ){ break; };
        determineDirection(focusedWindow);      

      break;

      case ClientMessage:

        listOfWindows = checkWindows();
        focusedWindow = checkFocused(listOfWindows);
        if ( focusedWindow == "NULL" ){ break; };
        determineDirection(focusedWindow);      
        break;

      default: break;
      };

    }


return 0;
}
