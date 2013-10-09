#ifndef _POLARHRM_CONFIG_H_
#define _POLARHRM_CONFIG_H_

#include <iostream>

/*
 *
 *  USER SETTINGS 
 *  update them to your accourding to your needs
 *
 */

/* the data are stored in PPP software and are used to wirte the HRM file */
static unsigned char USER_HR_MAX = 195;
static unsigned char USER_HR_REST = 86;
static unsigned char USER_V02_MAX = 51;
static unsigned char USER_WEIGHT = 77;

/* there is also data set at PPP software reffering to your Sport Zones 
 the values are compared with your HR Samples and are for statistical purpose 
 for graphics.
 I don t know if sportstracker or Golden Cheetah store this values themselve or
 just take the values form the given HR file (section [HRZones])

 NOTE: form highest to sallest value -> starting at HR MAX not more than 10
 values allowed because they are wirtten to HRM file 

 176 - USER_HR_MAX  -> MAX
 156 - 175 			-> HARD
 137 - 155			-> MODERAT

 and so on ...
 there is a structure for future purpose sportzone (some newer watches 
 have this values stored at watchsession e.g. rs400)
 */
#define USER_NUMBER_OF_SPORTZONES 11
static unsigned char user_sportzones[USER_NUMBER_OF_SPORTZONES] =
	{USER_HR_MAX,176,156,137,117,USER_HR_REST,0,0,0,0,0 };




/* HR Data are corrected during the parsing process 
 * when the HR sensor is not correctly messuring the HR 
 * this is during the training you don t see a value of HR on your watch display
 *
 * this gets fixed during processing at ppp software 
 * unforutnately I don t know the algorithem
 * see the session parsing function for details
 */
#define ENABLE_HR_FIXING




#endif