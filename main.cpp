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

int main(int argc, char **argv){

  string listOfWindows = "", focusedWindow = "";
  unsigned int width = 0, height = 0;
  Window focus;
  Window who;
  int a = 0, b = 0;
  unsigned int c,d;
  int revert;

  Display* display = XOpenDisplay(NULL); // creating a conection with X server

  Window allWIndows = RootWindow(display, 0); // getting the root window

  XSelectInput(display, allWIndows, LeaveWindowMask | EnterWindowMask | SubstructureNotifyMask | StructureNotifyMask );	// selecting the type of event to check

  while (True)
  {

    XEvent event;

    XNextEvent(display, &event);

    switch(event.type)
    {
      case MapNotify:

        XGetInputFocus(display, &focus, &revert); // returning the window focused
        XGetGeometry(display, focus, &who, &a, &b, &width, &height, &c, &d); //getting the geometry that whe will use for calculate the position for a new window
        if ( width >= height ){ i3ipc_run_command("split h"); };
        if ( height > width ) { i3ipc_run_command("split v"); };

        break;

      default: break;
    };

  }


return 0;
}
