/**
* @file ConfigRW.h in FusionUnitXIH6
* @author rebeater
* @comment
* Create on 11/3/21 10:48 PM
* @version 1.0
**/

#ifndef FUSIONUNITXIH6_CORE_SRC_CONFIGRW_H_
#define FUSIONUNITXIH6_CORE_SRC_CONFIGRW_H_

#define END_OF_CONFIG "END OF CONFIG\n"

/**
 * Load config file from FATFS
 * @param path file path for config file
 * @param opt option for LooselyCouple
 * @return 0 for OK
 */
int LoadConfig(char *path, Option *opt);

/**
 * save config file to FATFS
 * @param path file_path for config file
 * @param opt Option to be saved
 */
void CreateConfigFile(char *path,Option *opt);
#endif //FUSIONUNITXIH6_CORE_SRC_CONFIGRW_H_
