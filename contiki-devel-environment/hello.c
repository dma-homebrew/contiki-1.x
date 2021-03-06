/*
 * Copyright (c) 2003, Adam Dunkels.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution. 
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgement:
 *        This product includes software developed by Adam Dunkels. 
 * 4. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.  
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 *
 * This an example program for the Contiki desktop OS
 *
 * $Id: hello.c,v 1.3 2003/04/10 07:02:05 adamdunkels Exp $
 *
 */

/* This is an example of how to write programs for Contiki. It
   displays a window with the famous message "Hello world" and two
   buttons; one which changes the message slightly and one which quits
   the application. */

#include "ctk.h"
#include "dispatcher.h"
#include "loader.h"

static struct ctk_window window;

static struct ctk_label hellolabel =
  {CTK_LABEL(0, 0, 8, 1, "Hello")};
static struct ctk_label worldlabel =
  {CTK_LABEL(8, 1, 6, 1, "world!")};

static struct ctk_button updatebutton =
  {CTK_BUTTON(0, 3, 6, "Update")};
static struct ctk_button closebutton =
  {CTK_BUTTON(8, 3, 5, "Close")};

static DISPATCHER_SIGHANDLER(hello_sighandler, s, data);
static struct dispatcher_proc p =
  {DISPATCHER_PROC("Hello world", NULL, hello_sighandler, NULL)};
static ek_id_t id;

/*-----------------------------------------------------------------------------------*/
LOADER_INIT_FUNC(hello_init)
{
  if(id == EK_ID_NONE) {
    id = dispatcher_start(&p);
    
    ctk_window_new(&window, 15, 4, "Hello");

    CTK_WIDGET_ADD(&window, &hellolabel);
    CTK_WIDGET_ADD(&window, &worldlabel);

    CTK_WIDGET_ADD(&window, &updatebutton);
    CTK_WIDGET_ADD(&window, &closebutton);

    CTK_WIDGET_FOCUS(&window, &updatebutton);
        
    dispatcher_listen(ctk_signal_button_activate);
    dispatcher_listen(ctk_signal_window_close);
  }
  ctk_window_open(&window);
}
/*-----------------------------------------------------------------------------------*/
static void
hello_quit(void)
{
  dispatcher_exit(&p);
  id = EK_ID_NONE;
  LOADER_UNLOAD();
  ctk_redraw();
}
/*-----------------------------------------------------------------------------------*/
static DISPATCHER_SIGHANDLER(hello_sighandler, s, data)
{
  DISPATCHER_SIGHANDLER_ARGS(s, data);
    
  if(s == ctk_signal_button_activate) {
    if(data == (ek_data_t)&updatebutton) {
      ctk_label_set_text(&hellolabel, "Good bye");
      CTK_WIDGET_REDRAW(&hellolabel);
    } else if(data == (ek_data_t)&closebutton) {
      ctk_window_close(&window);
      hello_quit();
    }
  } else if(s == ctk_signal_window_close &&
	    data == (ek_data_t)&window) {
    hello_quit();
  }
}
/*-----------------------------------------------------------------------------------*/
