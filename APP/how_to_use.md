# How to use.
## 1. Create a project  
- Create a **base bare minimum** project on the target MCU.  
If RTOS, create a thread/task and call it app_thread.  
## 2. Import source  
- Copy *application_common.h* and *application.c* from **base_app** or **blinky_app** to the src folder  
- **base_app**
This application provides a simple application frame that does nothing.  
It provides a basic structure for adding additional components.  
- **blinky_app**
This application provides an example of how to use the control panel to manipulate LEDs.  
Based on the classic "Blinky" template, this project extends that with LED control based on a control panel.  
It allows users to manipulate the led blink parameters at runtime using the debugger.  
  
## 3. Add components
- Copy folders into src.  
- Consult the **APP_HAS_xxx** configuration options in application_common.h.  
For the following \#defines that are (1), add these folders to the src folder:  

| macro                |  folder to add         | comments                                 |  
| -------------------- | ---------------------- | ---------------------------------------- |  
| APP_HAS_CONTROLPANEL |  $project/CPAN         | panels are application specific          |  
| APP_HAS_CONSOLE      |  console               | add a uart instance named g_console_uart |  
| APP_HAS_CMD_SHELL    |  commands              | edit commands.c to configure commands    |
| APP_HAS_DEBUG_IO     |  POP                   | edit configuration file for your pinning |  
## 4. Call app_entry()
- **bare metal**  
Declare void app_entry(void) in hal_entry.c and call in hal_entry().  
- **RTOS**  
Declare and call app_entry() in app_thread_entry().  


**NOTE**  Currently, only Azure works, and it has issues too  
What can I say, work in progress.

