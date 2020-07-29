/*!
 * \file      eeprom-board.c
 *
 * \brief     Target board EEPROM driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include "sdkconfig.h"
#include "esp_log.h"
#include "eeprom-board.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "utilities.h"




/** Local Definitions **/


//								_123456789012345 //MAXIMUM 15 Chars
#define key_dummy				"dummy"			// Dummy
#define key_93C66_Adr0000		"k93C66A0000"	// Key for EEPROM Address 93C66 Address 0000
#define key_93C66_Adr0001		"k93C66A0001"	// Key for EEPROM Address 93C66 Address 0001
#define key_93C66_Adr0002		"k93C66A0002"	// Key for EEPROM Address 93C66 Address 0002
#define key_93C66_Adr0003		"k93C66A0003"	// Key for EEPROM Address 93C66 Address 0003
#define key_93C66_Adr0004		"k93C66A0004"	// Key for EEPROM Address 93C66 Address 0004
#define key_93C66_Adr0005		"k93C66A0005"	// Key for EEPROM Address 93C66 Address 0005
#define key_93C66_Adr0006		"k93C66A0006"	// Key for EEPROM Address 93C66 Address 0006
#define key_93C66_Adr0007		"k93C66A0007"	// Key for EEPROM Address 93C66 Address 0007
#define key_93C66_Adr0008		"k93C66A0008"	// Key for EEPROM Address 93C66 Address 0008
#define key_93C66_Adr0009		"k93C66A0009"	// Key for EEPROM Address 93C66 Address 0009
#define key_93C66_Adr0010		"k93C66A0010"	// Key for EEPROM Address 93C66 Address 0010
#define key_93C66_Adr0011		"k93C66A0011"	// Key for EEPROM Address 93C66 Address 0011
#define key_93C66_Adr0012		"k93C66A0012"	// Key for EEPROM Address 93C66 Address 0012
#define key_93C66_Adr0013		"k93C66A0013"	// Key for EEPROM Address 93C66 Address 0013
#define key_93C66_Adr0014		"k93C66A0014"	// Key for EEPROM Address 93C66 Address 0014
#define key_93C66_Adr0015		"k93C66A0015"	// Key for EEPROM Address 93C66 Address 0015
#define key_93C66_Adr0016		"k93C66A0016"	// Key for EEPROM Address 93C66 Address 0016
#define key_93C66_Adr0017		"k93C66A0017"	// Key for EEPROM Address 93C66 Address 0017
#define key_93C66_Adr0018		"k93C66A0018"	// Key for EEPROM Address 93C66 Address 0018
#define key_93C66_Adr0019		"k93C66A0019"	// Key for EEPROM Address 93C66 Address 0019
#define key_93C66_Adr0020		"k93C66A0020"	// Key for EEPROM Address 93C66 Address 0020
#define key_93C66_Adr0021		"k93C66A0021"	// Key for EEPROM Address 93C66 Address 0021
#define key_93C66_Adr0022		"k93C66A0022"	// Key for EEPROM Address 93C66 Address 0022
#define key_93C66_Adr0023		"k93C66A0023"	// Key for EEPROM Address 93C66 Address 0023
#define key_93C66_Adr0024		"k93C66A0024"	// Key for EEPROM Address 93C66 Address 0024
#define key_93C66_Adr0025		"k93C66A0025"	// Key for EEPROM Address 93C66 Address 0025
#define key_93C66_Adr0026		"k93C66A0026"	// Key for EEPROM Address 93C66 Address 0026
#define key_93C66_Adr0027		"k93C66A0027"	// Key for EEPROM Address 93C66 Address 0027
#define key_93C66_Adr0028		"k93C66A0028"	// Key for EEPROM Address 93C66 Address 0028
#define key_93C66_Adr0029		"k93C66A0029"	// Key for EEPROM Address 93C66 Address 0029
#define key_93C66_Adr0030		"k93C66A0030"	// Key for EEPROM Address 93C66 Address 0030
#define key_93C66_Adr0031		"k93C66A0031"	// Key for EEPROM Address 93C66 Address 0031
#define key_93C66_Adr0032		"k93C66A0032"	// Key for EEPROM Address 93C66 Address 0032
#define key_93C66_Adr0033		"k93C66A0033"	// Key for EEPROM Address 93C66 Address 0033
#define key_93C66_Adr0034		"k93C66A0034"	// Key for EEPROM Address 93C66 Address 0034
#define key_93C66_Adr0035		"k93C66A0035"	// Key for EEPROM Address 93C66 Address 0035
#define key_93C66_Adr0036		"k93C66A0036"	// Key for EEPROM Address 93C66 Address 0036
#define key_93C66_Adr0037		"k93C66A0037"	// Key for EEPROM Address 93C66 Address 0037
#define key_93C66_Adr0038		"k93C66A0038"	// Key for EEPROM Address 93C66 Address 0038
#define key_93C66_Adr0039		"k93C66A0039"	// Key for EEPROM Address 93C66 Address 0039
#define key_93C66_Adr0040		"k93C66A0040"	// Key for EEPROM Address 93C66 Address 0040
#define key_93C66_Adr0041		"k93C66A0041"	// Key for EEPROM Address 93C66 Address 0041
#define key_93C66_Adr0042		"k93C66A0042"	// Key for EEPROM Address 93C66 Address 0042
#define key_93C66_Adr0043		"k93C66A0043"	// Key for EEPROM Address 93C66 Address 0043
#define key_93C66_Adr0044		"k93C66A0044"	// Key for EEPROM Address 93C66 Address 0044
#define key_93C66_Adr0045		"k93C66A0045"	// Key for EEPROM Address 93C66 Address 0045
#define key_93C66_Adr0046		"k93C66A0046"	// Key for EEPROM Address 93C66 Address 0046
#define key_93C66_Adr0047		"k93C66A0047"	// Key for EEPROM Address 93C66 Address 0047
#define key_93C66_Adr0048		"k93C66A0048"	// Key for EEPROM Address 93C66 Address 0048
#define key_93C66_Adr0049		"k93C66A0049"	// Key for EEPROM Address 93C66 Address 0049
#define key_93C66_Adr0050		"k93C66A0050"	// Key for EEPROM Address 93C66 Address 0050
#define key_93C66_Adr0051		"k93C66A0051"	// Key for EEPROM Address 93C66 Address 0051
#define key_93C66_Adr0052		"k93C66A0052"	// Key for EEPROM Address 93C66 Address 0052
#define key_93C66_Adr0053		"k93C66A0053"	// Key for EEPROM Address 93C66 Address 0053
#define key_93C66_Adr0054		"k93C66A0054"	// Key for EEPROM Address 93C66 Address 0054
#define key_93C66_Adr0055		"k93C66A0055"	// Key for EEPROM Address 93C66 Address 0055
#define key_93C66_Adr0056		"k93C66A0056"	// Key for EEPROM Address 93C66 Address 0056
#define key_93C66_Adr0057		"k93C66A0057"	// Key for EEPROM Address 93C66 Address 0057
#define key_93C66_Adr0058		"k93C66A0058"	// Key for EEPROM Address 93C66 Address 0058
#define key_93C66_Adr0059		"k93C66A0059"	// Key for EEPROM Address 93C66 Address 0059
#define key_93C66_Adr0060		"k93C66A0060"	// Key for EEPROM Address 93C66 Address 0060
#define key_93C66_Adr0061		"k93C66A0061"	// Key for EEPROM Address 93C66 Address 0061
#define key_93C66_Adr0062		"k93C66A0062"	// Key for EEPROM Address 93C66 Address 0062
#define key_93C66_Adr0063		"k93C66A0063"	// Key for EEPROM Address 93C66 Address 0063
#define key_93C66_Adr0064		"k93C66A0064"	// Key for EEPROM Address 93C66 Address 0064
#define key_93C66_Adr0065		"k93C66A0065"	// Key for EEPROM Address 93C66 Address 0065
#define key_93C66_Adr0066		"k93C66A0066"	// Key for EEPROM Address 93C66 Address 0066
#define key_93C66_Adr0067		"k93C66A0067"	// Key for EEPROM Address 93C66 Address 0067
#define key_93C66_Adr0068		"k93C66A0068"	// Key for EEPROM Address 93C66 Address 0068
#define key_93C66_Adr0069		"k93C66A0069"	// Key for EEPROM Address 93C66 Address 0069
#define key_93C66_Adr0070		"k93C66A0070"	// Key for EEPROM Address 93C66 Address 0070
#define key_93C66_Adr0071		"k93C66A0071"	// Key for EEPROM Address 93C66 Address 0071
#define key_93C66_Adr0072		"k93C66A0072"	// Key for EEPROM Address 93C66 Address 0072
#define key_93C66_Adr0073		"k93C66A0073"	// Key for EEPROM Address 93C66 Address 0073
#define key_93C66_Adr0074		"k93C66A0074"	// Key for EEPROM Address 93C66 Address 0074
#define key_93C66_Adr0075		"k93C66A0075"	// Key for EEPROM Address 93C66 Address 0075
#define key_93C66_Adr0076		"k93C66A0076"	// Key for EEPROM Address 93C66 Address 0076
#define key_93C66_Adr0077		"k93C66A0077"	// Key for EEPROM Address 93C66 Address 0077
#define key_93C66_Adr0078		"k93C66A0078"	// Key for EEPROM Address 93C66 Address 0078
#define key_93C66_Adr0079		"k93C66A0079"	// Key for EEPROM Address 93C66 Address 0079
#define key_93C66_Adr0080		"k93C66A0080"	// Key for EEPROM Address 93C66 Address 0080
#define key_93C66_Adr0081		"k93C66A0081"	// Key for EEPROM Address 93C66 Address 0081
#define key_93C66_Adr0082		"k93C66A0082"	// Key for EEPROM Address 93C66 Address 0082
#define key_93C66_Adr0083		"k93C66A0083"	// Key for EEPROM Address 93C66 Address 0083
#define key_93C66_Adr0084		"k93C66A0084"	// Key for EEPROM Address 93C66 Address 0084
#define key_93C66_Adr0085		"k93C66A0085"	// Key for EEPROM Address 93C66 Address 0085
#define key_93C66_Adr0086		"k93C66A0086"	// Key for EEPROM Address 93C66 Address 0086
#define key_93C66_Adr0087		"k93C66A0087"	// Key for EEPROM Address 93C66 Address 0087
#define key_93C66_Adr0088		"k93C66A0088"	// Key for EEPROM Address 93C66 Address 0088
#define key_93C66_Adr0089		"k93C66A0089"	// Key for EEPROM Address 93C66 Address 0089
#define key_93C66_Adr0090		"k93C66A0090"	// Key for EEPROM Address 93C66 Address 0090
#define key_93C66_Adr0091		"k93C66A0091"	// Key for EEPROM Address 93C66 Address 0091
#define key_93C66_Adr0092		"k93C66A0092"	// Key for EEPROM Address 93C66 Address 0092
#define key_93C66_Adr0093		"k93C66A0093"	// Key for EEPROM Address 93C66 Address 0093
#define key_93C66_Adr0094		"k93C66A0094"	// Key for EEPROM Address 93C66 Address 0094
#define key_93C66_Adr0095		"k93C66A0095"	// Key for EEPROM Address 93C66 Address 0095
#define key_93C66_Adr0096		"k93C66A0096"	// Key for EEPROM Address 93C66 Address 0096
#define key_93C66_Adr0097		"k93C66A0097"	// Key for EEPROM Address 93C66 Address 0097
#define key_93C66_Adr0098		"k93C66A0098"	// Key for EEPROM Address 93C66 Address 0098
#define key_93C66_Adr0099		"k93C66A0099"	// Key for EEPROM Address 93C66 Address 0099
#define key_93C66_Adr0100		"k93C66A0100"	// Key for EEPROM Address 93C66 Address 0100
#define key_93C66_Adr0101		"k93C66A0101"	// Key for EEPROM Address 93C66 Address 0101
#define key_93C66_Adr0102		"k93C66A0102"	// Key for EEPROM Address 93C66 Address 0102
#define key_93C66_Adr0103		"k93C66A0103"	// Key for EEPROM Address 93C66 Address 0103
#define key_93C66_Adr0104		"k93C66A0104"	// Key for EEPROM Address 93C66 Address 0104
#define key_93C66_Adr0105		"k93C66A0105"	// Key for EEPROM Address 93C66 Address 0105
#define key_93C66_Adr0106		"k93C66A0106"	// Key for EEPROM Address 93C66 Address 0106
#define key_93C66_Adr0107		"k93C66A0107"	// Key for EEPROM Address 93C66 Address 0107
#define key_93C66_Adr0108		"k93C66A0108"	// Key for EEPROM Address 93C66 Address 0108
#define key_93C66_Adr0109		"k93C66A0109"	// Key for EEPROM Address 93C66 Address 0109
#define key_93C66_Adr0110		"k93C66A0110"	// Key for EEPROM Address 93C66 Address 0110
#define key_93C66_Adr0111		"k93C66A0111"	// Key for EEPROM Address 93C66 Address 0111
#define key_93C66_Adr0112		"k93C66A0112"	// Key for EEPROM Address 93C66 Address 0112
#define key_93C66_Adr0113		"k93C66A0113"	// Key for EEPROM Address 93C66 Address 0113
#define key_93C66_Adr0114		"k93C66A0114"	// Key for EEPROM Address 93C66 Address 0114
#define key_93C66_Adr0115		"k93C66A0115"	// Key for EEPROM Address 93C66 Address 0115
#define key_93C66_Adr0116		"k93C66A0116"	// Key for EEPROM Address 93C66 Address 0116
#define key_93C66_Adr0117		"k93C66A0117"	// Key for EEPROM Address 93C66 Address 0117
#define key_93C66_Adr0118		"k93C66A0118"	// Key for EEPROM Address 93C66 Address 0118
#define key_93C66_Adr0119		"k93C66A0119"	// Key for EEPROM Address 93C66 Address 0119
#define key_93C66_Adr0120		"k93C66A0120"	// Key for EEPROM Address 93C66 Address 0120
#define key_93C66_Adr0121		"k93C66A0121"	// Key for EEPROM Address 93C66 Address 0121
#define key_93C66_Adr0122		"k93C66A0122"	// Key for EEPROM Address 93C66 Address 0122
#define key_93C66_Adr0123		"k93C66A0123"	// Key for EEPROM Address 93C66 Address 0123
#define key_93C66_Adr0124		"k93C66A0124"	// Key for EEPROM Address 93C66 Address 0124
#define key_93C66_Adr0125		"k93C66A0125"	// Key for EEPROM Address 93C66 Address 0125
#define key_93C66_Adr0126		"k93C66A0126"	// Key for EEPROM Address 93C66 Address 0126
#define key_93C66_Adr0127		"k93C66A0127"	// Key for EEPROM Address 93C66 Address 0127
#define key_93C66_Adr0128		"k93C66A0128"	// Key for EEPROM Address 93C66 Address 0128
#define key_93C66_Adr0129		"k93C66A0129"	// Key for EEPROM Address 93C66 Address 0129
#define key_93C66_Adr0130		"k93C66A0130"	// Key for EEPROM Address 93C66 Address 0130
#define key_93C66_Adr0131		"k93C66A0131"	// Key for EEPROM Address 93C66 Address 0131
#define key_93C66_Adr0132		"k93C66A0132"	// Key for EEPROM Address 93C66 Address 0132
#define key_93C66_Adr0133		"k93C66A0133"	// Key for EEPROM Address 93C66 Address 0133
#define key_93C66_Adr0134		"k93C66A0134"	// Key for EEPROM Address 93C66 Address 0134
#define key_93C66_Adr0135		"k93C66A0135"	// Key for EEPROM Address 93C66 Address 0135
#define key_93C66_Adr0136		"k93C66A0136"	// Key for EEPROM Address 93C66 Address 0136
#define key_93C66_Adr0137		"k93C66A0137"	// Key for EEPROM Address 93C66 Address 0137
#define key_93C66_Adr0138		"k93C66A0138"	// Key for EEPROM Address 93C66 Address 0138
#define key_93C66_Adr0139		"k93C66A0139"	// Key for EEPROM Address 93C66 Address 0139
#define key_93C66_Adr0140		"k93C66A0140"	// Key for EEPROM Address 93C66 Address 0140
#define key_93C66_Adr0141		"k93C66A0141"	// Key for EEPROM Address 93C66 Address 0141
#define key_93C66_Adr0142		"k93C66A0142"	// Key for EEPROM Address 93C66 Address 0142
#define key_93C66_Adr0143		"k93C66A0143"	// Key for EEPROM Address 93C66 Address 0143
#define key_93C66_Adr0144		"k93C66A0144"	// Key for EEPROM Address 93C66 Address 0144
#define key_93C66_Adr0145		"k93C66A0145"	// Key for EEPROM Address 93C66 Address 0145
#define key_93C66_Adr0146		"k93C66A0146"	// Key for EEPROM Address 93C66 Address 0146
#define key_93C66_Adr0147		"k93C66A0147"	// Key for EEPROM Address 93C66 Address 0147
#define key_93C66_Adr0148		"k93C66A0148"	// Key for EEPROM Address 93C66 Address 0148
#define key_93C66_Adr0149		"k93C66A0149"	// Key for EEPROM Address 93C66 Address 0149
#define key_93C66_Adr0150		"k93C66A0150"	// Key for EEPROM Address 93C66 Address 0150
#define key_93C66_Adr0151		"k93C66A0151"	// Key for EEPROM Address 93C66 Address 0151
#define key_93C66_Adr0152		"k93C66A0152"	// Key for EEPROM Address 93C66 Address 0152
#define key_93C66_Adr0153		"k93C66A0153"	// Key for EEPROM Address 93C66 Address 0153
#define key_93C66_Adr0154		"k93C66A0154"	// Key for EEPROM Address 93C66 Address 0154
#define key_93C66_Adr0155		"k93C66A0155"	// Key for EEPROM Address 93C66 Address 0155
#define key_93C66_Adr0156		"k93C66A0156"	// Key for EEPROM Address 93C66 Address 0156
#define key_93C66_Adr0157		"k93C66A0157"	// Key for EEPROM Address 93C66 Address 0157
#define key_93C66_Adr0158		"k93C66A0158"	// Key for EEPROM Address 93C66 Address 0158
#define key_93C66_Adr0159		"k93C66A0159"	// Key for EEPROM Address 93C66 Address 0159
#define key_93C66_Adr0160		"k93C66A0160"	// Key for EEPROM Address 93C66 Address 0160
#define key_93C66_Adr0161		"k93C66A0161"	// Key for EEPROM Address 93C66 Address 0161
#define key_93C66_Adr0162		"k93C66A0162"	// Key for EEPROM Address 93C66 Address 0162
#define key_93C66_Adr0163		"k93C66A0163"	// Key for EEPROM Address 93C66 Address 0163
#define key_93C66_Adr0164		"k93C66A0164"	// Key for EEPROM Address 93C66 Address 0164
#define key_93C66_Adr0165		"k93C66A0165"	// Key for EEPROM Address 93C66 Address 0165
#define key_93C66_Adr0166		"k93C66A0166"	// Key for EEPROM Address 93C66 Address 0166
#define key_93C66_Adr0167		"k93C66A0167"	// Key for EEPROM Address 93C66 Address 0167
#define key_93C66_Adr0168		"k93C66A0168"	// Key for EEPROM Address 93C66 Address 0168
#define key_93C66_Adr0169		"k93C66A0169"	// Key for EEPROM Address 93C66 Address 0169
#define key_93C66_Adr0170		"k93C66A0170"	// Key for EEPROM Address 93C66 Address 0170
#define key_93C66_Adr0171		"k93C66A0171"	// Key for EEPROM Address 93C66 Address 0171
#define key_93C66_Adr0172		"k93C66A0172"	// Key for EEPROM Address 93C66 Address 0172
#define key_93C66_Adr0173		"k93C66A0173"	// Key for EEPROM Address 93C66 Address 0173
#define key_93C66_Adr0174		"k93C66A0174"	// Key for EEPROM Address 93C66 Address 0174
#define key_93C66_Adr0175		"k93C66A0175"	// Key for EEPROM Address 93C66 Address 0175
#define key_93C66_Adr0176		"k93C66A0176"	// Key for EEPROM Address 93C66 Address 0176
#define key_93C66_Adr0177		"k93C66A0177"	// Key for EEPROM Address 93C66 Address 0177
#define key_93C66_Adr0178		"k93C66A0178"	// Key for EEPROM Address 93C66 Address 0178
#define key_93C66_Adr0179		"k93C66A0179"	// Key for EEPROM Address 93C66 Address 0179
#define key_93C66_Adr0180		"k93C66A0180"	// Key for EEPROM Address 93C66 Address 0180
#define key_93C66_Adr0181		"k93C66A0181"	// Key for EEPROM Address 93C66 Address 0181
#define key_93C66_Adr0182		"k93C66A0182"	// Key for EEPROM Address 93C66 Address 0182
#define key_93C66_Adr0183		"k93C66A0183"	// Key for EEPROM Address 93C66 Address 0183
#define key_93C66_Adr0184		"k93C66A0184"	// Key for EEPROM Address 93C66 Address 0184
#define key_93C66_Adr0185		"k93C66A0185"	// Key for EEPROM Address 93C66 Address 0185
#define key_93C66_Adr0186		"k93C66A0186"	// Key for EEPROM Address 93C66 Address 0186
#define key_93C66_Adr0187		"k93C66A0187"	// Key for EEPROM Address 93C66 Address 0187
#define key_93C66_Adr0188		"k93C66A0188"	// Key for EEPROM Address 93C66 Address 0188
#define key_93C66_Adr0189		"k93C66A0189"	// Key for EEPROM Address 93C66 Address 0189
#define key_93C66_Adr0190		"k93C66A0190"	// Key for EEPROM Address 93C66 Address 0190
#define key_93C66_Adr0191		"k93C66A0191"	// Key for EEPROM Address 93C66 Address 0191
#define key_93C66_Adr0192		"k93C66A0192"	// Key for EEPROM Address 93C66 Address 0192
#define key_93C66_Adr0193		"k93C66A0193"	// Key for EEPROM Address 93C66 Address 0193
#define key_93C66_Adr0194		"k93C66A0194"	// Key for EEPROM Address 93C66 Address 0194
#define key_93C66_Adr0195		"k93C66A0195"	// Key for EEPROM Address 93C66 Address 0195
#define key_93C66_Adr0196		"k93C66A0196"	// Key for EEPROM Address 93C66 Address 0196
#define key_93C66_Adr0197		"k93C66A0197"	// Key for EEPROM Address 93C66 Address 0197
#define key_93C66_Adr0198		"k93C66A0198"	// Key for EEPROM Address 93C66 Address 0198
#define key_93C66_Adr0199		"k93C66A0199"	// Key for EEPROM Address 93C66 Address 0199
#define key_93C66_Adr0200		"k93C66A0200"	// Key for EEPROM Address 93C66 Address 0200
#define key_93C66_Adr0201		"k93C66A0201"	// Key for EEPROM Address 93C66 Address 0201
#define key_93C66_Adr0202		"k93C66A0202"	// Key for EEPROM Address 93C66 Address 0202
#define key_93C66_Adr0203		"k93C66A0203"	// Key for EEPROM Address 93C66 Address 0203
#define key_93C66_Adr0204		"k93C66A0204"	// Key for EEPROM Address 93C66 Address 0204
#define key_93C66_Adr0205		"k93C66A0205"	// Key for EEPROM Address 93C66 Address 0205
#define key_93C66_Adr0206		"k93C66A0206"	// Key for EEPROM Address 93C66 Address 0206
#define key_93C66_Adr0207		"k93C66A0207"	// Key for EEPROM Address 93C66 Address 0207
#define key_93C66_Adr0208		"k93C66A0208"	// Key for EEPROM Address 93C66 Address 0208
#define key_93C66_Adr0209		"k93C66A0209"	// Key for EEPROM Address 93C66 Address 0209
#define key_93C66_Adr0210		"k93C66A0210"	// Key for EEPROM Address 93C66 Address 0210
#define key_93C66_Adr0211		"k93C66A0211"	// Key for EEPROM Address 93C66 Address 0211
#define key_93C66_Adr0212		"k93C66A0212"	// Key for EEPROM Address 93C66 Address 0212
#define key_93C66_Adr0213		"k93C66A0213"	// Key for EEPROM Address 93C66 Address 0213
#define key_93C66_Adr0214		"k93C66A0214"	// Key for EEPROM Address 93C66 Address 0214
#define key_93C66_Adr0215		"k93C66A0215"	// Key for EEPROM Address 93C66 Address 0215
#define key_93C66_Adr0216		"k93C66A0216"	// Key for EEPROM Address 93C66 Address 0216
#define key_93C66_Adr0217		"k93C66A0217"	// Key for EEPROM Address 93C66 Address 0217
#define key_93C66_Adr0218		"k93C66A0218"	// Key for EEPROM Address 93C66 Address 0218
#define key_93C66_Adr0219		"k93C66A0219"	// Key for EEPROM Address 93C66 Address 0219
#define key_93C66_Adr0220		"k93C66A0220"	// Key for EEPROM Address 93C66 Address 0220
#define key_93C66_Adr0221		"k93C66A0221"	// Key for EEPROM Address 93C66 Address 0221
#define key_93C66_Adr0222		"k93C66A0222"	// Key for EEPROM Address 93C66 Address 0222
#define key_93C66_Adr0223		"k93C66A0223"	// Key for EEPROM Address 93C66 Address 0223
#define key_93C66_Adr0224		"k93C66A0224"	// Key for EEPROM Address 93C66 Address 0224
#define key_93C66_Adr0225		"k93C66A0225"	// Key for EEPROM Address 93C66 Address 0225
#define key_93C66_Adr0226		"k93C66A0226"	// Key for EEPROM Address 93C66 Address 0226
#define key_93C66_Adr0227		"k93C66A0227"	// Key for EEPROM Address 93C66 Address 0227
#define key_93C66_Adr0228		"k93C66A0228"	// Key for EEPROM Address 93C66 Address 0228
#define key_93C66_Adr0229		"k93C66A0229"	// Key for EEPROM Address 93C66 Address 0229
#define key_93C66_Adr0230		"k93C66A0230"	// Key for EEPROM Address 93C66 Address 0230
#define key_93C66_Adr0231		"k93C66A0231"	// Key for EEPROM Address 93C66 Address 0231
#define key_93C66_Adr0232		"k93C66A0232"	// Key for EEPROM Address 93C66 Address 0232
#define key_93C66_Adr0233		"k93C66A0233"	// Key for EEPROM Address 93C66 Address 0233
#define key_93C66_Adr0234		"k93C66A0234"	// Key for EEPROM Address 93C66 Address 0234
#define key_93C66_Adr0235		"k93C66A0235"	// Key for EEPROM Address 93C66 Address 0235
#define key_93C66_Adr0236		"k93C66A0236"	// Key for EEPROM Address 93C66 Address 0236
#define key_93C66_Adr0237		"k93C66A0237"	// Key for EEPROM Address 93C66 Address 0237
#define key_93C66_Adr0238		"k93C66A0238"	// Key for EEPROM Address 93C66 Address 0238
#define key_93C66_Adr0239		"k93C66A0239"	// Key for EEPROM Address 93C66 Address 0239
#define key_93C66_Adr0240		"k93C66A0240"	// Key for EEPROM Address 93C66 Address 0240
#define key_93C66_Adr0241		"k93C66A0241"	// Key for EEPROM Address 93C66 Address 0241
#define key_93C66_Adr0242		"k93C66A0242"	// Key for EEPROM Address 93C66 Address 0242
#define key_93C66_Adr0243		"k93C66A0243"	// Key for EEPROM Address 93C66 Address 0243
#define key_93C66_Adr0244		"k93C66A0244"	// Key for EEPROM Address 93C66 Address 0244
#define key_93C66_Adr0245		"k93C66A0245"	// Key for EEPROM Address 93C66 Address 0245
#define key_93C66_Adr0246		"k93C66A0246"	// Key for EEPROM Address 93C66 Address 0246
#define key_93C66_Adr0247		"k93C66A0247"	// Key for EEPROM Address 93C66 Address 0247
#define key_93C66_Adr0248		"k93C66A0248"	// Key for EEPROM Address 93C66 Address 0248
#define key_93C66_Adr0249		"k93C66A0249"	// Key for EEPROM Address 93C66 Address 0249
#define key_93C66_Adr0250		"k93C66A0250"	// Key for EEPROM Address 93C66 Address 0250
#define key_93C66_Adr0251		"k93C66A0251"	// Key for EEPROM Address 93C66 Address 0251
#define key_93C66_Adr0252		"k93C66A0252"	// Key for EEPROM Address 93C66 Address 0252
#define key_93C66_Adr0253		"k93C66A0253"	// Key for EEPROM Address 93C66 Address 0253
#define key_93C66_Adr0254		"k93C66A0254"	// Key for EEPROM Address 93C66 Address 0254
#define key_93C66_Adr0255		"k93C66A0255"	// Key for EEPROM Address 93C66 Address 0255
#define key_93C66_Adr0256		"k93C66A0256"	// Key for EEPROM Address 93C66 Address 0256
#define key_93C66_Adr0257		"k93C66A0257"	// Key for EEPROM Address 93C66 Address 0257
#define key_93C66_Adr0258		"k93C66A0258"	// Key for EEPROM Address 93C66 Address 0258
#define key_93C66_Adr0259		"k93C66A0259"	// Key for EEPROM Address 93C66 Address 0259
#define key_93C66_Adr0260		"k93C66A0260"	// Key for EEPROM Address 93C66 Address 0260
#define key_93C66_Adr0261		"k93C66A0261"	// Key for EEPROM Address 93C66 Address 0261
#define key_93C66_Adr0262		"k93C66A0262"	// Key for EEPROM Address 93C66 Address 0262
#define key_93C66_Adr0263		"k93C66A0263"	// Key for EEPROM Address 93C66 Address 0263
#define key_93C66_Adr0264		"k93C66A0264"	// Key for EEPROM Address 93C66 Address 0264
#define key_93C66_Adr0265		"k93C66A0265"	// Key for EEPROM Address 93C66 Address 0265
#define key_93C66_Adr0266		"k93C66A0266"	// Key for EEPROM Address 93C66 Address 0266
#define key_93C66_Adr0267		"k93C66A0267"	// Key for EEPROM Address 93C66 Address 0267
#define key_93C66_Adr0268		"k93C66A0268"	// Key for EEPROM Address 93C66 Address 0268
#define key_93C66_Adr0269		"k93C66A0269"	// Key for EEPROM Address 93C66 Address 0269
#define key_93C66_Adr0270		"k93C66A0270"	// Key for EEPROM Address 93C66 Address 0270
#define key_93C66_Adr0271		"k93C66A0271"	// Key for EEPROM Address 93C66 Address 0271
#define key_93C66_Adr0272		"k93C66A0272"	// Key for EEPROM Address 93C66 Address 0272
#define key_93C66_Adr0273		"k93C66A0273"	// Key for EEPROM Address 93C66 Address 0273
#define key_93C66_Adr0274		"k93C66A0274"	// Key for EEPROM Address 93C66 Address 0274
#define key_93C66_Adr0275		"k93C66A0275"	// Key for EEPROM Address 93C66 Address 0275
#define key_93C66_Adr0276		"k93C66A0276"	// Key for EEPROM Address 93C66 Address 0276
#define key_93C66_Adr0277		"k93C66A0277"	// Key for EEPROM Address 93C66 Address 0277
#define key_93C66_Adr0278		"k93C66A0278"	// Key for EEPROM Address 93C66 Address 0278
#define key_93C66_Adr0279		"k93C66A0279"	// Key for EEPROM Address 93C66 Address 0279
#define key_93C66_Adr0280		"k93C66A0280"	// Key for EEPROM Address 93C66 Address 0280
#define key_93C66_Adr0281		"k93C66A0281"	// Key for EEPROM Address 93C66 Address 0281
#define key_93C66_Adr0282		"k93C66A0282"	// Key for EEPROM Address 93C66 Address 0282
#define key_93C66_Adr0283		"k93C66A0283"	// Key for EEPROM Address 93C66 Address 0283
#define key_93C66_Adr0284		"k93C66A0284"	// Key for EEPROM Address 93C66 Address 0284
#define key_93C66_Adr0285		"k93C66A0285"	// Key for EEPROM Address 93C66 Address 0285
#define key_93C66_Adr0286		"k93C66A0286"	// Key for EEPROM Address 93C66 Address 0286
#define key_93C66_Adr0287		"k93C66A0287"	// Key for EEPROM Address 93C66 Address 0287
#define key_93C66_Adr0288		"k93C66A0288"	// Key for EEPROM Address 93C66 Address 0288
#define key_93C66_Adr0289		"k93C66A0289"	// Key for EEPROM Address 93C66 Address 0289
#define key_93C66_Adr0290		"k93C66A0290"	// Key for EEPROM Address 93C66 Address 0290
#define key_93C66_Adr0291		"k93C66A0291"	// Key for EEPROM Address 93C66 Address 0291
#define key_93C66_Adr0292		"k93C66A0292"	// Key for EEPROM Address 93C66 Address 0292
#define key_93C66_Adr0293		"k93C66A0293"	// Key for EEPROM Address 93C66 Address 0293
#define key_93C66_Adr0294		"k93C66A0294"	// Key for EEPROM Address 93C66 Address 0294
#define key_93C66_Adr0295		"k93C66A0295"	// Key for EEPROM Address 93C66 Address 0295
#define key_93C66_Adr0296		"k93C66A0296"	// Key for EEPROM Address 93C66 Address 0296
#define key_93C66_Adr0297		"k93C66A0297"	// Key for EEPROM Address 93C66 Address 0297
#define key_93C66_Adr0298		"k93C66A0298"	// Key for EEPROM Address 93C66 Address 0298
#define key_93C66_Adr0299		"k93C66A0299"	// Key for EEPROM Address 93C66 Address 0299
#define key_93C66_Adr0300		"k93C66A0300"	// Key for EEPROM Address 93C66 Address 0300
#define key_93C66_Adr0301		"k93C66A0301"	// Key for EEPROM Address 93C66 Address 0301
#define key_93C66_Adr0302		"k93C66A0302"	// Key for EEPROM Address 93C66 Address 0302
#define key_93C66_Adr0303		"k93C66A0303"	// Key for EEPROM Address 93C66 Address 0303
#define key_93C66_Adr0304		"k93C66A0304"	// Key for EEPROM Address 93C66 Address 0304
#define key_93C66_Adr0305		"k93C66A0305"	// Key for EEPROM Address 93C66 Address 0305
#define key_93C66_Adr0306		"k93C66A0306"	// Key for EEPROM Address 93C66 Address 0306
#define key_93C66_Adr0307		"k93C66A0307"	// Key for EEPROM Address 93C66 Address 0307
#define key_93C66_Adr0308		"k93C66A0308"	// Key for EEPROM Address 93C66 Address 0308
#define key_93C66_Adr0309		"k93C66A0309"	// Key for EEPROM Address 93C66 Address 0309
#define key_93C66_Adr0310		"k93C66A0310"	// Key for EEPROM Address 93C66 Address 0310
#define key_93C66_Adr0311		"k93C66A0311"	// Key for EEPROM Address 93C66 Address 0311
#define key_93C66_Adr0312		"k93C66A0312"	// Key for EEPROM Address 93C66 Address 0312
#define key_93C66_Adr0313		"k93C66A0313"	// Key for EEPROM Address 93C66 Address 0313
#define key_93C66_Adr0314		"k93C66A0314"	// Key for EEPROM Address 93C66 Address 0314
#define key_93C66_Adr0315		"k93C66A0315"	// Key for EEPROM Address 93C66 Address 0315
#define key_93C66_Adr0316		"k93C66A0316"	// Key for EEPROM Address 93C66 Address 0316
#define key_93C66_Adr0317		"k93C66A0317"	// Key for EEPROM Address 93C66 Address 0317
#define key_93C66_Adr0318		"k93C66A0318"	// Key for EEPROM Address 93C66 Address 0318
#define key_93C66_Adr0319		"k93C66A0319"	// Key for EEPROM Address 93C66 Address 0319
#define key_93C66_Adr0320		"k93C66A0320"	// Key for EEPROM Address 93C66 Address 0320
#define key_93C66_Adr0321		"k93C66A0321"	// Key for EEPROM Address 93C66 Address 0321
#define key_93C66_Adr0322		"k93C66A0322"	// Key for EEPROM Address 93C66 Address 0322
#define key_93C66_Adr0323		"k93C66A0323"	// Key for EEPROM Address 93C66 Address 0323
#define key_93C66_Adr0324		"k93C66A0324"	// Key for EEPROM Address 93C66 Address 0324
#define key_93C66_Adr0325		"k93C66A0325"	// Key for EEPROM Address 93C66 Address 0325
#define key_93C66_Adr0326		"k93C66A0326"	// Key for EEPROM Address 93C66 Address 0326
#define key_93C66_Adr0327		"k93C66A0327"	// Key for EEPROM Address 93C66 Address 0327
#define key_93C66_Adr0328		"k93C66A0328"	// Key for EEPROM Address 93C66 Address 0328
#define key_93C66_Adr0329		"k93C66A0329"	// Key for EEPROM Address 93C66 Address 0329
#define key_93C66_Adr0330		"k93C66A0330"	// Key for EEPROM Address 93C66 Address 0330
#define key_93C66_Adr0331		"k93C66A0331"	// Key for EEPROM Address 93C66 Address 0331
#define key_93C66_Adr0332		"k93C66A0332"	// Key for EEPROM Address 93C66 Address 0332
#define key_93C66_Adr0333		"k93C66A0333"	// Key for EEPROM Address 93C66 Address 0333
#define key_93C66_Adr0334		"k93C66A0334"	// Key for EEPROM Address 93C66 Address 0334
#define key_93C66_Adr0335		"k93C66A0335"	// Key for EEPROM Address 93C66 Address 0335
#define key_93C66_Adr0336		"k93C66A0336"	// Key for EEPROM Address 93C66 Address 0336
#define key_93C66_Adr0337		"k93C66A0337"	// Key for EEPROM Address 93C66 Address 0337
#define key_93C66_Adr0338		"k93C66A0338"	// Key for EEPROM Address 93C66 Address 0338
#define key_93C66_Adr0339		"k93C66A0339"	// Key for EEPROM Address 93C66 Address 0339
#define key_93C66_Adr0340		"k93C66A0340"	// Key for EEPROM Address 93C66 Address 0340
#define key_93C66_Adr0341		"k93C66A0341"	// Key for EEPROM Address 93C66 Address 0341
#define key_93C66_Adr0342		"k93C66A0342"	// Key for EEPROM Address 93C66 Address 0342
#define key_93C66_Adr0343		"k93C66A0343"	// Key for EEPROM Address 93C66 Address 0343
#define key_93C66_Adr0344		"k93C66A0344"	// Key for EEPROM Address 93C66 Address 0344
#define key_93C66_Adr0345		"k93C66A0345"	// Key for EEPROM Address 93C66 Address 0345
#define key_93C66_Adr0346		"k93C66A0346"	// Key for EEPROM Address 93C66 Address 0346
#define key_93C66_Adr0347		"k93C66A0347"	// Key for EEPROM Address 93C66 Address 0347
#define key_93C66_Adr0348		"k93C66A0348"	// Key for EEPROM Address 93C66 Address 0348
#define key_93C66_Adr0349		"k93C66A0349"	// Key for EEPROM Address 93C66 Address 0349
#define key_93C66_Adr0350		"k93C66A0350"	// Key for EEPROM Address 93C66 Address 0350
#define key_93C66_Adr0351		"k93C66A0351"	// Key for EEPROM Address 93C66 Address 0351
#define key_93C66_Adr0352		"k93C66A0352"	// Key for EEPROM Address 93C66 Address 0352
#define key_93C66_Adr0353		"k93C66A0353"	// Key for EEPROM Address 93C66 Address 0353
#define key_93C66_Adr0354		"k93C66A0354"	// Key for EEPROM Address 93C66 Address 0354
#define key_93C66_Adr0355		"k93C66A0355"	// Key for EEPROM Address 93C66 Address 0355
#define key_93C66_Adr0356		"k93C66A0356"	// Key for EEPROM Address 93C66 Address 0356
#define key_93C66_Adr0357		"k93C66A0357"	// Key for EEPROM Address 93C66 Address 0357
#define key_93C66_Adr0358		"k93C66A0358"	// Key for EEPROM Address 93C66 Address 0358
#define key_93C66_Adr0359		"k93C66A0359"	// Key for EEPROM Address 93C66 Address 0359
#define key_93C66_Adr0360		"k93C66A0360"	// Key for EEPROM Address 93C66 Address 0360
#define key_93C66_Adr0361		"k93C66A0361"	// Key for EEPROM Address 93C66 Address 0361
#define key_93C66_Adr0362		"k93C66A0362"	// Key for EEPROM Address 93C66 Address 0362
#define key_93C66_Adr0363		"k93C66A0363"	// Key for EEPROM Address 93C66 Address 0363
#define key_93C66_Adr0364		"k93C66A0364"	// Key for EEPROM Address 93C66 Address 0364
#define key_93C66_Adr0365		"k93C66A0365"	// Key for EEPROM Address 93C66 Address 0365
#define key_93C66_Adr0366		"k93C66A0366"	// Key for EEPROM Address 93C66 Address 0366
#define key_93C66_Adr0367		"k93C66A0367"	// Key for EEPROM Address 93C66 Address 0367
#define key_93C66_Adr0368		"k93C66A0368"	// Key for EEPROM Address 93C66 Address 0368
#define key_93C66_Adr0369		"k93C66A0369"	// Key for EEPROM Address 93C66 Address 0369
#define key_93C66_Adr0370		"k93C66A0370"	// Key for EEPROM Address 93C66 Address 0370
#define key_93C66_Adr0371		"k93C66A0371"	// Key for EEPROM Address 93C66 Address 0371
#define key_93C66_Adr0372		"k93C66A0372"	// Key for EEPROM Address 93C66 Address 0372
#define key_93C66_Adr0373		"k93C66A0373"	// Key for EEPROM Address 93C66 Address 0373
#define key_93C66_Adr0374		"k93C66A0374"	// Key for EEPROM Address 93C66 Address 0374
#define key_93C66_Adr0375		"k93C66A0375"	// Key for EEPROM Address 93C66 Address 0375
#define key_93C66_Adr0376		"k93C66A0376"	// Key for EEPROM Address 93C66 Address 0376
#define key_93C66_Adr0377		"k93C66A0377"	// Key for EEPROM Address 93C66 Address 0377
#define key_93C66_Adr0378		"k93C66A0378"	// Key for EEPROM Address 93C66 Address 0378
#define key_93C66_Adr0379		"k93C66A0379"	// Key for EEPROM Address 93C66 Address 0379
#define key_93C66_Adr0380		"k93C66A0380"	// Key for EEPROM Address 93C66 Address 0380
#define key_93C66_Adr0381		"k93C66A0381"	// Key for EEPROM Address 93C66 Address 0381
#define key_93C66_Adr0382		"k93C66A0382"	// Key for EEPROM Address 93C66 Address 0382
#define key_93C66_Adr0383		"k93C66A0383"	// Key for EEPROM Address 93C66 Address 0383
#define key_93C66_Adr0384		"k93C66A0384"	// Key for EEPROM Address 93C66 Address 0384
#define key_93C66_Adr0385		"k93C66A0385"	// Key for EEPROM Address 93C66 Address 0385
#define key_93C66_Adr0386		"k93C66A0386"	// Key for EEPROM Address 93C66 Address 0386
#define key_93C66_Adr0387		"k93C66A0387"	// Key for EEPROM Address 93C66 Address 0387
#define key_93C66_Adr0388		"k93C66A0388"	// Key for EEPROM Address 93C66 Address 0388
#define key_93C66_Adr0389		"k93C66A0389"	// Key for EEPROM Address 93C66 Address 0389
#define key_93C66_Adr0390		"k93C66A0390"	// Key for EEPROM Address 93C66 Address 0390
#define key_93C66_Adr0391		"k93C66A0391"	// Key for EEPROM Address 93C66 Address 0391
#define key_93C66_Adr0392		"k93C66A0392"	// Key for EEPROM Address 93C66 Address 0392
#define key_93C66_Adr0393		"k93C66A0393"	// Key for EEPROM Address 93C66 Address 0393
#define key_93C66_Adr0394		"k93C66A0394"	// Key for EEPROM Address 93C66 Address 0394
#define key_93C66_Adr0395		"k93C66A0395"	// Key for EEPROM Address 93C66 Address 0395
#define key_93C66_Adr0396		"k93C66A0396"	// Key for EEPROM Address 93C66 Address 0396
#define key_93C66_Adr0397		"k93C66A0397"	// Key for EEPROM Address 93C66 Address 0397
#define key_93C66_Adr0398		"k93C66A0398"	// Key for EEPROM Address 93C66 Address 0398
#define key_93C66_Adr0399		"k93C66A0399"	// Key for EEPROM Address 93C66 Address 0399
#define key_93C66_Adr0400		"k93C66A0400"	// Key for EEPROM Address 93C66 Address 0400
#define key_93C66_Adr0401		"k93C66A0401"	// Key for EEPROM Address 93C66 Address 0401
#define key_93C66_Adr0402		"k93C66A0402"	// Key for EEPROM Address 93C66 Address 0402
#define key_93C66_Adr0403		"k93C66A0403"	// Key for EEPROM Address 93C66 Address 0403
#define key_93C66_Adr0404		"k93C66A0404"	// Key for EEPROM Address 93C66 Address 0404
#define key_93C66_Adr0405		"k93C66A0405"	// Key for EEPROM Address 93C66 Address 0405
#define key_93C66_Adr0406		"k93C66A0406"	// Key for EEPROM Address 93C66 Address 0406
#define key_93C66_Adr0407		"k93C66A0407"	// Key for EEPROM Address 93C66 Address 0407
#define key_93C66_Adr0408		"k93C66A0408"	// Key for EEPROM Address 93C66 Address 0408
#define key_93C66_Adr0409		"k93C66A0409"	// Key for EEPROM Address 93C66 Address 0409
#define key_93C66_Adr0410		"k93C66A0410"	// Key for EEPROM Address 93C66 Address 0410
#define key_93C66_Adr0411		"k93C66A0411"	// Key for EEPROM Address 93C66 Address 0411
#define key_93C66_Adr0412		"k93C66A0412"	// Key for EEPROM Address 93C66 Address 0412
#define key_93C66_Adr0413		"k93C66A0413"	// Key for EEPROM Address 93C66 Address 0413
#define key_93C66_Adr0414		"k93C66A0414"	// Key for EEPROM Address 93C66 Address 0414
#define key_93C66_Adr0415		"k93C66A0415"	// Key for EEPROM Address 93C66 Address 0415
#define key_93C66_Adr0416		"k93C66A0416"	// Key for EEPROM Address 93C66 Address 0416
#define key_93C66_Adr0417		"k93C66A0417"	// Key for EEPROM Address 93C66 Address 0417
#define key_93C66_Adr0418		"k93C66A0418"	// Key for EEPROM Address 93C66 Address 0418
#define key_93C66_Adr0419		"k93C66A0419"	// Key for EEPROM Address 93C66 Address 0419
#define key_93C66_Adr0420		"k93C66A0420"	// Key for EEPROM Address 93C66 Address 0420
#define key_93C66_Adr0421		"k93C66A0421"	// Key for EEPROM Address 93C66 Address 0421
#define key_93C66_Adr0422		"k93C66A0422"	// Key for EEPROM Address 93C66 Address 0422
#define key_93C66_Adr0423		"k93C66A0423"	// Key for EEPROM Address 93C66 Address 0423
#define key_93C66_Adr0424		"k93C66A0424"	// Key for EEPROM Address 93C66 Address 0424
#define key_93C66_Adr0425		"k93C66A0425"	// Key for EEPROM Address 93C66 Address 0425
#define key_93C66_Adr0426		"k93C66A0426"	// Key for EEPROM Address 93C66 Address 0426
#define key_93C66_Adr0427		"k93C66A0427"	// Key for EEPROM Address 93C66 Address 0427
#define key_93C66_Adr0428		"k93C66A0428"	// Key for EEPROM Address 93C66 Address 0428
#define key_93C66_Adr0429		"k93C66A0429"	// Key for EEPROM Address 93C66 Address 0429
#define key_93C66_Adr0430		"k93C66A0430"	// Key for EEPROM Address 93C66 Address 0430
#define key_93C66_Adr0431		"k93C66A0431"	// Key for EEPROM Address 93C66 Address 0431
#define key_93C66_Adr0432		"k93C66A0432"	// Key for EEPROM Address 93C66 Address 0432
#define key_93C66_Adr0433		"k93C66A0433"	// Key for EEPROM Address 93C66 Address 0433
#define key_93C66_Adr0434		"k93C66A0434"	// Key for EEPROM Address 93C66 Address 0434
#define key_93C66_Adr0435		"k93C66A0435"	// Key for EEPROM Address 93C66 Address 0435
#define key_93C66_Adr0436		"k93C66A0436"	// Key for EEPROM Address 93C66 Address 0436
#define key_93C66_Adr0437		"k93C66A0437"	// Key for EEPROM Address 93C66 Address 0437
#define key_93C66_Adr0438		"k93C66A0438"	// Key for EEPROM Address 93C66 Address 0438
#define key_93C66_Adr0439		"k93C66A0439"	// Key for EEPROM Address 93C66 Address 0439
#define key_93C66_Adr0440		"k93C66A0440"	// Key for EEPROM Address 93C66 Address 0440
#define key_93C66_Adr0441		"k93C66A0441"	// Key for EEPROM Address 93C66 Address 0441
#define key_93C66_Adr0442		"k93C66A0442"	// Key for EEPROM Address 93C66 Address 0442
#define key_93C66_Adr0443		"k93C66A0443"	// Key for EEPROM Address 93C66 Address 0443
#define key_93C66_Adr0444		"k93C66A0444"	// Key for EEPROM Address 93C66 Address 0444
#define key_93C66_Adr0445		"k93C66A0445"	// Key for EEPROM Address 93C66 Address 0445
#define key_93C66_Adr0446		"k93C66A0446"	// Key for EEPROM Address 93C66 Address 0446
#define key_93C66_Adr0447		"k93C66A0447"	// Key for EEPROM Address 93C66 Address 0447
#define key_93C66_Adr0448		"k93C66A0448"	// Key for EEPROM Address 93C66 Address 0448
#define key_93C66_Adr0449		"k93C66A0449"	// Key for EEPROM Address 93C66 Address 0449
#define key_93C66_Adr0450		"k93C66A0450"	// Key for EEPROM Address 93C66 Address 0450
#define key_93C66_Adr0451		"k93C66A0451"	// Key for EEPROM Address 93C66 Address 0451
#define key_93C66_Adr0452		"k93C66A0452"	// Key for EEPROM Address 93C66 Address 0452
#define key_93C66_Adr0453		"k93C66A0453"	// Key for EEPROM Address 93C66 Address 0453
#define key_93C66_Adr0454		"k93C66A0454"	// Key for EEPROM Address 93C66 Address 0454
#define key_93C66_Adr0455		"k93C66A0455"	// Key for EEPROM Address 93C66 Address 0455
#define key_93C66_Adr0456		"k93C66A0456"	// Key for EEPROM Address 93C66 Address 0456
#define key_93C66_Adr0457		"k93C66A0457"	// Key for EEPROM Address 93C66 Address 0457
#define key_93C66_Adr0458		"k93C66A0458"	// Key for EEPROM Address 93C66 Address 0458
#define key_93C66_Adr0459		"k93C66A0459"	// Key for EEPROM Address 93C66 Address 0459
#define key_93C66_Adr0460		"k93C66A0460"	// Key for EEPROM Address 93C66 Address 0460
#define key_93C66_Adr0461		"k93C66A0461"	// Key for EEPROM Address 93C66 Address 0461
#define key_93C66_Adr0462		"k93C66A0462"	// Key for EEPROM Address 93C66 Address 0462
#define key_93C66_Adr0463		"k93C66A0463"	// Key for EEPROM Address 93C66 Address 0463
#define key_93C66_Adr0464		"k93C66A0464"	// Key for EEPROM Address 93C66 Address 0464
#define key_93C66_Adr0465		"k93C66A0465"	// Key for EEPROM Address 93C66 Address 0465
#define key_93C66_Adr0466		"k93C66A0466"	// Key for EEPROM Address 93C66 Address 0466
#define key_93C66_Adr0467		"k93C66A0467"	// Key for EEPROM Address 93C66 Address 0467
#define key_93C66_Adr0468		"k93C66A0468"	// Key for EEPROM Address 93C66 Address 0468
#define key_93C66_Adr0469		"k93C66A0469"	// Key for EEPROM Address 93C66 Address 0469
#define key_93C66_Adr0470		"k93C66A0470"	// Key for EEPROM Address 93C66 Address 0470
#define key_93C66_Adr0471		"k93C66A0471"	// Key for EEPROM Address 93C66 Address 0471
#define key_93C66_Adr0472		"k93C66A0472"	// Key for EEPROM Address 93C66 Address 0472
#define key_93C66_Adr0473		"k93C66A0473"	// Key for EEPROM Address 93C66 Address 0473
#define key_93C66_Adr0474		"k93C66A0474"	// Key for EEPROM Address 93C66 Address 0474
#define key_93C66_Adr0475		"k93C66A0475"	// Key for EEPROM Address 93C66 Address 0475
#define key_93C66_Adr0476		"k93C66A0476"	// Key for EEPROM Address 93C66 Address 0476
#define key_93C66_Adr0477		"k93C66A0477"	// Key for EEPROM Address 93C66 Address 0477
#define key_93C66_Adr0478		"k93C66A0478"	// Key for EEPROM Address 93C66 Address 0478
#define key_93C66_Adr0479		"k93C66A0479"	// Key for EEPROM Address 93C66 Address 0479
#define key_93C66_Adr0480		"k93C66A0480"	// Key for EEPROM Address 93C66 Address 0480
#define key_93C66_Adr0481		"k93C66A0481"	// Key for EEPROM Address 93C66 Address 0481
#define key_93C66_Adr0482		"k93C66A0482"	// Key for EEPROM Address 93C66 Address 0482
#define key_93C66_Adr0483		"k93C66A0483"	// Key for EEPROM Address 93C66 Address 0483
#define key_93C66_Adr0484		"k93C66A0484"	// Key for EEPROM Address 93C66 Address 0484
#define key_93C66_Adr0485		"k93C66A0485"	// Key for EEPROM Address 93C66 Address 0485
#define key_93C66_Adr0486		"k93C66A0486"	// Key for EEPROM Address 93C66 Address 0486
#define key_93C66_Adr0487		"k93C66A0487"	// Key for EEPROM Address 93C66 Address 0487
#define key_93C66_Adr0488		"k93C66A0488"	// Key for EEPROM Address 93C66 Address 0488
#define key_93C66_Adr0489		"k93C66A0489"	// Key for EEPROM Address 93C66 Address 0489
#define key_93C66_Adr0490		"k93C66A0490"	// Key for EEPROM Address 93C66 Address 0490
#define key_93C66_Adr0491		"k93C66A0491"	// Key for EEPROM Address 93C66 Address 0491
#define key_93C66_Adr0492		"k93C66A0492"	// Key for EEPROM Address 93C66 Address 0492
#define key_93C66_Adr0493		"k93C66A0493"	// Key for EEPROM Address 93C66 Address 0493
#define key_93C66_Adr0494		"k93C66A0494"	// Key for EEPROM Address 93C66 Address 0494
#define key_93C66_Adr0495		"k93C66A0495"	// Key for EEPROM Address 93C66 Address 0495
#define key_93C66_Adr0496		"k93C66A0496"	// Key for EEPROM Address 93C66 Address 0496
#define key_93C66_Adr0497		"k93C66A0497"	// Key for EEPROM Address 93C66 Address 0497
#define key_93C66_Adr0498		"k93C66A0498"	// Key for EEPROM Address 93C66 Address 0498
#define key_93C66_Adr0499		"k93C66A0499"	// Key for EEPROM Address 93C66 Address 0499
#define key_93C66_Adr0500		"k93C66A0500"	// Key for EEPROM Address 93C66 Address 0500
#define key_93C66_Adr0501		"k93C66A0501"	// Key for EEPROM Address 93C66 Address 0501
#define key_93C66_Adr0502		"k93C66A0502"	// Key for EEPROM Address 93C66 Address 0502
#define key_93C66_Adr0503		"k93C66A0503"	// Key for EEPROM Address 93C66 Address 0503
#define key_93C66_Adr0504		"k93C66A0504"	// Key for EEPROM Address 93C66 Address 0504
#define key_93C66_Adr0505		"k93C66A0505"	// Key for EEPROM Address 93C66 Address 0505
#define key_93C66_Adr0506		"k93C66A0506"	// Key for EEPROM Address 93C66 Address 0506
#define key_93C66_Adr0507		"k93C66A0507"	// Key for EEPROM Address 93C66 Address 0507
#define key_93C66_Adr0508		"k93C66A0508"	// Key for EEPROM Address 93C66 Address 0508
#define key_93C66_Adr0509		"k93C66A0509"	// Key for EEPROM Address 93C66 Address 0509
#define key_93C66_Adr0510		"k93C66A0510"	// Key for EEPROM Address 93C66 Address 0510
#define key_93C66_Adr0511		"k93C66A0511"	// Key for EEPROM Address 93C66 Address 0511
#define key_93C66_Adr0512		"k93C66A0512"	// Key for EEPROM Address 93C66 Address 0512
#define key_93C66_Adr0513		"k93C66A0513"	// Key for EEPROM Address 93C66 Address 0513
#define key_93C66_Adr0514		"k93C66A0514"	// Key for EEPROM Address 93C66 Address 0514
#define key_93C66_Adr0515		"k93C66A0515"	// Key for EEPROM Address 93C66 Address 0515
#define key_93C66_Adr0516		"k93C66A0516"	// Key for EEPROM Address 93C66 Address 0516
#define key_93C66_Adr0517		"k93C66A0517"	// Key for EEPROM Address 93C66 Address 0517
#define key_93C66_Adr0518		"k93C66A0518"	// Key for EEPROM Address 93C66 Address 0518
#define key_93C66_Adr0519		"k93C66A0519"	// Key for EEPROM Address 93C66 Address 0519
#define key_93C66_Adr0520		"k93C66A0520"	// Key for EEPROM Address 93C66 Address 0520
#define key_93C66_Adr0521		"k93C66A0521"	// Key for EEPROM Address 93C66 Address 0521
#define key_93C66_Adr0522		"k93C66A0522"	// Key for EEPROM Address 93C66 Address 0522
#define key_93C66_Adr0523		"k93C66A0523"	// Key for EEPROM Address 93C66 Address 0523
#define key_93C66_Adr0524		"k93C66A0524"	// Key for EEPROM Address 93C66 Address 0524
#define key_93C66_Adr0525		"k93C66A0525"	// Key for EEPROM Address 93C66 Address 0525
#define key_93C66_Adr0526		"k93C66A0526"	// Key for EEPROM Address 93C66 Address 0526
#define key_93C66_Adr0527		"k93C66A0527"	// Key for EEPROM Address 93C66 Address 0527
#define key_93C66_Adr0528		"k93C66A0528"	// Key for EEPROM Address 93C66 Address 0528
#define key_93C66_Adr0529		"k93C66A0529"	// Key for EEPROM Address 93C66 Address 0529
#define key_93C66_Adr0530		"k93C66A0530"	// Key for EEPROM Address 93C66 Address 0530
#define key_93C66_Adr0531		"k93C66A0531"	// Key for EEPROM Address 93C66 Address 0531
#define key_93C66_Adr0532		"k93C66A0532"	// Key for EEPROM Address 93C66 Address 0532
#define key_93C66_Adr0533		"k93C66A0533"	// Key for EEPROM Address 93C66 Address 0533
#define key_93C66_Adr0534		"k93C66A0534"	// Key for EEPROM Address 93C66 Address 0534
#define key_93C66_Adr0535		"k93C66A0535"	// Key for EEPROM Address 93C66 Address 0535
#define key_93C66_Adr0536		"k93C66A0536"	// Key for EEPROM Address 93C66 Address 0536
#define key_93C66_Adr0537		"k93C66A0537"	// Key for EEPROM Address 93C66 Address 0537
#define key_93C66_Adr0538		"k93C66A0538"	// Key for EEPROM Address 93C66 Address 0538
#define key_93C66_Adr0539		"k93C66A0539"	// Key for EEPROM Address 93C66 Address 0539
#define key_93C66_Adr0540		"k93C66A0540"	// Key for EEPROM Address 93C66 Address 0540
#define key_93C66_Adr0541		"k93C66A0541"	// Key for EEPROM Address 93C66 Address 0541
#define key_93C66_Adr0542		"k93C66A0542"	// Key for EEPROM Address 93C66 Address 0542
#define key_93C66_Adr0543		"k93C66A0543"	// Key for EEPROM Address 93C66 Address 0543
#define key_93C66_Adr0544		"k93C66A0544"	// Key for EEPROM Address 93C66 Address 0544
#define key_93C66_Adr0545		"k93C66A0545"	// Key for EEPROM Address 93C66 Address 0545
#define key_93C66_Adr0546		"k93C66A0546"	// Key for EEPROM Address 93C66 Address 0546
#define key_93C66_Adr0547		"k93C66A0547"	// Key for EEPROM Address 93C66 Address 0547
#define key_93C66_Adr0548		"k93C66A0548"	// Key for EEPROM Address 93C66 Address 0548
#define key_93C66_Adr0549		"k93C66A0549"	// Key for EEPROM Address 93C66 Address 0549
#define key_93C66_Adr0550		"k93C66A0550"	// Key for EEPROM Address 93C66 Address 0550
#define key_93C66_Adr0551		"k93C66A0551"	// Key for EEPROM Address 93C66 Address 0551
#define key_93C66_Adr0552		"k93C66A0552"	// Key for EEPROM Address 93C66 Address 0552
#define key_93C66_Adr0553		"k93C66A0553"	// Key for EEPROM Address 93C66 Address 0553
#define key_93C66_Adr0554		"k93C66A0554"	// Key for EEPROM Address 93C66 Address 0554
#define key_93C66_Adr0555		"k93C66A0555"	// Key for EEPROM Address 93C66 Address 0555
#define key_93C66_Adr0556		"k93C66A0556"	// Key for EEPROM Address 93C66 Address 0556
#define key_93C66_Adr0557		"k93C66A0557"	// Key for EEPROM Address 93C66 Address 0557
#define key_93C66_Adr0558		"k93C66A0558"	// Key for EEPROM Address 93C66 Address 0558
#define key_93C66_Adr0559		"k93C66A0559"	// Key for EEPROM Address 93C66 Address 0559
#define key_93C66_Adr0560		"k93C66A0560"	// Key for EEPROM Address 93C66 Address 0560
#define key_93C66_Adr0561		"k93C66A0561"	// Key for EEPROM Address 93C66 Address 0561
#define key_93C66_Adr0562		"k93C66A0562"	// Key for EEPROM Address 93C66 Address 0562
#define key_93C66_Adr0563		"k93C66A0563"	// Key for EEPROM Address 93C66 Address 0563
#define key_93C66_Adr0564		"k93C66A0564"	// Key for EEPROM Address 93C66 Address 0564
#define key_93C66_Adr0565		"k93C66A0565"	// Key for EEPROM Address 93C66 Address 0565
#define key_93C66_Adr0566		"k93C66A0566"	// Key for EEPROM Address 93C66 Address 0566
#define key_93C66_Adr0567		"k93C66A0567"	// Key for EEPROM Address 93C66 Address 0567
#define key_93C66_Adr0568		"k93C66A0568"	// Key for EEPROM Address 93C66 Address 0568
#define key_93C66_Adr0569		"k93C66A0569"	// Key for EEPROM Address 93C66 Address 0569
#define key_93C66_Adr0570		"k93C66A0570"	// Key for EEPROM Address 93C66 Address 0570
#define key_93C66_Adr0571		"k93C66A0571"	// Key for EEPROM Address 93C66 Address 0571
#define key_93C66_Adr0572		"k93C66A0572"	// Key for EEPROM Address 93C66 Address 0572
#define key_93C66_Adr0573		"k93C66A0573"	// Key for EEPROM Address 93C66 Address 0573
#define key_93C66_Adr0574		"k93C66A0574"	// Key for EEPROM Address 93C66 Address 0574
#define key_93C66_Adr0575		"k93C66A0575"	// Key for EEPROM Address 93C66 Address 0575
#define key_93C66_Adr0576		"k93C66A0576"	// Key for EEPROM Address 93C66 Address 0576
#define key_93C66_Adr0577		"k93C66A0577"	// Key for EEPROM Address 93C66 Address 0577
#define key_93C66_Adr0578		"k93C66A0578"	// Key for EEPROM Address 93C66 Address 0578
#define key_93C66_Adr0579		"k93C66A0579"	// Key for EEPROM Address 93C66 Address 0579
#define key_93C66_Adr0580		"k93C66A0580"	// Key for EEPROM Address 93C66 Address 0580
#define key_93C66_Adr0581		"k93C66A0581"	// Key for EEPROM Address 93C66 Address 0581
#define key_93C66_Adr0582		"k93C66A0582"	// Key for EEPROM Address 93C66 Address 0582
#define key_93C66_Adr0583		"k93C66A0583"	// Key for EEPROM Address 93C66 Address 0583
#define key_93C66_Adr0584		"k93C66A0584"	// Key for EEPROM Address 93C66 Address 0584
#define key_93C66_Adr0585		"k93C66A0585"	// Key for EEPROM Address 93C66 Address 0585
#define key_93C66_Adr0586		"k93C66A0586"	// Key for EEPROM Address 93C66 Address 0586
#define key_93C66_Adr0587		"k93C66A0587"	// Key for EEPROM Address 93C66 Address 0587
#define key_93C66_Adr0588		"k93C66A0588"	// Key for EEPROM Address 93C66 Address 0588
#define key_93C66_Adr0589		"k93C66A0589"	// Key for EEPROM Address 93C66 Address 0589
#define key_93C66_Adr0590		"k93C66A0590"	// Key for EEPROM Address 93C66 Address 0590
#define key_93C66_Adr0591		"k93C66A0591"	// Key for EEPROM Address 93C66 Address 0591
#define key_93C66_Adr0592		"k93C66A0592"	// Key for EEPROM Address 93C66 Address 0592
#define key_93C66_Adr0593		"k93C66A0593"	// Key for EEPROM Address 93C66 Address 0593
#define key_93C66_Adr0594		"k93C66A0594"	// Key for EEPROM Address 93C66 Address 0594
#define key_93C66_Adr0595		"k93C66A0595"	// Key for EEPROM Address 93C66 Address 0595
#define key_93C66_Adr0596		"k93C66A0596"	// Key for EEPROM Address 93C66 Address 0596
#define key_93C66_Adr0597		"k93C66A0597"	// Key for EEPROM Address 93C66 Address 0597
#define key_93C66_Adr0598		"k93C66A0598"	// Key for EEPROM Address 93C66 Address 0598
#define key_93C66_Adr0599		"k93C66A0599"	// Key for EEPROM Address 93C66 Address 0599
#define key_93C66_Adr0600		"k93C66A0600"	// Key for EEPROM Address 93C66 Address 0600
#define key_93C66_Adr0601		"k93C66A0601"	// Key for EEPROM Address 93C66 Address 0601
#define key_93C66_Adr0602		"k93C66A0602"	// Key for EEPROM Address 93C66 Address 0602
#define key_93C66_Adr0603		"k93C66A0603"	// Key for EEPROM Address 93C66 Address 0603
#define key_93C66_Adr0604		"k93C66A0604"	// Key for EEPROM Address 93C66 Address 0604
#define key_93C66_Adr0605		"k93C66A0605"	// Key for EEPROM Address 93C66 Address 0605
#define key_93C66_Adr0606		"k93C66A0606"	// Key for EEPROM Address 93C66 Address 0606
#define key_93C66_Adr0607		"k93C66A0607"	// Key for EEPROM Address 93C66 Address 0607
#define key_93C66_Adr0608		"k93C66A0608"	// Key for EEPROM Address 93C66 Address 0608
#define key_93C66_Adr0609		"k93C66A0609"	// Key for EEPROM Address 93C66 Address 0609
#define key_93C66_Adr0610		"k93C66A0610"	// Key for EEPROM Address 93C66 Address 0610
#define key_93C66_Adr0611		"k93C66A0611"	// Key for EEPROM Address 93C66 Address 0611
#define key_93C66_Adr0612		"k93C66A0612"	// Key for EEPROM Address 93C66 Address 0612
#define key_93C66_Adr0613		"k93C66A0613"	// Key for EEPROM Address 93C66 Address 0613
#define key_93C66_Adr0614		"k93C66A0614"	// Key for EEPROM Address 93C66 Address 0614
#define key_93C66_Adr0615		"k93C66A0615"	// Key for EEPROM Address 93C66 Address 0615
#define key_93C66_Adr0616		"k93C66A0616"	// Key for EEPROM Address 93C66 Address 0616
#define key_93C66_Adr0617		"k93C66A0617"	// Key for EEPROM Address 93C66 Address 0617
#define key_93C66_Adr0618		"k93C66A0618"	// Key for EEPROM Address 93C66 Address 0618
#define key_93C66_Adr0619		"k93C66A0619"	// Key for EEPROM Address 93C66 Address 0619
#define key_93C66_Adr0620		"k93C66A0620"	// Key for EEPROM Address 93C66 Address 0620
#define key_93C66_Adr0621		"k93C66A0621"	// Key for EEPROM Address 93C66 Address 0621
#define key_93C66_Adr0622		"k93C66A0622"	// Key for EEPROM Address 93C66 Address 0622
#define key_93C66_Adr0623		"k93C66A0623"	// Key for EEPROM Address 93C66 Address 0623
#define key_93C66_Adr0624		"k93C66A0624"	// Key for EEPROM Address 93C66 Address 0624
#define key_93C66_Adr0625		"k93C66A0625"	// Key for EEPROM Address 93C66 Address 0625
#define key_93C66_Adr0626		"k93C66A0626"	// Key for EEPROM Address 93C66 Address 0626
#define key_93C66_Adr0627		"k93C66A0627"	// Key for EEPROM Address 93C66 Address 0627
#define key_93C66_Adr0628		"k93C66A0628"	// Key for EEPROM Address 93C66 Address 0628
#define key_93C66_Adr0629		"k93C66A0629"	// Key for EEPROM Address 93C66 Address 0629
#define key_93C66_Adr0630		"k93C66A0630"	// Key for EEPROM Address 93C66 Address 0630
#define key_93C66_Adr0631		"k93C66A0631"	// Key for EEPROM Address 93C66 Address 0631
#define key_93C66_Adr0632		"k93C66A0632"	// Key for EEPROM Address 93C66 Address 0632
#define key_93C66_Adr0633		"k93C66A0633"	// Key for EEPROM Address 93C66 Address 0633
#define key_93C66_Adr0634		"k93C66A0634"	// Key for EEPROM Address 93C66 Address 0634
#define key_93C66_Adr0635		"k93C66A0635"	// Key for EEPROM Address 93C66 Address 0635
#define key_93C66_Adr0636		"k93C66A0636"	// Key for EEPROM Address 93C66 Address 0636
#define key_93C66_Adr0637		"k93C66A0637"	// Key for EEPROM Address 93C66 Address 0637
#define key_93C66_Adr0638		"k93C66A0638"	// Key for EEPROM Address 93C66 Address 0638
#define key_93C66_Adr0639		"k93C66A0639"	// Key for EEPROM Address 93C66 Address 0639
#define key_93C66_Adr0640		"k93C66A0640"	// Key for EEPROM Address 93C66 Address 0640
#define key_93C66_Adr0641		"k93C66A0641"	// Key for EEPROM Address 93C66 Address 0641
#define key_93C66_Adr0642		"k93C66A0642"	// Key for EEPROM Address 93C66 Address 0642
#define key_93C66_Adr0643		"k93C66A0643"	// Key for EEPROM Address 93C66 Address 0643
#define key_93C66_Adr0644		"k93C66A0644"	// Key for EEPROM Address 93C66 Address 0644
#define key_93C66_Adr0645		"k93C66A0645"	// Key for EEPROM Address 93C66 Address 0645
#define key_93C66_Adr0646		"k93C66A0646"	// Key for EEPROM Address 93C66 Address 0646
#define key_93C66_Adr0647		"k93C66A0647"	// Key for EEPROM Address 93C66 Address 0647
#define key_93C66_Adr0648		"k93C66A0648"	// Key for EEPROM Address 93C66 Address 0648
#define key_93C66_Adr0649		"k93C66A0649"	// Key for EEPROM Address 93C66 Address 0649
#define key_93C66_Adr0650		"k93C66A0650"	// Key for EEPROM Address 93C66 Address 0650
#define key_93C66_Adr0651		"k93C66A0651"	// Key for EEPROM Address 93C66 Address 0651
#define key_93C66_Adr0652		"k93C66A0652"	// Key for EEPROM Address 93C66 Address 0652
#define key_93C66_Adr0653		"k93C66A0653"	// Key for EEPROM Address 93C66 Address 0653
#define key_93C66_Adr0654		"k93C66A0654"	// Key for EEPROM Address 93C66 Address 0654
#define key_93C66_Adr0655		"k93C66A0655"	// Key for EEPROM Address 93C66 Address 0655
#define key_93C66_Adr0656		"k93C66A0656"	// Key for EEPROM Address 93C66 Address 0656
#define key_93C66_Adr0657		"k93C66A0657"	// Key for EEPROM Address 93C66 Address 0657
#define key_93C66_Adr0658		"k93C66A0658"	// Key for EEPROM Address 93C66 Address 0658
#define key_93C66_Adr0659		"k93C66A0659"	// Key for EEPROM Address 93C66 Address 0659
#define key_93C66_Adr0660		"k93C66A0660"	// Key for EEPROM Address 93C66 Address 0660
#define key_93C66_Adr0661		"k93C66A0661"	// Key for EEPROM Address 93C66 Address 0661
#define key_93C66_Adr0662		"k93C66A0662"	// Key for EEPROM Address 93C66 Address 0662
#define key_93C66_Adr0663		"k93C66A0663"	// Key for EEPROM Address 93C66 Address 0663
#define key_93C66_Adr0664		"k93C66A0664"	// Key for EEPROM Address 93C66 Address 0664
#define key_93C66_Adr0665		"k93C66A0665"	// Key for EEPROM Address 93C66 Address 0665
#define key_93C66_Adr0666		"k93C66A0666"	// Key for EEPROM Address 93C66 Address 0666
#define key_93C66_Adr0667		"k93C66A0667"	// Key for EEPROM Address 93C66 Address 0667
#define key_93C66_Adr0668		"k93C66A0668"	// Key for EEPROM Address 93C66 Address 0668
#define key_93C66_Adr0669		"k93C66A0669"	// Key for EEPROM Address 93C66 Address 0669
#define key_93C66_Adr0670		"k93C66A0670"	// Key for EEPROM Address 93C66 Address 0670
#define key_93C66_Adr0671		"k93C66A0671"	// Key for EEPROM Address 93C66 Address 0671
#define key_93C66_Adr0672		"k93C66A0672"	// Key for EEPROM Address 93C66 Address 0672
#define key_93C66_Adr0673		"k93C66A0673"	// Key for EEPROM Address 93C66 Address 0673
#define key_93C66_Adr0674		"k93C66A0674"	// Key for EEPROM Address 93C66 Address 0674
#define key_93C66_Adr0675		"k93C66A0675"	// Key for EEPROM Address 93C66 Address 0675
#define key_93C66_Adr0676		"k93C66A0676"	// Key for EEPROM Address 93C66 Address 0676
#define key_93C66_Adr0677		"k93C66A0677"	// Key for EEPROM Address 93C66 Address 0677
#define key_93C66_Adr0678		"k93C66A0678"	// Key for EEPROM Address 93C66 Address 0678
#define key_93C66_Adr0679		"k93C66A0679"	// Key for EEPROM Address 93C66 Address 0679
#define key_93C66_Adr0680		"k93C66A0680"	// Key for EEPROM Address 93C66 Address 0680
#define key_93C66_Adr0681		"k93C66A0681"	// Key for EEPROM Address 93C66 Address 0681
#define key_93C66_Adr0682		"k93C66A0682"	// Key for EEPROM Address 93C66 Address 0682
#define key_93C66_Adr0683		"k93C66A0683"	// Key for EEPROM Address 93C66 Address 0683
#define key_93C66_Adr0684		"k93C66A0684"	// Key for EEPROM Address 93C66 Address 0684
#define key_93C66_Adr0685		"k93C66A0685"	// Key for EEPROM Address 93C66 Address 0685
#define key_93C66_Adr0686		"k93C66A0686"	// Key for EEPROM Address 93C66 Address 0686
#define key_93C66_Adr0687		"k93C66A0687"	// Key for EEPROM Address 93C66 Address 0687
#define key_93C66_Adr0688		"k93C66A0688"	// Key for EEPROM Address 93C66 Address 0688
#define key_93C66_Adr0689		"k93C66A0689"	// Key for EEPROM Address 93C66 Address 0689
#define key_93C66_Adr0690		"k93C66A0690"	// Key for EEPROM Address 93C66 Address 0690
#define key_93C66_Adr0691		"k93C66A0691"	// Key for EEPROM Address 93C66 Address 0691
#define key_93C66_Adr0692		"k93C66A0692"	// Key for EEPROM Address 93C66 Address 0692
#define key_93C66_Adr0693		"k93C66A0693"	// Key for EEPROM Address 93C66 Address 0693
#define key_93C66_Adr0694		"k93C66A0694"	// Key for EEPROM Address 93C66 Address 0694
#define key_93C66_Adr0695		"k93C66A0695"	// Key for EEPROM Address 93C66 Address 0695
#define key_93C66_Adr0696		"k93C66A0696"	// Key for EEPROM Address 93C66 Address 0696
#define key_93C66_Adr0697		"k93C66A0697"	// Key for EEPROM Address 93C66 Address 0697
#define key_93C66_Adr0698		"k93C66A0698"	// Key for EEPROM Address 93C66 Address 0698
#define key_93C66_Adr0699		"k93C66A0699"	// Key for EEPROM Address 93C66 Address 0699
#define key_93C66_Adr0700		"k93C66A0700"	// Key for EEPROM Address 93C66 Address 0700
#define key_93C66_Adr0701		"k93C66A0701"	// Key for EEPROM Address 93C66 Address 0701
#define key_93C66_Adr0702		"k93C66A0702"	// Key for EEPROM Address 93C66 Address 0702
#define key_93C66_Adr0703		"k93C66A0703"	// Key for EEPROM Address 93C66 Address 0703
#define key_93C66_Adr0704		"k93C66A0704"	// Key for EEPROM Address 93C66 Address 0704
#define key_93C66_Adr0705		"k93C66A0705"	// Key for EEPROM Address 93C66 Address 0705
#define key_93C66_Adr0706		"k93C66A0706"	// Key for EEPROM Address 93C66 Address 0706
#define key_93C66_Adr0707		"k93C66A0707"	// Key for EEPROM Address 93C66 Address 0707
#define key_93C66_Adr0708		"k93C66A0708"	// Key for EEPROM Address 93C66 Address 0708
#define key_93C66_Adr0709		"k93C66A0709"	// Key for EEPROM Address 93C66 Address 0709
#define key_93C66_Adr0710		"k93C66A0710"	// Key for EEPROM Address 93C66 Address 0710
#define key_93C66_Adr0711		"k93C66A0711"	// Key for EEPROM Address 93C66 Address 0711
#define key_93C66_Adr0712		"k93C66A0712"	// Key for EEPROM Address 93C66 Address 0712
#define key_93C66_Adr0713		"k93C66A0713"	// Key for EEPROM Address 93C66 Address 0713
#define key_93C66_Adr0714		"k93C66A0714"	// Key for EEPROM Address 93C66 Address 0714
#define key_93C66_Adr0715		"k93C66A0715"	// Key for EEPROM Address 93C66 Address 0715
#define key_93C66_Adr0716		"k93C66A0716"	// Key for EEPROM Address 93C66 Address 0716
#define key_93C66_Adr0717		"k93C66A0717"	// Key for EEPROM Address 93C66 Address 0717
#define key_93C66_Adr0718		"k93C66A0718"	// Key for EEPROM Address 93C66 Address 0718
#define key_93C66_Adr0719		"k93C66A0719"	// Key for EEPROM Address 93C66 Address 0719
#define key_93C66_Adr0720		"k93C66A0720"	// Key for EEPROM Address 93C66 Address 0720
#define key_93C66_Adr0721		"k93C66A0721"	// Key for EEPROM Address 93C66 Address 0721
#define key_93C66_Adr0722		"k93C66A0722"	// Key for EEPROM Address 93C66 Address 0722
#define key_93C66_Adr0723		"k93C66A0723"	// Key for EEPROM Address 93C66 Address 0723
#define key_93C66_Adr0724		"k93C66A0724"	// Key for EEPROM Address 93C66 Address 0724
#define key_93C66_Adr0725		"k93C66A0725"	// Key for EEPROM Address 93C66 Address 0725
#define key_93C66_Adr0726		"k93C66A0726"	// Key for EEPROM Address 93C66 Address 0726
#define key_93C66_Adr0727		"k93C66A0727"	// Key for EEPROM Address 93C66 Address 0727
#define key_93C66_Adr0728		"k93C66A0728"	// Key for EEPROM Address 93C66 Address 0728
#define key_93C66_Adr0729		"k93C66A0729"	// Key for EEPROM Address 93C66 Address 0729
#define key_93C66_Adr0730		"k93C66A0730"	// Key for EEPROM Address 93C66 Address 0730
#define key_93C66_Adr0731		"k93C66A0731"	// Key for EEPROM Address 93C66 Address 0731
#define key_93C66_Adr0732		"k93C66A0732"	// Key for EEPROM Address 93C66 Address 0732
#define key_93C66_Adr0733		"k93C66A0733"	// Key for EEPROM Address 93C66 Address 0733
#define key_93C66_Adr0734		"k93C66A0734"	// Key for EEPROM Address 93C66 Address 0734
#define key_93C66_Adr0735		"k93C66A0735"	// Key for EEPROM Address 93C66 Address 0735
#define key_93C66_Adr0736		"k93C66A0736"	// Key for EEPROM Address 93C66 Address 0736
#define key_93C66_Adr0737		"k93C66A0737"	// Key for EEPROM Address 93C66 Address 0737
#define key_93C66_Adr0738		"k93C66A0738"	// Key for EEPROM Address 93C66 Address 0738
#define key_93C66_Adr0739		"k93C66A0739"	// Key for EEPROM Address 93C66 Address 0739
#define key_93C66_Adr0740		"k93C66A0740"	// Key for EEPROM Address 93C66 Address 0740
#define key_93C66_Adr0741		"k93C66A0741"	// Key for EEPROM Address 93C66 Address 0741
#define key_93C66_Adr0742		"k93C66A0742"	// Key for EEPROM Address 93C66 Address 0742
#define key_93C66_Adr0743		"k93C66A0743"	// Key for EEPROM Address 93C66 Address 0743
#define key_93C66_Adr0744		"k93C66A0744"	// Key for EEPROM Address 93C66 Address 0744
#define key_93C66_Adr0745		"k93C66A0745"	// Key for EEPROM Address 93C66 Address 0745
#define key_93C66_Adr0746		"k93C66A0746"	// Key for EEPROM Address 93C66 Address 0746
#define key_93C66_Adr0747		"k93C66A0747"	// Key for EEPROM Address 93C66 Address 0747
#define key_93C66_Adr0748		"k93C66A0748"	// Key for EEPROM Address 93C66 Address 0748
#define key_93C66_Adr0749		"k93C66A0749"	// Key for EEPROM Address 93C66 Address 0749
#define key_93C66_Adr0750		"k93C66A0750"	// Key for EEPROM Address 93C66 Address 0750
#define key_93C66_Adr0751		"k93C66A0751"	// Key for EEPROM Address 93C66 Address 0751
#define key_93C66_Adr0752		"k93C66A0752"	// Key for EEPROM Address 93C66 Address 0752
#define key_93C66_Adr0753		"k93C66A0753"	// Key for EEPROM Address 93C66 Address 0753
#define key_93C66_Adr0754		"k93C66A0754"	// Key for EEPROM Address 93C66 Address 0754
#define key_93C66_Adr0755		"k93C66A0755"	// Key for EEPROM Address 93C66 Address 0755
#define key_93C66_Adr0756		"k93C66A0756"	// Key for EEPROM Address 93C66 Address 0756
#define key_93C66_Adr0757		"k93C66A0757"	// Key for EEPROM Address 93C66 Address 0757
#define key_93C66_Adr0758		"k93C66A0758"	// Key for EEPROM Address 93C66 Address 0758
#define key_93C66_Adr0759		"k93C66A0759"	// Key for EEPROM Address 93C66 Address 0759
#define key_93C66_Adr0760		"k93C66A0760"	// Key for EEPROM Address 93C66 Address 0760
#define key_93C66_Adr0761		"k93C66A0761"	// Key for EEPROM Address 93C66 Address 0761
#define key_93C66_Adr0762		"k93C66A0762"	// Key for EEPROM Address 93C66 Address 0762
#define key_93C66_Adr0763		"k93C66A0763"	// Key for EEPROM Address 93C66 Address 0763
#define key_93C66_Adr0764		"k93C66A0764"	// Key for EEPROM Address 93C66 Address 0764
#define key_93C66_Adr0765		"k93C66A0765"	// Key for EEPROM Address 93C66 Address 0765
#define key_93C66_Adr0766		"k93C66A0766"	// Key for EEPROM Address 93C66 Address 0766
#define key_93C66_Adr0767		"k93C66A0767"	// Key for EEPROM Address 93C66 Address 0767
#define key_93C66_Adr0768		"k93C66A0768"	// Key for EEPROM Address 93C66 Address 0768
#define key_93C66_Adr0769		"k93C66A0769"	// Key for EEPROM Address 93C66 Address 0769
#define key_93C66_Adr0770		"k93C66A0770"	// Key for EEPROM Address 93C66 Address 0770
#define key_93C66_Adr0771		"k93C66A0771"	// Key for EEPROM Address 93C66 Address 0771
#define key_93C66_Adr0772		"k93C66A0772"	// Key for EEPROM Address 93C66 Address 0772
#define key_93C66_Adr0773		"k93C66A0773"	// Key for EEPROM Address 93C66 Address 0773
#define key_93C66_Adr0774		"k93C66A0774"	// Key for EEPROM Address 93C66 Address 0774
#define key_93C66_Adr0775		"k93C66A0775"	// Key for EEPROM Address 93C66 Address 0775
#define key_93C66_Adr0776		"k93C66A0776"	// Key for EEPROM Address 93C66 Address 0776
#define key_93C66_Adr0777		"k93C66A0777"	// Key for EEPROM Address 93C66 Address 0777
#define key_93C66_Adr0778		"k93C66A0778"	// Key for EEPROM Address 93C66 Address 0778
#define key_93C66_Adr0779		"k93C66A0779"	// Key for EEPROM Address 93C66 Address 0779
#define key_93C66_Adr0780		"k93C66A0780"	// Key for EEPROM Address 93C66 Address 0780
#define key_93C66_Adr0781		"k93C66A0781"	// Key for EEPROM Address 93C66 Address 0781
#define key_93C66_Adr0782		"k93C66A0782"	// Key for EEPROM Address 93C66 Address 0782
#define key_93C66_Adr0783		"k93C66A0783"	// Key for EEPROM Address 93C66 Address 0783
#define key_93C66_Adr0784		"k93C66A0784"	// Key for EEPROM Address 93C66 Address 0784
#define key_93C66_Adr0785		"k93C66A0785"	// Key for EEPROM Address 93C66 Address 0785
#define key_93C66_Adr0786		"k93C66A0786"	// Key for EEPROM Address 93C66 Address 0786
#define key_93C66_Adr0787		"k93C66A0787"	// Key for EEPROM Address 93C66 Address 0787
#define key_93C66_Adr0788		"k93C66A0788"	// Key for EEPROM Address 93C66 Address 0788
#define key_93C66_Adr0789		"k93C66A0789"	// Key for EEPROM Address 93C66 Address 0789
#define key_93C66_Adr0790		"k93C66A0790"	// Key for EEPROM Address 93C66 Address 0790
#define key_93C66_Adr0791		"k93C66A0791"	// Key for EEPROM Address 93C66 Address 0791
#define key_93C66_Adr0792		"k93C66A0792"	// Key for EEPROM Address 93C66 Address 0792
#define key_93C66_Adr0793		"k93C66A0793"	// Key for EEPROM Address 93C66 Address 0793
#define key_93C66_Adr0794		"k93C66A0794"	// Key for EEPROM Address 93C66 Address 0794
#define key_93C66_Adr0795		"k93C66A0795"	// Key for EEPROM Address 93C66 Address 0795
#define key_93C66_Adr0796		"k93C66A0796"	// Key for EEPROM Address 93C66 Address 0796
#define key_93C66_Adr0797		"k93C66A0797"	// Key for EEPROM Address 93C66 Address 0797
#define key_93C66_Adr0798		"k93C66A0798"	// Key for EEPROM Address 93C66 Address 0798
#define key_93C66_Adr0799		"k93C66A0799"	// Key for EEPROM Address 93C66 Address 0799
#define key_93C66_Adr0800		"k93C66A0800"	// Key for EEPROM Address 93C66 Address 0800
#define key_93C66_Adr0801		"k93C66A0801"	// Key for EEPROM Address 93C66 Address 0801
#define key_93C66_Adr0802		"k93C66A0802"	// Key for EEPROM Address 93C66 Address 0802
#define key_93C66_Adr0803		"k93C66A0803"	// Key for EEPROM Address 93C66 Address 0803
#define key_93C66_Adr0804		"k93C66A0804"	// Key for EEPROM Address 93C66 Address 0804
#define key_93C66_Adr0805		"k93C66A0805"	// Key for EEPROM Address 93C66 Address 0805
#define key_93C66_Adr0806		"k93C66A0806"	// Key for EEPROM Address 93C66 Address 0806
#define key_93C66_Adr0807		"k93C66A0807"	// Key for EEPROM Address 93C66 Address 0807
#define key_93C66_Adr0808		"k93C66A0808"	// Key for EEPROM Address 93C66 Address 0808
#define key_93C66_Adr0809		"k93C66A0809"	// Key for EEPROM Address 93C66 Address 0809
#define key_93C66_Adr0810		"k93C66A0810"	// Key for EEPROM Address 93C66 Address 0810
#define key_93C66_Adr0811		"k93C66A0811"	// Key for EEPROM Address 93C66 Address 0811
#define key_93C66_Adr0812		"k93C66A0812"	// Key for EEPROM Address 93C66 Address 0812
#define key_93C66_Adr0813		"k93C66A0813"	// Key for EEPROM Address 93C66 Address 0813
#define key_93C66_Adr0814		"k93C66A0814"	// Key for EEPROM Address 93C66 Address 0814
#define key_93C66_Adr0815		"k93C66A0815"	// Key for EEPROM Address 93C66 Address 0815
#define key_93C66_Adr0816		"k93C66A0816"	// Key for EEPROM Address 93C66 Address 0816
#define key_93C66_Adr0817		"k93C66A0817"	// Key for EEPROM Address 93C66 Address 0817
#define key_93C66_Adr0818		"k93C66A0818"	// Key for EEPROM Address 93C66 Address 0818
#define key_93C66_Adr0819		"k93C66A0819"	// Key for EEPROM Address 93C66 Address 0819
#define key_93C66_Adr0820		"k93C66A0820"	// Key for EEPROM Address 93C66 Address 0820
#define key_93C66_Adr0821		"k93C66A0821"	// Key for EEPROM Address 93C66 Address 0821
#define key_93C66_Adr0822		"k93C66A0822"	// Key for EEPROM Address 93C66 Address 0822
#define key_93C66_Adr0823		"k93C66A0823"	// Key for EEPROM Address 93C66 Address 0823
#define key_93C66_Adr0824		"k93C66A0824"	// Key for EEPROM Address 93C66 Address 0824
#define key_93C66_Adr0825		"k93C66A0825"	// Key for EEPROM Address 93C66 Address 0825
#define key_93C66_Adr0826		"k93C66A0826"	// Key for EEPROM Address 93C66 Address 0826
#define key_93C66_Adr0827		"k93C66A0827"	// Key for EEPROM Address 93C66 Address 0827
#define key_93C66_Adr0828		"k93C66A0828"	// Key for EEPROM Address 93C66 Address 0828
#define key_93C66_Adr0829		"k93C66A0829"	// Key for EEPROM Address 93C66 Address 0829
#define key_93C66_Adr0830		"k93C66A0830"	// Key for EEPROM Address 93C66 Address 0830
#define key_93C66_Adr0831		"k93C66A0831"	// Key for EEPROM Address 93C66 Address 0831
#define key_93C66_Adr0832		"k93C66A0832"	// Key for EEPROM Address 93C66 Address 0832
#define key_93C66_Adr0833		"k93C66A0833"	// Key for EEPROM Address 93C66 Address 0833
#define key_93C66_Adr0834		"k93C66A0834"	// Key for EEPROM Address 93C66 Address 0834
#define key_93C66_Adr0835		"k93C66A0835"	// Key for EEPROM Address 93C66 Address 0835
#define key_93C66_Adr0836		"k93C66A0836"	// Key for EEPROM Address 93C66 Address 0836
#define key_93C66_Adr0837		"k93C66A0837"	// Key for EEPROM Address 93C66 Address 0837
#define key_93C66_Adr0838		"k93C66A0838"	// Key for EEPROM Address 93C66 Address 0838
#define key_93C66_Adr0839		"k93C66A0839"	// Key for EEPROM Address 93C66 Address 0839
#define key_93C66_Adr0840		"k93C66A0840"	// Key for EEPROM Address 93C66 Address 0840
#define key_93C66_Adr0841		"k93C66A0841"	// Key for EEPROM Address 93C66 Address 0841
#define key_93C66_Adr0842		"k93C66A0842"	// Key for EEPROM Address 93C66 Address 0842
#define key_93C66_Adr0843		"k93C66A0843"	// Key for EEPROM Address 93C66 Address 0843
#define key_93C66_Adr0844		"k93C66A0844"	// Key for EEPROM Address 93C66 Address 0844
#define key_93C66_Adr0845		"k93C66A0845"	// Key for EEPROM Address 93C66 Address 0845
#define key_93C66_Adr0846		"k93C66A0846"	// Key for EEPROM Address 93C66 Address 0846
#define key_93C66_Adr0847		"k93C66A0847"	// Key for EEPROM Address 93C66 Address 0847
#define key_93C66_Adr0848		"k93C66A0848"	// Key for EEPROM Address 93C66 Address 0848
#define key_93C66_Adr0849		"k93C66A0849"	// Key for EEPROM Address 93C66 Address 0849
#define key_93C66_Adr0850		"k93C66A0850"	// Key for EEPROM Address 93C66 Address 0850
#define key_93C66_Adr0851		"k93C66A0851"	// Key for EEPROM Address 93C66 Address 0851
#define key_93C66_Adr0852		"k93C66A0852"	// Key for EEPROM Address 93C66 Address 0852
#define key_93C66_Adr0853		"k93C66A0853"	// Key for EEPROM Address 93C66 Address 0853
#define key_93C66_Adr0854		"k93C66A0854"	// Key for EEPROM Address 93C66 Address 0854
#define key_93C66_Adr0855		"k93C66A0855"	// Key for EEPROM Address 93C66 Address 0855
#define key_93C66_Adr0856		"k93C66A0856"	// Key for EEPROM Address 93C66 Address 0856
#define key_93C66_Adr0857		"k93C66A0857"	// Key for EEPROM Address 93C66 Address 0857
#define key_93C66_Adr0858		"k93C66A0858"	// Key for EEPROM Address 93C66 Address 0858
#define key_93C66_Adr0859		"k93C66A0859"	// Key for EEPROM Address 93C66 Address 0859
#define key_93C66_Adr0860		"k93C66A0860"	// Key for EEPROM Address 93C66 Address 0860
#define key_93C66_Adr0861		"k93C66A0861"	// Key for EEPROM Address 93C66 Address 0861
#define key_93C66_Adr0862		"k93C66A0862"	// Key for EEPROM Address 93C66 Address 0862
#define key_93C66_Adr0863		"k93C66A0863"	// Key for EEPROM Address 93C66 Address 0863
#define key_93C66_Adr0864		"k93C66A0864"	// Key for EEPROM Address 93C66 Address 0864
#define key_93C66_Adr0865		"k93C66A0865"	// Key for EEPROM Address 93C66 Address 0865
#define key_93C66_Adr0866		"k93C66A0866"	// Key for EEPROM Address 93C66 Address 0866
#define key_93C66_Adr0867		"k93C66A0867"	// Key for EEPROM Address 93C66 Address 0867
#define key_93C66_Adr0868		"k93C66A0868"	// Key for EEPROM Address 93C66 Address 0868
#define key_93C66_Adr0869		"k93C66A0869"	// Key for EEPROM Address 93C66 Address 0869
#define key_93C66_Adr0870		"k93C66A0870"	// Key for EEPROM Address 93C66 Address 0870
#define key_93C66_Adr0871		"k93C66A0871"	// Key for EEPROM Address 93C66 Address 0871
#define key_93C66_Adr0872		"k93C66A0872"	// Key for EEPROM Address 93C66 Address 0872
#define key_93C66_Adr0873		"k93C66A0873"	// Key for EEPROM Address 93C66 Address 0873
#define key_93C66_Adr0874		"k93C66A0874"	// Key for EEPROM Address 93C66 Address 0874
#define key_93C66_Adr0875		"k93C66A0875"	// Key for EEPROM Address 93C66 Address 0875
#define key_93C66_Adr0876		"k93C66A0876"	// Key for EEPROM Address 93C66 Address 0876
#define key_93C66_Adr0877		"k93C66A0877"	// Key for EEPROM Address 93C66 Address 0877
#define key_93C66_Adr0878		"k93C66A0878"	// Key for EEPROM Address 93C66 Address 0878
#define key_93C66_Adr0879		"k93C66A0879"	// Key for EEPROM Address 93C66 Address 0879
#define key_93C66_Adr0880		"k93C66A0880"	// Key for EEPROM Address 93C66 Address 0880
#define key_93C66_Adr0881		"k93C66A0881"	// Key for EEPROM Address 93C66 Address 0881
#define key_93C66_Adr0882		"k93C66A0882"	// Key for EEPROM Address 93C66 Address 0882
#define key_93C66_Adr0883		"k93C66A0883"	// Key for EEPROM Address 93C66 Address 0883
#define key_93C66_Adr0884		"k93C66A0884"	// Key for EEPROM Address 93C66 Address 0884
#define key_93C66_Adr0885		"k93C66A0885"	// Key for EEPROM Address 93C66 Address 0885
#define key_93C66_Adr0886		"k93C66A0886"	// Key for EEPROM Address 93C66 Address 0886
#define key_93C66_Adr0887		"k93C66A0887"	// Key for EEPROM Address 93C66 Address 0887
#define key_93C66_Adr0888		"k93C66A0888"	// Key for EEPROM Address 93C66 Address 0888
#define key_93C66_Adr0889		"k93C66A0889"	// Key for EEPROM Address 93C66 Address 0889
#define key_93C66_Adr0890		"k93C66A0890"	// Key for EEPROM Address 93C66 Address 0890
#define key_93C66_Adr0891		"k93C66A0891"	// Key for EEPROM Address 93C66 Address 0891
#define key_93C66_Adr0892		"k93C66A0892"	// Key for EEPROM Address 93C66 Address 0892
#define key_93C66_Adr0893		"k93C66A0893"	// Key for EEPROM Address 93C66 Address 0893
#define key_93C66_Adr0894		"k93C66A0894"	// Key for EEPROM Address 93C66 Address 0894
#define key_93C66_Adr0895		"k93C66A0895"	// Key for EEPROM Address 93C66 Address 0895
#define key_93C66_Adr0896		"k93C66A0896"	// Key for EEPROM Address 93C66 Address 0896
#define key_93C66_Adr0897		"k93C66A0897"	// Key for EEPROM Address 93C66 Address 0897
#define key_93C66_Adr0898		"k93C66A0898"	// Key for EEPROM Address 93C66 Address 0898
#define key_93C66_Adr0899		"k93C66A0899"	// Key for EEPROM Address 93C66 Address 0899
#define key_93C66_Adr0900		"k93C66A0900"	// Key for EEPROM Address 93C66 Address 0900
#define key_93C66_Adr0901		"k93C66A0901"	// Key for EEPROM Address 93C66 Address 0901
#define key_93C66_Adr0902		"k93C66A0902"	// Key for EEPROM Address 93C66 Address 0902
#define key_93C66_Adr0903		"k93C66A0903"	// Key for EEPROM Address 93C66 Address 0903
#define key_93C66_Adr0904		"k93C66A0904"	// Key for EEPROM Address 93C66 Address 0904
#define key_93C66_Adr0905		"k93C66A0905"	// Key for EEPROM Address 93C66 Address 0905
#define key_93C66_Adr0906		"k93C66A0906"	// Key for EEPROM Address 93C66 Address 0906
#define key_93C66_Adr0907		"k93C66A0907"	// Key for EEPROM Address 93C66 Address 0907
#define key_93C66_Adr0908		"k93C66A0908"	// Key for EEPROM Address 93C66 Address 0908
#define key_93C66_Adr0909		"k93C66A0909"	// Key for EEPROM Address 93C66 Address 0909
#define key_93C66_Adr0910		"k93C66A0910"	// Key for EEPROM Address 93C66 Address 0910
#define key_93C66_Adr0911		"k93C66A0911"	// Key for EEPROM Address 93C66 Address 0911
#define key_93C66_Adr0912		"k93C66A0912"	// Key for EEPROM Address 93C66 Address 0912
#define key_93C66_Adr0913		"k93C66A0913"	// Key for EEPROM Address 93C66 Address 0913
#define key_93C66_Adr0914		"k93C66A0914"	// Key for EEPROM Address 93C66 Address 0914
#define key_93C66_Adr0915		"k93C66A0915"	// Key for EEPROM Address 93C66 Address 0915
#define key_93C66_Adr0916		"k93C66A0916"	// Key for EEPROM Address 93C66 Address 0916
#define key_93C66_Adr0917		"k93C66A0917"	// Key for EEPROM Address 93C66 Address 0917
#define key_93C66_Adr0918		"k93C66A0918"	// Key for EEPROM Address 93C66 Address 0918
#define key_93C66_Adr0919		"k93C66A0919"	// Key for EEPROM Address 93C66 Address 0919
#define key_93C66_Adr0920		"k93C66A0920"	// Key for EEPROM Address 93C66 Address 0920
#define key_93C66_Adr0921		"k93C66A0921"	// Key for EEPROM Address 93C66 Address 0921
#define key_93C66_Adr0922		"k93C66A0922"	// Key for EEPROM Address 93C66 Address 0922
#define key_93C66_Adr0923		"k93C66A0923"	// Key for EEPROM Address 93C66 Address 0923
#define key_93C66_Adr0924		"k93C66A0924"	// Key for EEPROM Address 93C66 Address 0924
#define key_93C66_Adr0925		"k93C66A0925"	// Key for EEPROM Address 93C66 Address 0925
#define key_93C66_Adr0926		"k93C66A0926"	// Key for EEPROM Address 93C66 Address 0926
#define key_93C66_Adr0927		"k93C66A0927"	// Key for EEPROM Address 93C66 Address 0927
#define key_93C66_Adr0928		"k93C66A0928"	// Key for EEPROM Address 93C66 Address 0928
#define key_93C66_Adr0929		"k93C66A0929"	// Key for EEPROM Address 93C66 Address 0929
#define key_93C66_Adr0930		"k93C66A0930"	// Key for EEPROM Address 93C66 Address 0930
#define key_93C66_Adr0931		"k93C66A0931"	// Key for EEPROM Address 93C66 Address 0931
#define key_93C66_Adr0932		"k93C66A0932"	// Key for EEPROM Address 93C66 Address 0932
#define key_93C66_Adr0933		"k93C66A0933"	// Key for EEPROM Address 93C66 Address 0933
#define key_93C66_Adr0934		"k93C66A0934"	// Key for EEPROM Address 93C66 Address 0934
#define key_93C66_Adr0935		"k93C66A0935"	// Key for EEPROM Address 93C66 Address 0935
#define key_93C66_Adr0936		"k93C66A0936"	// Key for EEPROM Address 93C66 Address 0936
#define key_93C66_Adr0937		"k93C66A0937"	// Key for EEPROM Address 93C66 Address 0937
#define key_93C66_Adr0938		"k93C66A0938"	// Key for EEPROM Address 93C66 Address 0938
#define key_93C66_Adr0939		"k93C66A0939"	// Key for EEPROM Address 93C66 Address 0939
#define key_93C66_Adr0940		"k93C66A0940"	// Key for EEPROM Address 93C66 Address 0940
#define key_93C66_Adr0941		"k93C66A0941"	// Key for EEPROM Address 93C66 Address 0941
#define key_93C66_Adr0942		"k93C66A0942"	// Key for EEPROM Address 93C66 Address 0942
#define key_93C66_Adr0943		"k93C66A0943"	// Key for EEPROM Address 93C66 Address 0943
#define key_93C66_Adr0944		"k93C66A0944"	// Key for EEPROM Address 93C66 Address 0944
#define key_93C66_Adr0945		"k93C66A0945"	// Key for EEPROM Address 93C66 Address 0945
#define key_93C66_Adr0946		"k93C66A0946"	// Key for EEPROM Address 93C66 Address 0946
#define key_93C66_Adr0947		"k93C66A0947"	// Key for EEPROM Address 93C66 Address 0947
#define key_93C66_Adr0948		"k93C66A0948"	// Key for EEPROM Address 93C66 Address 0948
#define key_93C66_Adr0949		"k93C66A0949"	// Key for EEPROM Address 93C66 Address 0949
#define key_93C66_Adr0950		"k93C66A0950"	// Key for EEPROM Address 93C66 Address 0950
#define key_93C66_Adr0951		"k93C66A0951"	// Key for EEPROM Address 93C66 Address 0951
#define key_93C66_Adr0952		"k93C66A0952"	// Key for EEPROM Address 93C66 Address 0952
#define key_93C66_Adr0953		"k93C66A0953"	// Key for EEPROM Address 93C66 Address 0953
#define key_93C66_Adr0954		"k93C66A0954"	// Key for EEPROM Address 93C66 Address 0954
#define key_93C66_Adr0955		"k93C66A0955"	// Key for EEPROM Address 93C66 Address 0955
#define key_93C66_Adr0956		"k93C66A0956"	// Key for EEPROM Address 93C66 Address 0956
#define key_93C66_Adr0957		"k93C66A0957"	// Key for EEPROM Address 93C66 Address 0957
#define key_93C66_Adr0958		"k93C66A0958"	// Key for EEPROM Address 93C66 Address 0958
#define key_93C66_Adr0959		"k93C66A0959"	// Key for EEPROM Address 93C66 Address 0959
#define key_93C66_Adr0960		"k93C66A0960"	// Key for EEPROM Address 93C66 Address 0960
#define key_93C66_Adr0961		"k93C66A0961"	// Key for EEPROM Address 93C66 Address 0961
#define key_93C66_Adr0962		"k93C66A0962"	// Key for EEPROM Address 93C66 Address 0962
#define key_93C66_Adr0963		"k93C66A0963"	// Key for EEPROM Address 93C66 Address 0963
#define key_93C66_Adr0964		"k93C66A0964"	// Key for EEPROM Address 93C66 Address 0964
#define key_93C66_Adr0965		"k93C66A0965"	// Key for EEPROM Address 93C66 Address 0965
#define key_93C66_Adr0966		"k93C66A0966"	// Key for EEPROM Address 93C66 Address 0966
#define key_93C66_Adr0967		"k93C66A0967"	// Key for EEPROM Address 93C66 Address 0967
#define key_93C66_Adr0968		"k93C66A0968"	// Key for EEPROM Address 93C66 Address 0968
#define key_93C66_Adr0969		"k93C66A0969"	// Key for EEPROM Address 93C66 Address 0969
#define key_93C66_Adr0970		"k93C66A0970"	// Key for EEPROM Address 93C66 Address 0970
#define key_93C66_Adr0971		"k93C66A0971"	// Key for EEPROM Address 93C66 Address 0971
#define key_93C66_Adr0972		"k93C66A0972"	// Key for EEPROM Address 93C66 Address 0972
#define key_93C66_Adr0973		"k93C66A0973"	// Key for EEPROM Address 93C66 Address 0973
#define key_93C66_Adr0974		"k93C66A0974"	// Key for EEPROM Address 93C66 Address 0974
#define key_93C66_Adr0975		"k93C66A0975"	// Key for EEPROM Address 93C66 Address 0975
#define key_93C66_Adr0976		"k93C66A0976"	// Key for EEPROM Address 93C66 Address 0976
#define key_93C66_Adr0977		"k93C66A0977"	// Key for EEPROM Address 93C66 Address 0977
#define key_93C66_Adr0978		"k93C66A0978"	// Key for EEPROM Address 93C66 Address 0978
#define key_93C66_Adr0979		"k93C66A0979"	// Key for EEPROM Address 93C66 Address 0979
#define key_93C66_Adr0980		"k93C66A0980"	// Key for EEPROM Address 93C66 Address 0980
#define key_93C66_Adr0981		"k93C66A0981"	// Key for EEPROM Address 93C66 Address 0981
#define key_93C66_Adr0982		"k93C66A0982"	// Key for EEPROM Address 93C66 Address 0982
#define key_93C66_Adr0983		"k93C66A0983"	// Key for EEPROM Address 93C66 Address 0983
#define key_93C66_Adr0984		"k93C66A0984"	// Key for EEPROM Address 93C66 Address 0984
#define key_93C66_Adr0985		"k93C66A0985"	// Key for EEPROM Address 93C66 Address 0985
#define key_93C66_Adr0986		"k93C66A0986"	// Key for EEPROM Address 93C66 Address 0986
#define key_93C66_Adr0987		"k93C66A0987"	// Key for EEPROM Address 93C66 Address 0987
#define key_93C66_Adr0988		"k93C66A0988"	// Key for EEPROM Address 93C66 Address 0988
#define key_93C66_Adr0989		"k93C66A0989"	// Key for EEPROM Address 93C66 Address 0989
#define key_93C66_Adr0990		"k93C66A0990"	// Key for EEPROM Address 93C66 Address 0990
#define key_93C66_Adr0991		"k93C66A0991"	// Key for EEPROM Address 93C66 Address 0991
#define key_93C66_Adr0992		"k93C66A0992"	// Key for EEPROM Address 93C66 Address 0992
#define key_93C66_Adr0993		"k93C66A0993"	// Key for EEPROM Address 93C66 Address 0993
#define key_93C66_Adr0994		"k93C66A0994"	// Key for EEPROM Address 93C66 Address 0994
#define key_93C66_Adr0995		"k93C66A0995"	// Key for EEPROM Address 93C66 Address 0995
#define key_93C66_Adr0996		"k93C66A0996"	// Key for EEPROM Address 93C66 Address 0996
#define key_93C66_Adr0997		"k93C66A0997"	// Key for EEPROM Address 93C66 Address 0997
#define key_93C66_Adr0998		"k93C66A0998"	// Key for EEPROM Address 93C66 Address 0998
#define key_93C66_Adr0999		"k93C66A0999"	// Key for EEPROM Address 93C66 Address 0999
#define key_93C66_Adr1000		"k93C66A1000"	// Key for EEPROM Address 93C66 Address 1000
#define key_93C66_Adr1001		"k93C66A1001"	// Key for EEPROM Address 93C66 Address 1001
#define key_93C66_Adr1002		"k93C66A1002"	// Key for EEPROM Address 93C66 Address 1002
#define key_93C66_Adr1003		"k93C66A1003"	// Key for EEPROM Address 93C66 Address 1003
#define key_93C66_Adr1004		"k93C66A1004"	// Key for EEPROM Address 93C66 Address 1004
#define key_93C66_Adr1005		"k93C66A1005"	// Key for EEPROM Address 93C66 Address 1005
#define key_93C66_Adr1006		"k93C66A1006"	// Key for EEPROM Address 93C66 Address 1006
#define key_93C66_Adr1007		"k93C66A1007"	// Key for EEPROM Address 93C66 Address 1007
#define key_93C66_Adr1008		"k93C66A1008"	// Key for EEPROM Address 93C66 Address 1008
#define key_93C66_Adr1009		"k93C66A1009"	// Key for EEPROM Address 93C66 Address 1009
#define key_93C66_Adr1010		"k93C66A1010"	// Key for EEPROM Address 93C66 Address 1010
#define key_93C66_Adr1011		"k93C66A1011"	// Key for EEPROM Address 93C66 Address 1011
#define key_93C66_Adr1012		"k93C66A1012"	// Key for EEPROM Address 93C66 Address 1012
#define key_93C66_Adr1013		"k93C66A1013"	// Key for EEPROM Address 93C66 Address 1013
#define key_93C66_Adr1014		"k93C66A1014"	// Key for EEPROM Address 93C66 Address 1014
#define key_93C66_Adr1015		"k93C66A1015"	// Key for EEPROM Address 93C66 Address 1015
#define key_93C66_Adr1016		"k93C66A1016"	// Key for EEPROM Address 93C66 Address 1016
#define key_93C66_Adr1017		"k93C66A1017"	// Key for EEPROM Address 93C66 Address 1017
#define key_93C66_Adr1018		"k93C66A1018"	// Key for EEPROM Address 93C66 Address 1018
#define key_93C66_Adr1019		"k93C66A1019"	// Key for EEPROM Address 93C66 Address 1019
#define key_93C66_Adr1020		"k93C66A1020"	// Key for EEPROM Address 93C66 Address 1020
#define key_93C66_Adr1021		"k93C66A1021"	// Key for EEPROM Address 93C66 Address 1021
#define key_93C66_Adr1022		"k93C66A1022"	// Key for EEPROM Address 93C66 Address 1022
#define key_93C66_Adr1023		"k93C66A1023"	// Key for EEPROM Address 93C66 Address 1023
#define key_93C66_Adr1024		"k93C66A1024"	// Key for EEPROM Address 93C66 Address 1024



struct nvsAdr_S
{
	const char* key;
};


typedef struct nvsAdr_S nvsAdr_T;

static const nvsAdr_T  nvsAdr [] =
{
		{ key_93C66_Adr0000 },   /*   */
		{ key_93C66_Adr0001 },   /*   */
		{ key_93C66_Adr0002 },   /*   */
		{ key_93C66_Adr0003 },   /*   */
		{ key_93C66_Adr0004 },   /*   */
		{ key_93C66_Adr0005 },   /*   */
		{ key_93C66_Adr0006 },   /*   */
		{ key_93C66_Adr0007 },   /*   */
		{ key_93C66_Adr0008 },   /*   */
		{ key_93C66_Adr0009 },   /*   */
		{ key_93C66_Adr0010 },   /*   */
		{ key_93C66_Adr0011 },   /*   */
		{ key_93C66_Adr0012 },   /*   */
		{ key_93C66_Adr0013 },   /*   */
		{ key_93C66_Adr0014 },   /*   */
		{ key_93C66_Adr0015 },   /*   */
		{ key_93C66_Adr0016 },   /*   */
		{ key_93C66_Adr0017 },   /*   */
		{ key_93C66_Adr0018 },   /*   */
		{ key_93C66_Adr0019 },   /*   */
		{ key_93C66_Adr0020 },   /*   */
		{ key_93C66_Adr0021 },   /*   */
		{ key_93C66_Adr0022 },   /*   */
		{ key_93C66_Adr0023 },   /*   */
		{ key_93C66_Adr0024 },   /*   */
		{ key_93C66_Adr0025 },   /*   */
		{ key_93C66_Adr0026 },   /*   */
		{ key_93C66_Adr0027 },   /*   */
		{ key_93C66_Adr0028 },   /*   */
		{ key_93C66_Adr0029 },   /*   */
		{ key_93C66_Adr0030 },   /*   */
		{ key_93C66_Adr0031 },   /*   */
		{ key_93C66_Adr0032 },   /*   */
		{ key_93C66_Adr0033 },   /*   */
		{ key_93C66_Adr0034 },   /*   */
		{ key_93C66_Adr0035 },   /*   */
		{ key_93C66_Adr0036 },   /*   */
		{ key_93C66_Adr0037 },   /*   */
		{ key_93C66_Adr0038 },   /*   */
		{ key_93C66_Adr0039 },   /*   */
		{ key_93C66_Adr0040 },   /*   */
		{ key_93C66_Adr0041 },   /*   */
		{ key_93C66_Adr0042 },   /*   */
		{ key_93C66_Adr0043 },   /*   */
		{ key_93C66_Adr0044 },   /*   */
		{ key_93C66_Adr0045 },   /*   */
		{ key_93C66_Adr0046 },   /*   */
		{ key_93C66_Adr0047 },   /*   */
		{ key_93C66_Adr0048 },   /*   */
		{ key_93C66_Adr0049 },   /*   */
		{ key_93C66_Adr0050 },   /*   */
		{ key_93C66_Adr0051 },   /*   */
		{ key_93C66_Adr0052 },   /*   */
		{ key_93C66_Adr0053 },   /*   */
		{ key_93C66_Adr0054 },   /*   */
		{ key_93C66_Adr0055 },   /*   */
		{ key_93C66_Adr0056 },   /*   */
		{ key_93C66_Adr0057 },   /*   */
		{ key_93C66_Adr0058 },   /*   */
		{ key_93C66_Adr0059 },   /*   */
		{ key_93C66_Adr0060 },   /*   */
		{ key_93C66_Adr0061 },   /*   */
		{ key_93C66_Adr0062 },   /*   */
		{ key_93C66_Adr0063 },   /*   */
		{ key_93C66_Adr0064 },   /*   */
		{ key_93C66_Adr0065 },   /*   */
		{ key_93C66_Adr0066 },   /*   */
		{ key_93C66_Adr0067 },   /*   */
		{ key_93C66_Adr0068 },   /*   */
		{ key_93C66_Adr0069 },   /*   */
		{ key_93C66_Adr0070 },   /*   */
		{ key_93C66_Adr0071 },   /*   */
		{ key_93C66_Adr0072 },   /*   */
		{ key_93C66_Adr0073 },   /*   */
		{ key_93C66_Adr0074 },   /*   */
		{ key_93C66_Adr0075 },   /*   */
		{ key_93C66_Adr0076 },   /*   */
		{ key_93C66_Adr0077 },   /*   */
		{ key_93C66_Adr0078 },   /*   */
		{ key_93C66_Adr0079 },   /*   */
		{ key_93C66_Adr0080 },   /*   */
		{ key_93C66_Adr0081 },   /*   */
		{ key_93C66_Adr0082 },   /*   */
		{ key_93C66_Adr0083 },   /*   */
		{ key_93C66_Adr0084 },   /*   */
		{ key_93C66_Adr0085 },   /*   */
		{ key_93C66_Adr0086 },   /*   */
		{ key_93C66_Adr0087 },   /*   */
		{ key_93C66_Adr0088 },   /*   */
		{ key_93C66_Adr0089 },   /*   */
		{ key_93C66_Adr0090 },   /*   */
		{ key_93C66_Adr0091 },   /*   */
		{ key_93C66_Adr0092 },   /*   */
		{ key_93C66_Adr0093 },   /*   */
		{ key_93C66_Adr0094 },   /*   */
		{ key_93C66_Adr0095 },   /*   */
		{ key_93C66_Adr0096 },   /*   */
		{ key_93C66_Adr0097 },   /*   */
		{ key_93C66_Adr0098 },   /*   */
		{ key_93C66_Adr0099 },   /*   */
		{ key_93C66_Adr0100 },   /*   */
		{ key_93C66_Adr0101 },   /*   */
		{ key_93C66_Adr0102 },   /*   */
		{ key_93C66_Adr0103 },   /*   */
		{ key_93C66_Adr0104 },   /*   */
		{ key_93C66_Adr0105 },   /*   */
		{ key_93C66_Adr0106 },   /*   */
		{ key_93C66_Adr0107 },   /*   */
		{ key_93C66_Adr0108 },   /*   */
		{ key_93C66_Adr0109 },   /*   */
		{ key_93C66_Adr0110 },   /*   */
		{ key_93C66_Adr0111 },   /*   */
		{ key_93C66_Adr0112 },   /*   */
		{ key_93C66_Adr0113 },   /*   */
		{ key_93C66_Adr0114 },   /*   */
		{ key_93C66_Adr0115 },   /*   */
		{ key_93C66_Adr0116 },   /*   */
		{ key_93C66_Adr0117 },   /*   */
		{ key_93C66_Adr0118 },   /*   */
		{ key_93C66_Adr0119 },   /*   */
		{ key_93C66_Adr0120 },   /*   */
		{ key_93C66_Adr0121 },   /*   */
		{ key_93C66_Adr0122 },   /*   */
		{ key_93C66_Adr0123 },   /*   */
		{ key_93C66_Adr0124 },   /*   */
		{ key_93C66_Adr0125 },   /*   */
		{ key_93C66_Adr0126 },   /*   */
		{ key_93C66_Adr0127 },   /*   */
		{ key_93C66_Adr0128 },   /*   */
		{ key_93C66_Adr0129 },   /*   */
		{ key_93C66_Adr0130 },   /*   */
		{ key_93C66_Adr0131 },   /*   */
		{ key_93C66_Adr0132 },   /*   */
		{ key_93C66_Adr0133 },   /*   */
		{ key_93C66_Adr0134 },   /*   */
		{ key_93C66_Adr0135 },   /*   */
		{ key_93C66_Adr0136 },   /*   */
		{ key_93C66_Adr0137 },   /*   */
		{ key_93C66_Adr0138 },   /*   */
		{ key_93C66_Adr0139 },   /*   */
		{ key_93C66_Adr0140 },   /*   */
		{ key_93C66_Adr0141 },   /*   */
		{ key_93C66_Adr0142 },   /*   */
		{ key_93C66_Adr0143 },   /*   */
		{ key_93C66_Adr0144 },   /*   */
		{ key_93C66_Adr0145 },   /*   */
		{ key_93C66_Adr0146 },   /*   */
		{ key_93C66_Adr0147 },   /*   */
		{ key_93C66_Adr0148 },   /*   */
		{ key_93C66_Adr0149 },   /*   */
		{ key_93C66_Adr0150 },   /*   */
		{ key_93C66_Adr0151 },   /*   */
		{ key_93C66_Adr0152 },   /*   */
		{ key_93C66_Adr0153 },   /*   */
		{ key_93C66_Adr0154 },   /*   */
		{ key_93C66_Adr0155 },   /*   */
		{ key_93C66_Adr0156 },   /*   */
		{ key_93C66_Adr0157 },   /*   */
		{ key_93C66_Adr0158 },   /*   */
		{ key_93C66_Adr0159 },   /*   */
		{ key_93C66_Adr0160 },   /*   */
		{ key_93C66_Adr0161 },   /*   */
		{ key_93C66_Adr0162 },   /*   */
		{ key_93C66_Adr0163 },   /*   */
		{ key_93C66_Adr0164 },   /*   */
		{ key_93C66_Adr0165 },   /*   */
		{ key_93C66_Adr0166 },   /*   */
		{ key_93C66_Adr0167 },   /*   */
		{ key_93C66_Adr0168 },   /*   */
		{ key_93C66_Adr0169 },   /*   */
		{ key_93C66_Adr0170 },   /*   */
		{ key_93C66_Adr0171 },   /*   */
		{ key_93C66_Adr0172 },   /*   */
		{ key_93C66_Adr0173 },   /*   */
		{ key_93C66_Adr0174 },   /*   */
		{ key_93C66_Adr0175 },   /*   */
		{ key_93C66_Adr0176 },   /*   */
		{ key_93C66_Adr0177 },   /*   */
		{ key_93C66_Adr0178 },   /*   */
		{ key_93C66_Adr0179 },   /*   */
		{ key_93C66_Adr0180 },   /*   */
		{ key_93C66_Adr0181 },   /*   */
		{ key_93C66_Adr0182 },   /*   */
		{ key_93C66_Adr0183 },   /*   */
		{ key_93C66_Adr0184 },   /*   */
		{ key_93C66_Adr0185 },   /*   */
		{ key_93C66_Adr0186 },   /*   */
		{ key_93C66_Adr0187 },   /*   */
		{ key_93C66_Adr0188 },   /*   */
		{ key_93C66_Adr0189 },   /*   */
		{ key_93C66_Adr0190 },   /*   */
		{ key_93C66_Adr0191 },   /*   */
		{ key_93C66_Adr0192 },   /*   */
		{ key_93C66_Adr0193 },   /*   */
		{ key_93C66_Adr0194 },   /*   */
		{ key_93C66_Adr0195 },   /*   */
		{ key_93C66_Adr0196 },   /*   */
		{ key_93C66_Adr0197 },   /*   */
		{ key_93C66_Adr0198 },   /*   */
		{ key_93C66_Adr0199 },   /*   */
		{ key_93C66_Adr0200 },   /*   */
		{ key_93C66_Adr0201 },   /*   */
		{ key_93C66_Adr0202 },   /*   */
		{ key_93C66_Adr0203 },   /*   */
		{ key_93C66_Adr0204 },   /*   */
		{ key_93C66_Adr0205 },   /*   */
		{ key_93C66_Adr0206 },   /*   */
		{ key_93C66_Adr0207 },   /*   */
		{ key_93C66_Adr0208 },   /*   */
		{ key_93C66_Adr0209 },   /*   */
		{ key_93C66_Adr0210 },   /*   */
		{ key_93C66_Adr0211 },   /*   */
		{ key_93C66_Adr0212 },   /*   */
		{ key_93C66_Adr0213 },   /*   */
		{ key_93C66_Adr0214 },   /*   */
		{ key_93C66_Adr0215 },   /*   */
		{ key_93C66_Adr0216 },   /*   */
		{ key_93C66_Adr0217 },   /*   */
		{ key_93C66_Adr0218 },   /*   */
		{ key_93C66_Adr0219 },   /*   */
		{ key_93C66_Adr0220 },   /*   */
		{ key_93C66_Adr0221 },   /*   */
		{ key_93C66_Adr0222 },   /*   */
		{ key_93C66_Adr0223 },   /*   */
		{ key_93C66_Adr0224 },   /*   */
		{ key_93C66_Adr0225 },   /*   */
		{ key_93C66_Adr0226 },   /*   */
		{ key_93C66_Adr0227 },   /*   */
		{ key_93C66_Adr0228 },   /*   */
		{ key_93C66_Adr0229 },   /*   */
		{ key_93C66_Adr0230 },   /*   */
		{ key_93C66_Adr0231 },   /*   */
		{ key_93C66_Adr0232 },   /*   */
		{ key_93C66_Adr0233 },   /*   */
		{ key_93C66_Adr0234 },   /*   */
		{ key_93C66_Adr0235 },   /*   */
		{ key_93C66_Adr0236 },   /*   */
		{ key_93C66_Adr0237 },   /*   */
		{ key_93C66_Adr0238 },   /*   */
		{ key_93C66_Adr0239 },   /*   */
		{ key_93C66_Adr0240 },   /*   */
		{ key_93C66_Adr0241 },   /*   */
		{ key_93C66_Adr0242 },   /*   */
		{ key_93C66_Adr0243 },   /*   */
		{ key_93C66_Adr0244 },   /*   */
		{ key_93C66_Adr0245 },   /*   */
		{ key_93C66_Adr0246 },   /*   */
		{ key_93C66_Adr0247 },   /*   */
		{ key_93C66_Adr0248 },   /*   */
		{ key_93C66_Adr0249 },   /*   */
		{ key_93C66_Adr0250 },   /*   */
		{ key_93C66_Adr0251 },   /*   */
		{ key_93C66_Adr0252 },   /*   */
		{ key_93C66_Adr0253 },   /*   */
		{ key_93C66_Adr0254 },   /*   */
		{ key_93C66_Adr0255 },   /*   */
		{ key_93C66_Adr0256 },   /*   */
		{ key_93C66_Adr0257 },   /*   */
		{ key_93C66_Adr0258 },   /*   */
		{ key_93C66_Adr0259 },   /*   */
		{ key_93C66_Adr0260 },   /*   */
		{ key_93C66_Adr0261 },   /*   */
		{ key_93C66_Adr0262 },   /*   */
		{ key_93C66_Adr0263 },   /*   */
		{ key_93C66_Adr0264 },   /*   */
		{ key_93C66_Adr0265 },   /*   */
		{ key_93C66_Adr0266 },   /*   */
		{ key_93C66_Adr0267 },   /*   */
		{ key_93C66_Adr0268 },   /*   */
		{ key_93C66_Adr0269 },   /*   */
		{ key_93C66_Adr0270 },   /*   */
		{ key_93C66_Adr0271 },   /*   */
		{ key_93C66_Adr0272 },   /*   */
		{ key_93C66_Adr0273 },   /*   */
		{ key_93C66_Adr0274 },   /*   */
		{ key_93C66_Adr0275 },   /*   */
		{ key_93C66_Adr0276 },   /*   */
		{ key_93C66_Adr0277 },   /*   */
		{ key_93C66_Adr0278 },   /*   */
		{ key_93C66_Adr0279 },   /*   */
		{ key_93C66_Adr0280 },   /*   */
		{ key_93C66_Adr0281 },   /*   */
		{ key_93C66_Adr0282 },   /*   */
		{ key_93C66_Adr0283 },   /*   */
		{ key_93C66_Adr0284 },   /*   */
		{ key_93C66_Adr0285 },   /*   */
		{ key_93C66_Adr0286 },   /*   */
		{ key_93C66_Adr0287 },   /*   */
		{ key_93C66_Adr0288 },   /*   */
		{ key_93C66_Adr0289 },   /*   */
		{ key_93C66_Adr0290 },   /*   */
		{ key_93C66_Adr0291 },   /*   */
		{ key_93C66_Adr0292 },   /*   */
		{ key_93C66_Adr0293 },   /*   */
		{ key_93C66_Adr0294 },   /*   */
		{ key_93C66_Adr0295 },   /*   */
		{ key_93C66_Adr0296 },   /*   */
		{ key_93C66_Adr0297 },   /*   */
		{ key_93C66_Adr0298 },   /*   */
		{ key_93C66_Adr0299 },   /*   */
		{ key_93C66_Adr0300 },   /*   */
		{ key_93C66_Adr0301 },   /*   */
		{ key_93C66_Adr0302 },   /*   */
		{ key_93C66_Adr0303 },   /*   */
		{ key_93C66_Adr0304 },   /*   */
		{ key_93C66_Adr0305 },   /*   */
		{ key_93C66_Adr0306 },   /*   */
		{ key_93C66_Adr0307 },   /*   */
		{ key_93C66_Adr0308 },   /*   */
		{ key_93C66_Adr0309 },   /*   */
		{ key_93C66_Adr0310 },   /*   */
		{ key_93C66_Adr0311 },   /*   */
		{ key_93C66_Adr0312 },   /*   */
		{ key_93C66_Adr0313 },   /*   */
		{ key_93C66_Adr0314 },   /*   */
		{ key_93C66_Adr0315 },   /*   */
		{ key_93C66_Adr0316 },   /*   */
		{ key_93C66_Adr0317 },   /*   */
		{ key_93C66_Adr0318 },   /*   */
		{ key_93C66_Adr0319 },   /*   */
		{ key_93C66_Adr0320 },   /*   */
		{ key_93C66_Adr0321 },   /*   */
		{ key_93C66_Adr0322 },   /*   */
		{ key_93C66_Adr0323 },   /*   */
		{ key_93C66_Adr0324 },   /*   */
		{ key_93C66_Adr0325 },   /*   */
		{ key_93C66_Adr0326 },   /*   */
		{ key_93C66_Adr0327 },   /*   */
		{ key_93C66_Adr0328 },   /*   */
		{ key_93C66_Adr0329 },   /*   */
		{ key_93C66_Adr0330 },   /*   */
		{ key_93C66_Adr0331 },   /*   */
		{ key_93C66_Adr0332 },   /*   */
		{ key_93C66_Adr0333 },   /*   */
		{ key_93C66_Adr0334 },   /*   */
		{ key_93C66_Adr0335 },   /*   */
		{ key_93C66_Adr0336 },   /*   */
		{ key_93C66_Adr0337 },   /*   */
		{ key_93C66_Adr0338 },   /*   */
		{ key_93C66_Adr0339 },   /*   */
		{ key_93C66_Adr0340 },   /*   */
		{ key_93C66_Adr0341 },   /*   */
		{ key_93C66_Adr0342 },   /*   */
		{ key_93C66_Adr0343 },   /*   */
		{ key_93C66_Adr0344 },   /*   */
		{ key_93C66_Adr0345 },   /*   */
		{ key_93C66_Adr0346 },   /*   */
		{ key_93C66_Adr0347 },   /*   */
		{ key_93C66_Adr0348 },   /*   */
		{ key_93C66_Adr0349 },   /*   */
		{ key_93C66_Adr0350 },   /*   */
		{ key_93C66_Adr0351 },   /*   */
		{ key_93C66_Adr0352 },   /*   */
		{ key_93C66_Adr0353 },   /*   */
		{ key_93C66_Adr0354 },   /*   */
		{ key_93C66_Adr0355 },   /*   */
		{ key_93C66_Adr0356 },   /*   */
		{ key_93C66_Adr0357 },   /*   */
		{ key_93C66_Adr0358 },   /*   */
		{ key_93C66_Adr0359 },   /*   */
		{ key_93C66_Adr0360 },   /*   */
		{ key_93C66_Adr0361 },   /*   */
		{ key_93C66_Adr0362 },   /*   */
		{ key_93C66_Adr0363 },   /*   */
		{ key_93C66_Adr0364 },   /*   */
		{ key_93C66_Adr0365 },   /*   */
		{ key_93C66_Adr0366 },   /*   */
		{ key_93C66_Adr0367 },   /*   */
		{ key_93C66_Adr0368 },   /*   */
		{ key_93C66_Adr0369 },   /*   */
		{ key_93C66_Adr0370 },   /*   */
		{ key_93C66_Adr0371 },   /*   */
		{ key_93C66_Adr0372 },   /*   */
		{ key_93C66_Adr0373 },   /*   */
		{ key_93C66_Adr0374 },   /*   */
		{ key_93C66_Adr0375 },   /*   */
		{ key_93C66_Adr0376 },   /*   */
		{ key_93C66_Adr0377 },   /*   */
		{ key_93C66_Adr0378 },   /*   */
		{ key_93C66_Adr0379 },   /*   */
		{ key_93C66_Adr0380 },   /*   */
		{ key_93C66_Adr0381 },   /*   */
		{ key_93C66_Adr0382 },   /*   */
		{ key_93C66_Adr0383 },   /*   */
		{ key_93C66_Adr0384 },   /*   */
		{ key_93C66_Adr0385 },   /*   */
		{ key_93C66_Adr0386 },   /*   */
		{ key_93C66_Adr0387 },   /*   */
		{ key_93C66_Adr0388 },   /*   */
		{ key_93C66_Adr0389 },   /*   */
		{ key_93C66_Adr0390 },   /*   */
		{ key_93C66_Adr0391 },   /*   */
		{ key_93C66_Adr0392 },   /*   */
		{ key_93C66_Adr0393 },   /*   */
		{ key_93C66_Adr0394 },   /*   */
		{ key_93C66_Adr0395 },   /*   */
		{ key_93C66_Adr0396 },   /*   */
		{ key_93C66_Adr0397 },   /*   */
		{ key_93C66_Adr0398 },   /*   */
		{ key_93C66_Adr0399 },   /*   */
		{ key_93C66_Adr0400 },   /*   */
		{ key_93C66_Adr0401 },   /*   */
		{ key_93C66_Adr0402 },   /*   */
		{ key_93C66_Adr0403 },   /*   */
		{ key_93C66_Adr0404 },   /*   */
		{ key_93C66_Adr0405 },   /*   */
		{ key_93C66_Adr0406 },   /*   */
		{ key_93C66_Adr0407 },   /*   */
		{ key_93C66_Adr0408 },   /*   */
		{ key_93C66_Adr0409 },   /*   */
		{ key_93C66_Adr0410 },   /*   */
		{ key_93C66_Adr0411 },   /*   */
		{ key_93C66_Adr0412 },   /*   */
		{ key_93C66_Adr0413 },   /*   */
		{ key_93C66_Adr0414 },   /*   */
		{ key_93C66_Adr0415 },   /*   */
		{ key_93C66_Adr0416 },   /*   */
		{ key_93C66_Adr0417 },   /*   */
		{ key_93C66_Adr0418 },   /*   */
		{ key_93C66_Adr0419 },   /*   */
		{ key_93C66_Adr0420 },   /*   */
		{ key_93C66_Adr0421 },   /*   */
		{ key_93C66_Adr0422 },   /*   */
		{ key_93C66_Adr0423 },   /*   */
		{ key_93C66_Adr0424 },   /*   */
		{ key_93C66_Adr0425 },   /*   */
		{ key_93C66_Adr0426 },   /*   */
		{ key_93C66_Adr0427 },   /*   */
		{ key_93C66_Adr0428 },   /*   */
		{ key_93C66_Adr0429 },   /*   */
		{ key_93C66_Adr0430 },   /*   */
		{ key_93C66_Adr0431 },   /*   */
		{ key_93C66_Adr0432 },   /*   */
		{ key_93C66_Adr0433 },   /*   */
		{ key_93C66_Adr0434 },   /*   */
		{ key_93C66_Adr0435 },   /*   */
		{ key_93C66_Adr0436 },   /*   */
		{ key_93C66_Adr0437 },   /*   */
		{ key_93C66_Adr0438 },   /*   */
		{ key_93C66_Adr0439 },   /*   */
		{ key_93C66_Adr0440 },   /*   */
		{ key_93C66_Adr0441 },   /*   */
		{ key_93C66_Adr0442 },   /*   */
		{ key_93C66_Adr0443 },   /*   */
		{ key_93C66_Adr0444 },   /*   */
		{ key_93C66_Adr0445 },   /*   */
		{ key_93C66_Adr0446 },   /*   */
		{ key_93C66_Adr0447 },   /*   */
		{ key_93C66_Adr0448 },   /*   */
		{ key_93C66_Adr0449 },   /*   */
		{ key_93C66_Adr0450 },   /*   */
		{ key_93C66_Adr0451 },   /*   */
		{ key_93C66_Adr0452 },   /*   */
		{ key_93C66_Adr0453 },   /*   */
		{ key_93C66_Adr0454 },   /*   */
		{ key_93C66_Adr0455 },   /*   */
		{ key_93C66_Adr0456 },   /*   */
		{ key_93C66_Adr0457 },   /*   */
		{ key_93C66_Adr0458 },   /*   */
		{ key_93C66_Adr0459 },   /*   */
		{ key_93C66_Adr0460 },   /*   */
		{ key_93C66_Adr0461 },   /*   */
		{ key_93C66_Adr0462 },   /*   */
		{ key_93C66_Adr0463 },   /*   */
		{ key_93C66_Adr0464 },   /*   */
		{ key_93C66_Adr0465 },   /*   */
		{ key_93C66_Adr0466 },   /*   */
		{ key_93C66_Adr0467 },   /*   */
		{ key_93C66_Adr0468 },   /*   */
		{ key_93C66_Adr0469 },   /*   */
		{ key_93C66_Adr0470 },   /*   */
		{ key_93C66_Adr0471 },   /*   */
		{ key_93C66_Adr0472 },   /*   */
		{ key_93C66_Adr0473 },   /*   */
		{ key_93C66_Adr0474 },   /*   */
		{ key_93C66_Adr0475 },   /*   */
		{ key_93C66_Adr0476 },   /*   */
		{ key_93C66_Adr0477 },   /*   */
		{ key_93C66_Adr0478 },   /*   */
		{ key_93C66_Adr0479 },   /*   */
		{ key_93C66_Adr0480 },   /*   */
		{ key_93C66_Adr0481 },   /*   */
		{ key_93C66_Adr0482 },   /*   */
		{ key_93C66_Adr0483 },   /*   */
		{ key_93C66_Adr0484 },   /*   */
		{ key_93C66_Adr0485 },   /*   */
		{ key_93C66_Adr0486 },   /*   */
		{ key_93C66_Adr0487 },   /*   */
		{ key_93C66_Adr0488 },   /*   */
		{ key_93C66_Adr0489 },   /*   */
		{ key_93C66_Adr0490 },   /*   */
		{ key_93C66_Adr0491 },   /*   */
		{ key_93C66_Adr0492 },   /*   */
		{ key_93C66_Adr0493 },   /*   */
		{ key_93C66_Adr0494 },   /*   */
		{ key_93C66_Adr0495 },   /*   */
		{ key_93C66_Adr0496 },   /*   */
		{ key_93C66_Adr0497 },   /*   */
		{ key_93C66_Adr0498 },   /*   */
		{ key_93C66_Adr0499 },   /*   */
		{ key_93C66_Adr0500 },   /*   */
		{ key_93C66_Adr0501 },   /*   */
		{ key_93C66_Adr0502 },   /*   */
		{ key_93C66_Adr0503 },   /*   */
		{ key_93C66_Adr0504 },   /*   */
		{ key_93C66_Adr0505 },   /*   */
		{ key_93C66_Adr0506 },   /*   */
		{ key_93C66_Adr0507 },   /*   */
		{ key_93C66_Adr0508 },   /*   */
		{ key_93C66_Adr0509 },   /*   */
		{ key_93C66_Adr0510 },   /*   */
		{ key_93C66_Adr0511 },   /*   */
		{ key_93C66_Adr0512 },   /*   */
		{ key_93C66_Adr0513 },   /*   */
		{ key_93C66_Adr0514 },   /*   */
		{ key_93C66_Adr0515 },   /*   */
		{ key_93C66_Adr0516 },   /*   */
		{ key_93C66_Adr0517 },   /*   */
		{ key_93C66_Adr0518 },   /*   */
		{ key_93C66_Adr0519 },   /*   */
		{ key_93C66_Adr0520 },   /*   */
		{ key_93C66_Adr0521 },   /*   */
		{ key_93C66_Adr0522 },   /*   */
		{ key_93C66_Adr0523 },   /*   */
		{ key_93C66_Adr0524 },   /*   */
		{ key_93C66_Adr0525 },   /*   */
		{ key_93C66_Adr0526 },   /*   */
		{ key_93C66_Adr0527 },   /*   */
		{ key_93C66_Adr0528 },   /*   */
		{ key_93C66_Adr0529 },   /*   */
		{ key_93C66_Adr0530 },   /*   */
		{ key_93C66_Adr0531 },   /*   */
		{ key_93C66_Adr0532 },   /*   */
		{ key_93C66_Adr0533 },   /*   */
		{ key_93C66_Adr0534 },   /*   */
		{ key_93C66_Adr0535 },   /*   */
		{ key_93C66_Adr0536 },   /*   */
		{ key_93C66_Adr0537 },   /*   */
		{ key_93C66_Adr0538 },   /*   */
		{ key_93C66_Adr0539 },   /*   */
		{ key_93C66_Adr0540 },   /*   */
		{ key_93C66_Adr0541 },   /*   */
		{ key_93C66_Adr0542 },   /*   */
		{ key_93C66_Adr0543 },   /*   */
		{ key_93C66_Adr0544 },   /*   */
		{ key_93C66_Adr0545 },   /*   */
		{ key_93C66_Adr0546 },   /*   */
		{ key_93C66_Adr0547 },   /*   */
		{ key_93C66_Adr0548 },   /*   */
		{ key_93C66_Adr0549 },   /*   */
		{ key_93C66_Adr0550 },   /*   */
		{ key_93C66_Adr0551 },   /*   */
		{ key_93C66_Adr0552 },   /*   */
		{ key_93C66_Adr0553 },   /*   */
		{ key_93C66_Adr0554 },   /*   */
		{ key_93C66_Adr0555 },   /*   */
		{ key_93C66_Adr0556 },   /*   */
		{ key_93C66_Adr0557 },   /*   */
		{ key_93C66_Adr0558 },   /*   */
		{ key_93C66_Adr0559 },   /*   */
		{ key_93C66_Adr0560 },   /*   */
		{ key_93C66_Adr0561 },   /*   */
		{ key_93C66_Adr0562 },   /*   */
		{ key_93C66_Adr0563 },   /*   */
		{ key_93C66_Adr0564 },   /*   */
		{ key_93C66_Adr0565 },   /*   */
		{ key_93C66_Adr0566 },   /*   */
		{ key_93C66_Adr0567 },   /*   */
		{ key_93C66_Adr0568 },   /*   */
		{ key_93C66_Adr0569 },   /*   */
		{ key_93C66_Adr0570 },   /*   */
		{ key_93C66_Adr0571 },   /*   */
		{ key_93C66_Adr0572 },   /*   */
		{ key_93C66_Adr0573 },   /*   */
		{ key_93C66_Adr0574 },   /*   */
		{ key_93C66_Adr0575 },   /*   */
		{ key_93C66_Adr0576 },   /*   */
		{ key_93C66_Adr0577 },   /*   */
		{ key_93C66_Adr0578 },   /*   */
		{ key_93C66_Adr0579 },   /*   */
		{ key_93C66_Adr0580 },   /*   */
		{ key_93C66_Adr0581 },   /*   */
		{ key_93C66_Adr0582 },   /*   */
		{ key_93C66_Adr0583 },   /*   */
		{ key_93C66_Adr0584 },   /*   */
		{ key_93C66_Adr0585 },   /*   */
		{ key_93C66_Adr0586 },   /*   */
		{ key_93C66_Adr0587 },   /*   */
		{ key_93C66_Adr0588 },   /*   */
		{ key_93C66_Adr0589 },   /*   */
		{ key_93C66_Adr0590 },   /*   */
		{ key_93C66_Adr0591 },   /*   */
		{ key_93C66_Adr0592 },   /*   */
		{ key_93C66_Adr0593 },   /*   */
		{ key_93C66_Adr0594 },   /*   */
		{ key_93C66_Adr0595 },   /*   */
		{ key_93C66_Adr0596 },   /*   */
		{ key_93C66_Adr0597 },   /*   */
		{ key_93C66_Adr0598 },   /*   */
		{ key_93C66_Adr0599 },   /*   */
		{ key_93C66_Adr0600 },   /*   */
		{ key_93C66_Adr0601 },   /*   */
		{ key_93C66_Adr0602 },   /*   */
		{ key_93C66_Adr0603 },   /*   */
		{ key_93C66_Adr0604 },   /*   */
		{ key_93C66_Adr0605 },   /*   */
		{ key_93C66_Adr0606 },   /*   */
		{ key_93C66_Adr0607 },   /*   */
		{ key_93C66_Adr0608 },   /*   */
		{ key_93C66_Adr0609 },   /*   */
		{ key_93C66_Adr0610 },   /*   */
		{ key_93C66_Adr0611 },   /*   */
		{ key_93C66_Adr0612 },   /*   */
		{ key_93C66_Adr0613 },   /*   */
		{ key_93C66_Adr0614 },   /*   */
		{ key_93C66_Adr0615 },   /*   */
		{ key_93C66_Adr0616 },   /*   */
		{ key_93C66_Adr0617 },   /*   */
		{ key_93C66_Adr0618 },   /*   */
		{ key_93C66_Adr0619 },   /*   */
		{ key_93C66_Adr0620 },   /*   */
		{ key_93C66_Adr0621 },   /*   */
		{ key_93C66_Adr0622 },   /*   */
		{ key_93C66_Adr0623 },   /*   */
		{ key_93C66_Adr0624 },   /*   */
		{ key_93C66_Adr0625 },   /*   */
		{ key_93C66_Adr0626 },   /*   */
		{ key_93C66_Adr0627 },   /*   */
		{ key_93C66_Adr0628 },   /*   */
		{ key_93C66_Adr0629 },   /*   */
		{ key_93C66_Adr0630 },   /*   */
		{ key_93C66_Adr0631 },   /*   */
		{ key_93C66_Adr0632 },   /*   */
		{ key_93C66_Adr0633 },   /*   */
		{ key_93C66_Adr0634 },   /*   */
		{ key_93C66_Adr0635 },   /*   */
		{ key_93C66_Adr0636 },   /*   */
		{ key_93C66_Adr0637 },   /*   */
		{ key_93C66_Adr0638 },   /*   */
		{ key_93C66_Adr0639 },   /*   */
		{ key_93C66_Adr0640 },   /*   */
		{ key_93C66_Adr0641 },   /*   */
		{ key_93C66_Adr0642 },   /*   */
		{ key_93C66_Adr0643 },   /*   */
		{ key_93C66_Adr0644 },   /*   */
		{ key_93C66_Adr0645 },   /*   */
		{ key_93C66_Adr0646 },   /*   */
		{ key_93C66_Adr0647 },   /*   */
		{ key_93C66_Adr0648 },   /*   */
		{ key_93C66_Adr0649 },   /*   */
		{ key_93C66_Adr0650 },   /*   */
		{ key_93C66_Adr0651 },   /*   */
		{ key_93C66_Adr0652 },   /*   */
		{ key_93C66_Adr0653 },   /*   */
		{ key_93C66_Adr0654 },   /*   */
		{ key_93C66_Adr0655 },   /*   */
		{ key_93C66_Adr0656 },   /*   */
		{ key_93C66_Adr0657 },   /*   */
		{ key_93C66_Adr0658 },   /*   */
		{ key_93C66_Adr0659 },   /*   */
		{ key_93C66_Adr0660 },   /*   */
		{ key_93C66_Adr0661 },   /*   */
		{ key_93C66_Adr0662 },   /*   */
		{ key_93C66_Adr0663 },   /*   */
		{ key_93C66_Adr0664 },   /*   */
		{ key_93C66_Adr0665 },   /*   */
		{ key_93C66_Adr0666 },   /*   */
		{ key_93C66_Adr0667 },   /*   */
		{ key_93C66_Adr0668 },   /*   */
		{ key_93C66_Adr0669 },   /*   */
		{ key_93C66_Adr0670 },   /*   */
		{ key_93C66_Adr0671 },   /*   */
		{ key_93C66_Adr0672 },   /*   */
		{ key_93C66_Adr0673 },   /*   */
		{ key_93C66_Adr0674 },   /*   */
		{ key_93C66_Adr0675 },   /*   */
		{ key_93C66_Adr0676 },   /*   */
		{ key_93C66_Adr0677 },   /*   */
		{ key_93C66_Adr0678 },   /*   */
		{ key_93C66_Adr0679 },   /*   */
		{ key_93C66_Adr0680 },   /*   */
		{ key_93C66_Adr0681 },   /*   */
		{ key_93C66_Adr0682 },   /*   */
		{ key_93C66_Adr0683 },   /*   */
		{ key_93C66_Adr0684 },   /*   */
		{ key_93C66_Adr0685 },   /*   */
		{ key_93C66_Adr0686 },   /*   */
		{ key_93C66_Adr0687 },   /*   */
		{ key_93C66_Adr0688 },   /*   */
		{ key_93C66_Adr0689 },   /*   */
		{ key_93C66_Adr0690 },   /*   */
		{ key_93C66_Adr0691 },   /*   */
		{ key_93C66_Adr0692 },   /*   */
		{ key_93C66_Adr0693 },   /*   */
		{ key_93C66_Adr0694 },   /*   */
		{ key_93C66_Adr0695 },   /*   */
		{ key_93C66_Adr0696 },   /*   */
		{ key_93C66_Adr0697 },   /*   */
		{ key_93C66_Adr0698 },   /*   */
		{ key_93C66_Adr0699 },   /*   */
		{ key_93C66_Adr0700 },   /*   */
		{ key_93C66_Adr0701 },   /*   */
		{ key_93C66_Adr0702 },   /*   */
		{ key_93C66_Adr0703 },   /*   */
		{ key_93C66_Adr0704 },   /*   */
		{ key_93C66_Adr0705 },   /*   */
		{ key_93C66_Adr0706 },   /*   */
		{ key_93C66_Adr0707 },   /*   */
		{ key_93C66_Adr0708 },   /*   */
		{ key_93C66_Adr0709 },   /*   */
		{ key_93C66_Adr0710 },   /*   */
		{ key_93C66_Adr0711 },   /*   */
		{ key_93C66_Adr0712 },   /*   */
		{ key_93C66_Adr0713 },   /*   */
		{ key_93C66_Adr0714 },   /*   */
		{ key_93C66_Adr0715 },   /*   */
		{ key_93C66_Adr0716 },   /*   */
		{ key_93C66_Adr0717 },   /*   */
		{ key_93C66_Adr0718 },   /*   */
		{ key_93C66_Adr0719 },   /*   */
		{ key_93C66_Adr0720 },   /*   */
		{ key_93C66_Adr0721 },   /*   */
		{ key_93C66_Adr0722 },   /*   */
		{ key_93C66_Adr0723 },   /*   */
		{ key_93C66_Adr0724 },   /*   */
		{ key_93C66_Adr0725 },   /*   */
		{ key_93C66_Adr0726 },   /*   */
		{ key_93C66_Adr0727 },   /*   */
		{ key_93C66_Adr0728 },   /*   */
		{ key_93C66_Adr0729 },   /*   */
		{ key_93C66_Adr0730 },   /*   */
		{ key_93C66_Adr0731 },   /*   */
		{ key_93C66_Adr0732 },   /*   */
		{ key_93C66_Adr0733 },   /*   */
		{ key_93C66_Adr0734 },   /*   */
		{ key_93C66_Adr0735 },   /*   */
		{ key_93C66_Adr0736 },   /*   */
		{ key_93C66_Adr0737 },   /*   */
		{ key_93C66_Adr0738 },   /*   */
		{ key_93C66_Adr0739 },   /*   */
		{ key_93C66_Adr0740 },   /*   */
		{ key_93C66_Adr0741 },   /*   */
		{ key_93C66_Adr0742 },   /*   */
		{ key_93C66_Adr0743 },   /*   */
		{ key_93C66_Adr0744 },   /*   */
		{ key_93C66_Adr0745 },   /*   */
		{ key_93C66_Adr0746 },   /*   */
		{ key_93C66_Adr0747 },   /*   */
		{ key_93C66_Adr0748 },   /*   */
		{ key_93C66_Adr0749 },   /*   */
		{ key_93C66_Adr0750 },   /*   */
		{ key_93C66_Adr0751 },   /*   */
		{ key_93C66_Adr0752 },   /*   */
		{ key_93C66_Adr0753 },   /*   */
		{ key_93C66_Adr0754 },   /*   */
		{ key_93C66_Adr0755 },   /*   */
		{ key_93C66_Adr0756 },   /*   */
		{ key_93C66_Adr0757 },   /*   */
		{ key_93C66_Adr0758 },   /*   */
		{ key_93C66_Adr0759 },   /*   */
		{ key_93C66_Adr0760 },   /*   */
		{ key_93C66_Adr0761 },   /*   */
		{ key_93C66_Adr0762 },   /*   */
		{ key_93C66_Adr0763 },   /*   */
		{ key_93C66_Adr0764 },   /*   */
		{ key_93C66_Adr0765 },   /*   */
		{ key_93C66_Adr0766 },   /*   */
		{ key_93C66_Adr0767 },   /*   */
		{ key_93C66_Adr0768 },   /*   */
		{ key_93C66_Adr0769 },   /*   */
		{ key_93C66_Adr0770 },   /*   */
		{ key_93C66_Adr0771 },   /*   */
		{ key_93C66_Adr0772 },   /*   */
		{ key_93C66_Adr0773 },   /*   */
		{ key_93C66_Adr0774 },   /*   */
		{ key_93C66_Adr0775 },   /*   */
		{ key_93C66_Adr0776 },   /*   */
		{ key_93C66_Adr0777 },   /*   */
		{ key_93C66_Adr0778 },   /*   */
		{ key_93C66_Adr0779 },   /*   */
		{ key_93C66_Adr0780 },   /*   */
		{ key_93C66_Adr0781 },   /*   */
		{ key_93C66_Adr0782 },   /*   */
		{ key_93C66_Adr0783 },   /*   */
		{ key_93C66_Adr0784 },   /*   */
		{ key_93C66_Adr0785 },   /*   */
		{ key_93C66_Adr0786 },   /*   */
		{ key_93C66_Adr0787 },   /*   */
		{ key_93C66_Adr0788 },   /*   */
		{ key_93C66_Adr0789 },   /*   */
		{ key_93C66_Adr0790 },   /*   */
		{ key_93C66_Adr0791 },   /*   */
		{ key_93C66_Adr0792 },   /*   */
		{ key_93C66_Adr0793 },   /*   */
		{ key_93C66_Adr0794 },   /*   */
		{ key_93C66_Adr0795 },   /*   */
		{ key_93C66_Adr0796 },   /*   */
		{ key_93C66_Adr0797 },   /*   */
		{ key_93C66_Adr0798 },   /*   */
		{ key_93C66_Adr0799 },   /*   */
		{ key_93C66_Adr0800 },   /*   */
		{ key_93C66_Adr0801 },   /*   */
		{ key_93C66_Adr0802 },   /*   */
		{ key_93C66_Adr0803 },   /*   */
		{ key_93C66_Adr0804 },   /*   */
		{ key_93C66_Adr0805 },   /*   */
		{ key_93C66_Adr0806 },   /*   */
		{ key_93C66_Adr0807 },   /*   */
		{ key_93C66_Adr0808 },   /*   */
		{ key_93C66_Adr0809 },   /*   */
		{ key_93C66_Adr0810 },   /*   */
		{ key_93C66_Adr0811 },   /*   */
		{ key_93C66_Adr0812 },   /*   */
		{ key_93C66_Adr0813 },   /*   */
		{ key_93C66_Adr0814 },   /*   */
		{ key_93C66_Adr0815 },   /*   */
		{ key_93C66_Adr0816 },   /*   */
		{ key_93C66_Adr0817 },   /*   */
		{ key_93C66_Adr0818 },   /*   */
		{ key_93C66_Adr0819 },   /*   */
		{ key_93C66_Adr0820 },   /*   */
		{ key_93C66_Adr0821 },   /*   */
		{ key_93C66_Adr0822 },   /*   */
		{ key_93C66_Adr0823 },   /*   */
		{ key_93C66_Adr0824 },   /*   */
		{ key_93C66_Adr0825 },   /*   */
		{ key_93C66_Adr0826 },   /*   */
		{ key_93C66_Adr0827 },   /*   */
		{ key_93C66_Adr0828 },   /*   */
		{ key_93C66_Adr0829 },   /*   */
		{ key_93C66_Adr0830 },   /*   */
		{ key_93C66_Adr0831 },   /*   */
		{ key_93C66_Adr0832 },   /*   */
		{ key_93C66_Adr0833 },   /*   */
		{ key_93C66_Adr0834 },   /*   */
		{ key_93C66_Adr0835 },   /*   */
		{ key_93C66_Adr0836 },   /*   */
		{ key_93C66_Adr0837 },   /*   */
		{ key_93C66_Adr0838 },   /*   */
		{ key_93C66_Adr0839 },   /*   */
		{ key_93C66_Adr0840 },   /*   */
		{ key_93C66_Adr0841 },   /*   */
		{ key_93C66_Adr0842 },   /*   */
		{ key_93C66_Adr0843 },   /*   */
		{ key_93C66_Adr0844 },   /*   */
		{ key_93C66_Adr0845 },   /*   */
		{ key_93C66_Adr0846 },   /*   */
		{ key_93C66_Adr0847 },   /*   */
		{ key_93C66_Adr0848 },   /*   */
		{ key_93C66_Adr0849 },   /*   */
		{ key_93C66_Adr0850 },   /*   */
		{ key_93C66_Adr0851 },   /*   */
		{ key_93C66_Adr0852 },   /*   */
		{ key_93C66_Adr0853 },   /*   */
		{ key_93C66_Adr0854 },   /*   */
		{ key_93C66_Adr0855 },   /*   */
		{ key_93C66_Adr0856 },   /*   */
		{ key_93C66_Adr0857 },   /*   */
		{ key_93C66_Adr0858 },   /*   */
		{ key_93C66_Adr0859 },   /*   */
		{ key_93C66_Adr0860 },   /*   */
		{ key_93C66_Adr0861 },   /*   */
		{ key_93C66_Adr0862 },   /*   */
		{ key_93C66_Adr0863 },   /*   */
		{ key_93C66_Adr0864 },   /*   */
		{ key_93C66_Adr0865 },   /*   */
		{ key_93C66_Adr0866 },   /*   */
		{ key_93C66_Adr0867 },   /*   */
		{ key_93C66_Adr0868 },   /*   */
		{ key_93C66_Adr0869 },   /*   */
		{ key_93C66_Adr0870 },   /*   */
		{ key_93C66_Adr0871 },   /*   */
		{ key_93C66_Adr0872 },   /*   */
		{ key_93C66_Adr0873 },   /*   */
		{ key_93C66_Adr0874 },   /*   */
		{ key_93C66_Adr0875 },   /*   */
		{ key_93C66_Adr0876 },   /*   */
		{ key_93C66_Adr0877 },   /*   */
		{ key_93C66_Adr0878 },   /*   */
		{ key_93C66_Adr0879 },   /*   */
		{ key_93C66_Adr0880 },   /*   */
		{ key_93C66_Adr0881 },   /*   */
		{ key_93C66_Adr0882 },   /*   */
		{ key_93C66_Adr0883 },   /*   */
		{ key_93C66_Adr0884 },   /*   */
		{ key_93C66_Adr0885 },   /*   */
		{ key_93C66_Adr0886 },   /*   */
		{ key_93C66_Adr0887 },   /*   */
		{ key_93C66_Adr0888 },   /*   */
		{ key_93C66_Adr0889 },   /*   */
		{ key_93C66_Adr0890 },   /*   */
		{ key_93C66_Adr0891 },   /*   */
		{ key_93C66_Adr0892 },   /*   */
		{ key_93C66_Adr0893 },   /*   */
		{ key_93C66_Adr0894 },   /*   */
		{ key_93C66_Adr0895 },   /*   */
		{ key_93C66_Adr0896 },   /*   */
		{ key_93C66_Adr0897 },   /*   */
		{ key_93C66_Adr0898 },   /*   */
		{ key_93C66_Adr0899 },   /*   */
		{ key_93C66_Adr0900 },   /*   */
		{ key_93C66_Adr0901 },   /*   */
		{ key_93C66_Adr0902 },   /*   */
		{ key_93C66_Adr0903 },   /*   */
		{ key_93C66_Adr0904 },   /*   */
		{ key_93C66_Adr0905 },   /*   */
		{ key_93C66_Adr0906 },   /*   */
		{ key_93C66_Adr0907 },   /*   */
		{ key_93C66_Adr0908 },   /*   */
		{ key_93C66_Adr0909 },   /*   */
		{ key_93C66_Adr0910 },   /*   */
		{ key_93C66_Adr0911 },   /*   */
		{ key_93C66_Adr0912 },   /*   */
		{ key_93C66_Adr0913 },   /*   */
		{ key_93C66_Adr0914 },   /*   */
		{ key_93C66_Adr0915 },   /*   */
		{ key_93C66_Adr0916 },   /*   */
		{ key_93C66_Adr0917 },   /*   */
		{ key_93C66_Adr0918 },   /*   */
		{ key_93C66_Adr0919 },   /*   */
		{ key_93C66_Adr0920 },   /*   */
		{ key_93C66_Adr0921 },   /*   */
		{ key_93C66_Adr0922 },   /*   */
		{ key_93C66_Adr0923 },   /*   */
		{ key_93C66_Adr0924 },   /*   */
		{ key_93C66_Adr0925 },   /*   */
		{ key_93C66_Adr0926 },   /*   */
		{ key_93C66_Adr0927 },   /*   */
		{ key_93C66_Adr0928 },   /*   */
		{ key_93C66_Adr0929 },   /*   */
		{ key_93C66_Adr0930 },   /*   */
		{ key_93C66_Adr0931 },   /*   */
		{ key_93C66_Adr0932 },   /*   */
		{ key_93C66_Adr0933 },   /*   */
		{ key_93C66_Adr0934 },   /*   */
		{ key_93C66_Adr0935 },   /*   */
		{ key_93C66_Adr0936 },   /*   */
		{ key_93C66_Adr0937 },   /*   */
		{ key_93C66_Adr0938 },   /*   */
		{ key_93C66_Adr0939 },   /*   */
		{ key_93C66_Adr0940 },   /*   */
		{ key_93C66_Adr0941 },   /*   */
		{ key_93C66_Adr0942 },   /*   */
		{ key_93C66_Adr0943 },   /*   */
		{ key_93C66_Adr0944 },   /*   */
		{ key_93C66_Adr0945 },   /*   */
		{ key_93C66_Adr0946 },   /*   */
		{ key_93C66_Adr0947 },   /*   */
		{ key_93C66_Adr0948 },   /*   */
		{ key_93C66_Adr0949 },   /*   */
		{ key_93C66_Adr0950 },   /*   */
		{ key_93C66_Adr0951 },   /*   */
		{ key_93C66_Adr0952 },   /*   */
		{ key_93C66_Adr0953 },   /*   */
		{ key_93C66_Adr0954 },   /*   */
		{ key_93C66_Adr0955 },   /*   */
		{ key_93C66_Adr0956 },   /*   */
		{ key_93C66_Adr0957 },   /*   */
		{ key_93C66_Adr0958 },   /*   */
		{ key_93C66_Adr0959 },   /*   */
		{ key_93C66_Adr0960 },   /*   */
		{ key_93C66_Adr0961 },   /*   */
		{ key_93C66_Adr0962 },   /*   */
		{ key_93C66_Adr0963 },   /*   */
		{ key_93C66_Adr0964 },   /*   */
		{ key_93C66_Adr0965 },   /*   */
		{ key_93C66_Adr0966 },   /*   */
		{ key_93C66_Adr0967 },   /*   */
		{ key_93C66_Adr0968 },   /*   */
		{ key_93C66_Adr0969 },   /*   */
		{ key_93C66_Adr0970 },   /*   */
		{ key_93C66_Adr0971 },   /*   */
		{ key_93C66_Adr0972 },   /*   */
		{ key_93C66_Adr0973 },   /*   */
		{ key_93C66_Adr0974 },   /*   */
		{ key_93C66_Adr0975 },   /*   */
		{ key_93C66_Adr0976 },   /*   */
		{ key_93C66_Adr0977 },   /*   */
		{ key_93C66_Adr0978 },   /*   */
		{ key_93C66_Adr0979 },   /*   */
		{ key_93C66_Adr0980 },   /*   */
		{ key_93C66_Adr0981 },   /*   */
		{ key_93C66_Adr0982 },   /*   */
		{ key_93C66_Adr0983 },   /*   */
		{ key_93C66_Adr0984 },   /*   */
		{ key_93C66_Adr0985 },   /*   */
		{ key_93C66_Adr0986 },   /*   */
		{ key_93C66_Adr0987 },   /*   */
		{ key_93C66_Adr0988 },   /*   */
		{ key_93C66_Adr0989 },   /*   */
		{ key_93C66_Adr0990 },   /*   */
		{ key_93C66_Adr0991 },   /*   */
		{ key_93C66_Adr0992 },   /*   */
		{ key_93C66_Adr0993 },   /*   */
		{ key_93C66_Adr0994 },   /*   */
		{ key_93C66_Adr0995 },   /*   */
		{ key_93C66_Adr0996 },   /*   */
		{ key_93C66_Adr0997 },   /*   */
		{ key_93C66_Adr0998 },   /*   */
		{ key_93C66_Adr0999 },   /*   */
		{ key_93C66_Adr1000 },   /*   */
		{ key_93C66_Adr1001 },   /*   */
		{ key_93C66_Adr1002 },   /*   */
		{ key_93C66_Adr1003 },   /*   */
		{ key_93C66_Adr1004 },   /*   */
		{ key_93C66_Adr1005 },   /*   */
		{ key_93C66_Adr1006 },   /*   */
		{ key_93C66_Adr1007 },   /*   */
		{ key_93C66_Adr1008 },   /*   */
		{ key_93C66_Adr1009 },   /*   */
		{ key_93C66_Adr1010 },   /*   */
		{ key_93C66_Adr1011 },   /*   */
		{ key_93C66_Adr1012 },   /*   */
		{ key_93C66_Adr1013 },   /*   */
		{ key_93C66_Adr1014 },   /*   */
		{ key_93C66_Adr1015 },   /*   */
		{ key_93C66_Adr1016 },   /*   */
		{ key_93C66_Adr1017 },   /*   */
		{ key_93C66_Adr1018 },   /*   */
		{ key_93C66_Adr1019 },   /*   */
		{ key_93C66_Adr1020 },   /*   */
		{ key_93C66_Adr1021 },   /*   */
		{ key_93C66_Adr1022 },   /*   */
		{ key_93C66_Adr1023 },   /*   */
		{ key_93C66_Adr1024 },   /*   */


};





#define STORAGE_NAMESPACE "storage"
#define TAG "eeprom-board"

uint8_t EepromMcuWriteBuffer( uint16_t addr, uint8_t *buffer, uint16_t size )
{

    ESP_LOGI(TAG, "EepromMcuWriteBuffer, addr = %i", addr);



    nvs_handle_t my_handle;
    esp_err_t err_open;

    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    err_open = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err_open != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_open));
    } else {
        printf("Done\n");
    }




    esp_err_t err_set_blob;
    err_set_blob = nvs_set_blob(my_handle, nvsAdr[addr].key, buffer, size);
    printf((err_set_blob != ESP_OK) ? "Failed!\n" : "Done\n");


    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    printf("Committing updates in NVS ... ");
    esp_err_t err_commit = nvs_commit(my_handle);
    printf((err_commit != ESP_OK) ? "Failed!\n" : "Done\n");

    // Close
    nvs_close(my_handle);

    if ((err_open == ESP_OK) && (err_set_blob == ESP_OK) && (err_commit == ESP_OK))
    {
    	return SUCCESS;
    }
    else
    {
    	return FAIL;
    }

}

uint8_t EepromMcuReadBuffer( uint16_t addr, uint8_t *buffer, uint16_t size )
{

    ESP_LOGI(TAG, "EepromMcuReadBuffer, addr = %i", addr);

    nvs_handle_t my_handle;
    esp_err_t err_open;

    // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    err_open = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
    if (err_open != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err_open));
    } else {
        printf("Done\n");
    }
    if (err_open != ESP_OK) return FAIL;


    esp_err_t err_get_blob;
    size_t mysize = size;
    size_t* length = &mysize;
    err_get_blob = nvs_get_blob(my_handle, nvsAdr[addr].key, buffer, length);
    switch (err_get_blob) {
        case ESP_OK:
            printf("Done\n");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err_get_blob));
    }

    // Close
    nvs_close(my_handle);

    if  ((err_open == ESP_OK) && (err_get_blob == ESP_OK))
    {
    	return SUCCESS;
    }
    else
    {
    	return FAIL;
    }
}

void EepromMcuSetDeviceAddr( uint8_t addr )
{

}

uint8_t EepromMcuGetDeviceAddr( void )
{
    return 0;
}
