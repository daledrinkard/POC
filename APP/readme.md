How to use.
Create a base project on the target MCU. Choose bare minimum.  If RTOS, create an app_thread.
Select the application template that best suits your needs:
APP +
    +--> base_app   (basic application state machine that does nothing)
	|
	+--> blinky_app (adds a Control Panel and connects the leds)
	|
	+--> blinky_app_console (adds a console to the blinky_app)
	
Copy all the files and folders to your src folder.

Bare Metal, in hal_entry call app_entry.
RTOS, in app_thread_entry call app_entry.


