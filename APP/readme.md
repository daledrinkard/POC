# APP  
## Tiny application framework for sample projects  
These files serve as a starting point for creating simple example projects.  
Application complexity grows from.  
- Simple application that does nothing.
- An application that "controls" devices or behavior through a structure known as a Control Panel.  
- An application with a console  
- An application with a console and a command intrepreter  
  
There are two example applications **base_app** and **blinky_app**.  
- **base_app**  
The basic application introduces app_entry() which serves as the entry point for a user application.  
It adds *application_common.h* to the user project.  This common \#include is used globally.  
At the top of this common include file is the following table.  
These macros define what components this application has.  
For the **base_app** there are no additional components.  
  
| macro                 |     | definition                                       |  
| --------------------- | --- | ------------------------------------------------ |  
| APP_HAS_DEBUG_IO      | (0) |  add support for gpio debugging (requires LA) |  
| APP_HAS_CONTROLPANEL  | (0) |  add functionality for a control panel           |  
| APP_HAS_CONSOLE       | (0) |  add support for a console (requires a SCI uart) |  
| APP_HAS_CMD_SHELL     | (0) |  add a command list  (requires console)          |  

- **blinky_app**  
The blinky application introduces a control panel that allows manipulation of the LEDs at runtime through the control panel.  
The ***application.c*** code is modified to blink the leds at a rate and pattern set in the control panel.  
  
| macro                  |     | definition                                       |  
| ---------------------- | --- | ------------------------------------------------ |  
| APP_HAS_DEBUG_IO       | (0) |  no debug    |  
| APP_HAS_CONTROLPANEL  | **(1)** |  adds src\CPAN folder from the blinky_app        |  
| APP_HAS_CONSOLE        | (0) |  no console               |  
| APP_HAS_CMD_SHELL      | (0) |  no command list   |  

Consult the how_to_use.md file for instructions on how to use these files and folders.  