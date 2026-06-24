# How to use.
## 1. Create a project  
- Create a base bare minimum project on the target MCU. 
If RTOS, create a thread/task and call it app_thread.  
## 2. import source  
- **base_app**
This application provides a simple application frame that does nothing.  
It provides a basic structure for adding additional components.  
Import application.c and application_common.h  into the src folder.  
- **blinky_app**
This application provides an example of how to use the control panel to manipulate LEDs.  
Based on the classic "Blinky" template, this project extends that with LED control based on a control panel.
It allows users to manipulate the led blink parameters at runtime using the debugger.
- **components**	
Copy all the files and folders to your src folder.

Bare Metal, in hal_entry call app_entry.
RTOS, in app_thread_entry call app_entry.


