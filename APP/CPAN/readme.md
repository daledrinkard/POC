# CPAN  
## Control Panel for APP projects  
These files serve as a starting point for creating an application control panel.  
A *control panel* is a simple structure that contains runtime information.  
This is a basic control panel that contains the following fields.  
1. **stat** is a 32bit bit-field register manipulated by the application through defined bits.  
These bits are used by the application code to indicate the current status or to direct the  
application to take action "stat", or now.  
pre-defined bits:  

| macro                 | value        | definition  
| --------------------- | ------------ | ------------------------------------------ |  
| CPAN_STAT_UPDATE      | (0x00000001) |  Indicates that the panel needs updating |  
| CPAN_STAT_RESTART_APP      | (0x00000002) |  Instructs the APP to restart |  

2. **creg**[] a group of 32bit registers that can be used for general purpose things.  

The following commands are implemented in **CP_commands.c** (console required)  
- **stat**  displays the current value of stat  
- **stat** [S|C] *value* sets or clears the bits in value  
- **creg** X [*value* ] read or write creg[X]  

