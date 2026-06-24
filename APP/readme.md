# How to use.
## 1. Create a project  
- Create a base bare minimum project on the target MCU.  
If RTOS, create a thread/task and call it app_thread.  
## 2. import source  
- **base_app**
This application provides a simple application frame that does nothing.  
It provides a basic structure for adding additional components.  
Import *application.c* and *application_common.h*  into the src folder.  
Edit application_common.h and verify that all APP_HAS_xxx defines are (0).  
- **blinky_app**
This application provides an example of how to use the control panel to manipulate LEDs.  
Based on the classic "Blinky" template, this project extends that with LED control based on a control panel.  
It allows users to manipulate the led blink parameters at runtime using the debugger.  
Import application.c, application_common.h and the CPAN folder into the src folder.  
Edit application_common.h and verify that all APP_HAS_CONTROLPANEL is (1).  Others should be (0).   
- **components**	
For the following defines, add these folders to the src folder:  
  APP_HAS_CONSOLE   (1)  add console  
  APP_HAS_CMD_SHELL (1)  add commands  
  APP_HAS_DEBUG_IO  (1)  add POP  
  
## 3. Call app_entry
- **bare metal**  
Declare void app_entry(void) in hal_entry.c and call in hal_entry().  
- **RTOS**  
Declare and call app_entry() in app_thread_entry().  


**NOTE**  Currently, only Azure works, and it has issues too  
What can I say, work in progress.

