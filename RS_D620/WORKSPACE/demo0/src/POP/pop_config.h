/*
 * pop_config.h
 *
 *  Created on: Dec 16, 2025
 *      Author: daled
 */

#ifndef POP_POP_CONFIG_H_
#define POP_POP_CONFIG_H_

#define RA8P1_Arduino (0)
#define RA8M1_Arduino (0)
#define RA6M5_Arduino (1)


/*_____            ___  _____  __                       _       _
 |  __ \     /\   / _ \|  __ \/_ |        /\           | |     (_)
 | |__) |   /  \ | (_) | |__) || |______ /  \   _ __ __| |_   _ _ _ __   ___
 |  _  /   / /\ \ > _ <|  ___/ | |______/ /\ \ | '__/ _` | | | | | '_ \ / _ \
 | | \ \  / ____ \ (_) | |     | |     / ____ \| | | (_| | |_| | | | | | (_) |
 |_|  \_\/_/    \_\___/|_|     |_|    /_/    \_\_|  \__,_|\__,_|_|_| |_|\___/ */
#if RA8P1_Arduino
#define GPT_CLK_MHZ 240
#define POP_VALID_CNT 6
#define PP0 3
#define PN0  12
#define PP1 8
#define PN1  10
#define PP2 0
#define PN2  11
#define PP3 8
#define PN3  11
#define PP4 1
#define PN4  10
#define PP5 13
#define PN5  1
#define PP6 4
#define PN6  0
#define PP7 4
#define PN7  1
#define PPR 10
#define PNR  7
#define PPG 3
#define PNG  3
#define PPB 6
#define PNB  0
//#define PINMASK (0x000007FF)
#define PINMASK (0x000007FF)
#endif
/*
  _____             __  __  __ _____                      _       _             
 |  __ \     /\    / / |  \/  | ____|       /\           | |     (_)            
 | |__) |   /  \  / /_ | \  / | |__ ______ /  \   _ __ __| |_   _ _ _ __   ___  
 |  _  /   / /\ \| '_ \| |\/| |___ \______/ /\ \ | '__/ _` | | | | | '_ \ / _ \ 
 | | \ \  / ____ \ (_) | |  | |___) |    / ____ \| | | (_| | |_| | | | | | (_) |
 |_|  \_\/_/    \_\___/|_|  |_|____/    /_/    \_\_|  \__,_|\__,_|_|_| |_|\___/ 
                                                                                
                                                                                */
#if RA6M5_Arduino
#define GPT_CLK_MHZ 100
#define POP_VALID_CNT 8
#define PP0 6
#define PN0  8
#define PP1 1    // PS enable 12
#define PN1  12
#define PP2 4
#define PN2  9
#define PP3 1    // PS enable 11
#define PN3  11
#define PP4 1    // PS enable 15
#define PN4  15
#define PP5 2
#define PN5  7
#define PP6 5
#define PN6  11
#define PP7 5
#define PN7  12
#define PPR 0
#define PNR  8
#define PPG 0
#define PNG  7
#define PPB 0
#define PNB  6
//#define PINMASK (0x000007FF)
#define PINMASK (0x000007FF)

#endif
#endif /* POP_POP_CONFIG_H_ */
