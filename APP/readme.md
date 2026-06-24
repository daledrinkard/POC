# APP  
## Tiny application framework for sample projects  
These files serve as a starting point for creating simple example projects.  
There are two example applications **base_app** and **blinky_app**.  
- **base_app**  
The basic application introduces app_entry() which serves as the entry point for a user application.  
It adds application_common.h to the user project.  At the top of this common include file is the following table.  
These macros define what components this application has.  For the **base_app** there are no additional components.  
  
| macro                 |     | definition                                       |  
| --------------------- | --- | ------------------------------------------------ |  
| APP_HAS_CONTROLPANEL  | (0) |  application project has a control panel         |  
| APP_HAS_CONSOLE       | (0) |  application project has a console               |  
| APP_HAS_CMD_SHELL     | (0) |  application project has a command intrepreter   |  
| APP_HAS_DEBUG_IO      | (0) |  application project embeds POP, DROP and TOG    |  
  
  

|                       |     |  this is useful in debugging with logic analyzer |  


