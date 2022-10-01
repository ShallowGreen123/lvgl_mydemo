/************************************************************************
 * FilePath     : drv_log.c
 * Author       : GX.Duan
 * Date         : 2022-09-21 22:22:33
 * LastEditTime : 2022-09-30 22:29:58
 * LastEditors  : ShallowGreen123 2608653986@qq.com
 * Copyright (c): 2022 by GX.Duan, All Rights Reserved.
 * Github       : https://github.com/ShallowGreen123/lvgl_mydemo
 ************************************************************************/
#define __DRV_LOG_C__
/*********************************************************************************
 *                                  INCLUDES
 * *******************************************************************************/
#include "drv_log.h"

/*********************************************************************************
 *                                   DEFINES
 * *******************************************************************************/

/*********************************************************************************
 *                                   MACROS
 * *******************************************************************************/

/*********************************************************************************
 *                                  TYPEDEFS
 * *******************************************************************************/

/*********************************************************************************
 *                              STATIC FUNCTION
 * *******************************************************************************/

/*********************************************************************************
 *                              GLOBAL FUNCTION
 * *******************************************************************************/
void Drv_LogPrintf(void *str)
{
    printf("\033[1;32m%s\n", (uint8_t *)str);
}

void Drv_LogColorPrintf(void *str, void *prefix)
{
    char  type      = *((char *)prefix + 1);
    char *color_str = NULL;

    /* clang-format off */
    switch (type) {
        case 'V': printf(RTT_CTRL_TEXT_BRIGHT_YELLOW); break;
        case 'D': printf(RTT_CTRL_TEXT_BRIGHT_GREEN); break;
        case 'I': printf(RTT_CTRL_TEXT_BRIGHT_WHITE); break;
        case 'W': printf(RTT_CTRL_TEXT_BRIGHT_YELLOW); break;
        case 'E': printf(RTT_CTRL_TEXT_BRIGHT_RED); break;
        case 'X' :printf(RTT_CTRL_TEXT_BRIGHT_MAGENTA); break;
        default:
            break;
    }
    /* clang-format on */
    printf("%s\n", (uint8_t *)str);
    printf(RTT_CTRL_RESET);
}
