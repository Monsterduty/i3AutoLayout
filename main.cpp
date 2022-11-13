//Coded by Monsterduty

#include <X11/Xlib.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#define I3IPC_IMPLEMENTATION
#include "i3ipc.h"
#include "help.hpp"

using std::system;
using std::cout;
using std::endl;
using std::string;

//========================================================================================
//GLOBAL VARIABLES
//========================================================================================

  string listOfWindows = "", focusedWindow = "";
  unsigned int width = 0, height = 0;
  Display *display;
  Window focus;
  Window who;
  int a = 0, b = 0;
  unsigned int c,d;
  int revert;
  bool output = false;
  bool daemonize = false;

//========================================================================================

void calculateSplit();

void help();

int errorCalback( Display *dpy, XErrorEvent *err );

int main(int argc, char **argv){

  if ( argc > 1 )
    for ( int i = 1; i < argc; i++ )
    {
      string aux = argv[i];
      if ( aux.find("--help") != string::npos ) help();
      else
      if ( aux.find("--daemon") != string::npos ) daemonize = true;
      else
      if ( aux.find("--output") != string::npos ) output = true;
      else
      {
        cout << "unrecongnized option ( " << aux << " ) " << endl;
        help();
      }
    }

  XSetErrorHandler( errorCalback );

  display = XOpenDisplay(NULL); // creating a conection with X server

  Window allWIndows = RootWindow(display, 0); // getting the root window

  XSelectInput(display, allWIndows, LeaveWindowMask | EnterWindowMask | SubstructureNotifyMask | StructureNotifyMask );	// selecting the type of event to check

  I3ipc_event *i3Event;

  if ( daemonize )
    daemon(0, 0);

  while (True)
  {
    
    XEvent event;

    XNextEvent(display, &event);

    if ( output )
      cout << event.type << endl;

    switch(event.type)
    {
      case LeaveNotify:
        calculateSplit();
        break;

      case EnterNotify:
        calculateSplit();
        break;

      case MapNotify:
        calculateSplit();
        break;

      case ConfigureNotify:
        calculateSplit();
        break;

      default: break;
    };

  }

return 0;
}


void calculateSplit()
{
  XGetInputFocus(display, &focus, &revert); // returning the window focused
  XGetGeometry(display, focus, &who, &a, &b, &width, &height, &c, &d); //getting the geometry that we will use for calculate the position for a new window

  if ( width >= height )
  {
    if ( output )
      cout << "horizontal" << endl;
    i3ipc_run_command("split h");
  };
  if ( height > width )
  {
    if ( output )
      cout << "vertical" << endl;
    i3ipc_run_command("split v");
  };

}

int errorCalback( Display *dpy, XErrorEvent *err )
{
  // this is a empty handler to prevent app crash if the requested windows does not exist!
  // also you can use this for your needs.
  return 0;
};

void help()
{
  cout << helpMessage << endl;
  exit(0);
}