/***************************************************************************
                          helper.cpp  -  description
                             -------------------
    begin                : Fri Jan 3 2003
    copyright            : (C) 2003 by Harald Roelle
    email                : roelle@informatik.uni-muenchen.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <iostream>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>


#include "OneWire++/LOW_network.h"
#include "OneWire++/LOW_helper_msglog.h"

#include "helper.h"

char menuKeys[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
                                 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                 'Y', 'Z' };
                                 

// from a news posting by Floyd Davidson
int hlp_kbhit()
{
  int                    cnt = 0;
  int                    error;
  static struct termios  Otty, Ntty;

  tcgetattr( 0, &Otty);
  Ntty = Otty;

  Ntty.c_iflag     = 0;	      /* input mode */
  Ntty.c_oflag     = 0;	      /* output mode */
  Ntty.c_lflag    &= ~ICANON; /* raw mode */
  Ntty.c_lflag &= ~ECHO;      /* disable echo */
  Ntty.c_cc[VMIN]  = CMIN;	  /* minimum time to wait */
  Ntty.c_cc[VTIME] = CTIME;   /* minimum characters to wait for */

  if (0 == (error = tcsetattr(0, TCSANOW, &Ntty))) {
    struct timeval	tv;
    error     += ioctl(0, FIONREAD, &cnt);
    error     += tcsetattr(0, TCSANOW, &Otty);
    tv.tv_sec  = 0;
    tv.tv_usec = 100;
    select(1, NULL, NULL, NULL, &tv);
  }

  return (error == 0 ? cnt : -1 );
}


// from a news posting by Floyd Davidson
int hlp_getch( bool inEcho, bool inFlush)
{
  char          ch;
  int           error, flag;
  static struct termios	Otty, Ntty;

  fflush( stdout);
  tcgetattr( 0, &Otty);
  Ntty = Otty;

  Ntty.c_iflag  =  0;       /* input mode		*/
  Ntty.c_oflag  =  0;       /* output mode		*/
  Ntty.c_lflag &= ~ICANON;  /* line settings 	*/

  if ( inEcho )
    /* enable echoing the char as it is typed */
    Ntty.c_lflag |=  ECHO;  /* enable echo	 	*/
  else
    /* disable echoing the char as it is typed */
    Ntty.c_lflag &= ~ECHO;  /* disable echo 	*/


  Ntty.c_cc[VMIN]  = 1;  /* minimum time to wait	*/
  Ntty.c_cc[VTIME] = 1;  /* minimum chars to wait for */

  if ( inFlush )
    flag = TCSAFLUSH;
  else
    flag = TCSANOW;

  if (0 == (error = tcsetattr( 0, flag, &Ntty))) {
    error  = read( 0, &ch, 1 );            /* get char from stdin */
    error += tcsetattr( 0, flag, &Otty);   /* restore old settings */
  }

  return (error == 1 ? static_cast<int>(ch) : -1 );
}


void hlp_keyToCont()
{
  printf( "Press any key to continue...");
  hlp_getch( false, false);
  printf( "\n");
}


int hlp_getMenuKey( const char *inAcceptChars)
{
  printf( "\n");
  printf( "-> ");
  while( true ) {
    int key = hlp_getch( false, true);
    if ( index( inAcceptChars, key) != 0 ) {
      printf( "%c\n", key);
      return key;
    }
    if ( key == 27 ) // ESC
      throw hlp_userAbort( "Selection aborted by user.", __FILE__, __LINE__);
  }
}


unsigned int hlp_selectSingleOption( std::string inMsg, std::vector<std::string> inOptions, unsigned int inDefault)
{
  if ( inOptions.size() > MAX_MENU_ENTRIES ) {
    LOW_helper_msglog::printError( "Too many options to handle in menu. Exiting...");
    exit( 1);
  }

  if ( inDefault >= inOptions.size() ) {
    LOW_helper_msglog::printError( "Default out of range. Exiting...");
    exit( 1);
  }

  char acceptChars[MAX_MENU_ENTRIES+1];
  strncpy( acceptChars, menuKeys, inOptions.size());
  acceptChars[inOptions.size()] = 0;
  strcat( acceptChars, "\r");
  
  hlp_printDashline();
  printf( "%s:\n", inMsg.c_str());
  for( unsigned int a=0; a<inOptions.size(); a++) {
    printf( "  <%c> %s %s\n", menuKeys[a], inOptions[a].c_str(), a==inDefault?"(DEFAULT)":"");
  }
  printf( "\n");
  printf( "Select option -> ");

  int key;
  while( true ) {
    key = hlp_getch( false, true);
    if ( index( acceptChars, key) != 0 ) {
      break;
    }
    if ( key == 27 ) // ESC
      throw hlp_userAbort( "Selection aborted by user.", __FILE__, __LINE__);
  }

  if ( key == '\r' ) {
    printf( "%c\n", acceptChars[inDefault]);
    return inDefault;
  }
    
  printf( "%c\n", key);
  unsigned int selected = index( menuKeys, key) - menuKeys;

  return selected;
}

bool hlp_selectBoolOption( std::string inMsg, bool inDefault)
{
  hlp_printDashline();
  printf( "%s:\n", inMsg.c_str());
  printf( "  <y> Yes %s\n", inDefault==true?"(DEFAULT)":"");
  printf( "  <n> No %s\n",  inDefault==false?"(DEFAULT)":"");
  printf( "\n");
  printf( "Select -> ");

  int key;
  while( true ) {
    key = hlp_getch( false, true);
    if ( index( "\rny", key) != 0 ) {
      break;
    }
    if ( key == 27 ) // ESC
      throw hlp_userAbort( "Selection aborted by user.", __FILE__, __LINE__);
  }

  if ( key == '\r' ) {
    printf( "%c\n", inDefault?'y':'n');
    return inDefault;
  }
  if ( key == 'n' ) {
    printf( "n\n");
    return false;
  }
  if ( key == 'y' ) {
    printf( "y\n");
    return true;
  }

  return inDefault; // to satisfy compiler
}


unsigned int hlp_getTermLineLength()
{
#ifdef TIOCGWINSZ
  struct winsize size;
  if ( ioctl( STDIN_FILENO, TIOCGWINSZ, reinterpret_cast<char*>(&size)) < 0 )
    return 80;
  else
    return size.ws_col;
#else
  return 80;
#endif
}


void hlp_printDashline()
{
  for ( unsigned int a=0; a<hlp_getTermLineLength()-1; a++)
  {
    printf( "=");
  }
  printf( "\n");
}
