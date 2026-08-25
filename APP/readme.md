# APP  
## Tiny application framework for sample projects  
These files serve as a starting point for creating simple example projects.  
Application complexity grows from.  
- Simple application that does nothing.
- An application that "controls" devices or behavior through a structure known as a Control Panel.  
- An application with a console  
- An application with a console and a command intrepreter  
  
All projects use a base application state sequencer **base_app**   
This file contains the code for the state sequencer that is common for all example projects.  
The following **weak** functions are defined:
- int app_func_reset   (void);
- int app_func_startup (void);
- int app_func_restart (void);
- int app_func_run     (void);  
  
These functions are called at the coresponding state APP.state  
Demos override these functions to perform the various necessary tasks, like reset, startup, etc.  

All source files include application_common.h, and it is expected that this file be modified by the user.  
Several #defines control how demo application projects are built and what they can do.  
  
| macro                 |     | definition                                       |  
| --------------------- | --- | ------------------------------------------------ |  
| APP_HAS_DEBUG_IO      | (0) |  add support for gpio debugging (requires LA) |  
| APP_HAS_CONTROLPANEL  | (0) |  add functionality for a control panel           |  
| APP_HAS_CONSOLE       | (0) |  add support for a console (requires a SCI uart) |  
| APP_HAS_CMD_SHELL     | (0) |  add a command list  (requires console)          |  


Consult the how_to_use.md file for instructions on how to use these files and folders.  