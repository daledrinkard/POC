# BLINKY APP  
## Sample application to blink LEDs using a control panel  
This sample application attaches the boards LEDs to a control panel  
It illustrates using fields in the base CPAN to control the application at runtime.  
application_common.h is configured like:  

| macro                  |     | definition                                       |  
| ---------------------- | --- | ------------------------------------------------ |  
| APP_HAS_DEBUG_IO       | (0) |  no debug    |  
| APP_HAS_CONTROLPANEL  | **(1)** |  adds src\CPAN folder from the blinky_app        |  
| APP_HAS_CONSOLE        | (0) |  no console               |  
| APP_HAS_CMD_SHELL      | (0) |  no command list   |  


Consult the how_to_use.md file for instructions on how to use these files and folders.  