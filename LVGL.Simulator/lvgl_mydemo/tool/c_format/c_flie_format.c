/**
 * @file c_flie_format.c
 * @author Gx.Duan
 * @brief C File format generation
 *
 * method of application:
 *
 * 1.Run [a.exe] and input
 * [file name] to generate the file in the current path.
 *
 * 2.Run [a.exe] [file path] + '\' and
 * input [file name] to generate the file in the specified path.
 *
 * @version 0.1
 * @date 2022-10-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEF_MAX_FILE_NAME_LEN 32
#define DEF_MAX_FILE_PATH_LEN 128

char file_name[DEF_MAX_FILE_NAME_LEN];
char file_path[DEF_MAX_FILE_PATH_LEN];
char file_c[DEF_MAX_FILE_PATH_LEN];
char file_h[DEF_MAX_FILE_PATH_LEN];

//
void WriteCFile(FILE *fp_c);
void WriteHFile(FILE *fc_h);

int  main(int argc, char **argv)
{
    FILE *fp_c = NULL;
    FILE *fp_h = NULL;

    if (argv[1] != NULL) {
        strcpy(file_path, argv[1]);
    } else {
        strcpy(file_path, "./");
    }

    printf("intput file name : ");
    scanf("%s", file_name);

    sprintf(file_path, "%s%s", file_path, file_name);

    WriteCFile(fp_c);
    WriteHFile(fp_h);

    fclose(fp_c);
    fclose(fp_h);
}

void WriteCFile(FILE *fp_c)
{
    int  i;
    char name_upper[DEF_MAX_FILE_NAME_LEN];
    char name_lower[DEF_MAX_FILE_NAME_LEN];

    sprintf(file_c, "%s%s", file_path, ".c");
    fp_c = fopen(file_c, "w+");

    for (i = 0; i < strlen(file_name); i++) {
        name_upper[i] = toupper(file_name[i]);
    }
    for (i = 0; i < strlen(file_name); i++) {
        name_lower[i] = tolower(file_name[i]);
    }

    fprintf(fp_c, "\n#define DEF_%s_MODULE \n", name_upper);
    fprintf(fp_c, "/*********************************************************************************   \n");
    fprintf(fp_c, " *                                   INCLUDES                                        \n");
    fprintf(fp_c, " *********************************************************************************/  \n");
    fprintf(fp_c, "#include \"%s.h\" \n\n", name_lower);
    fprintf(fp_c, "/*********************************************************************************   \n");
    fprintf(fp_c, " *                                   DEFINES                                         \n");
    fprintf(fp_c, " *********************************************************************************/  \n\n");
    fprintf(fp_c, "/*********************************************************************************   \n");
    fprintf(fp_c, " *                                   MACROS                                          \n");
    fprintf(fp_c, " *********************************************************************************/  \n\n");
    fprintf(fp_c, "/*********************************************************************************   \n");
    fprintf(fp_c, " *                                  TYPEDEFS                                         \n");
    fprintf(fp_c, " *********************************************************************************/  \n\n");
    fprintf(fp_c, "/*********************************************************************************   \n");
    fprintf(fp_c, " *                               STATIC FUNCTION                                     \n");
    fprintf(fp_c, " *********************************************************************************/  \n\n");
    fprintf(fp_c, "/*********************************************************************************   \n");
    fprintf(fp_c, " *                               GLOBAL FUNCTION                                     \n");
    fprintf(fp_c, " *********************************************************************************/  \n\n");
}

void WriteHFile(FILE *fp_h)
{
    int  i = 0;
    char name_upper[DEF_MAX_FILE_NAME_LEN];
    char name_lower[DEF_MAX_FILE_NAME_LEN];

    sprintf(file_h, "%s%s", file_path, ".h");
    fp_h = fopen(file_h, "w+");

    for (i = 0; i < strlen(file_name); i++) {
        name_upper[i] = toupper(file_name[i]);
    }
    for (i = 0; i < strlen(file_name); i++) {
        name_lower[i] = tolower(file_name[i]);
    }

    fprintf(fp_h, "\n#ifndef __%s_H__ \n", name_upper);
    fprintf(fp_h, "#define __%s_H__ \n", name_upper);
    fprintf(fp_h, "/*********************************************************************************   \n");
    fprintf(fp_h, " *                                   INCLUDES                                        \n");
    fprintf(fp_h, " *********************************************************************************/  \n\n");
    fprintf(fp_h, "/*********************************************************************************   \n");
    fprintf(fp_h, " *                                   DEFINES                                         \n");
    fprintf(fp_h, " *********************************************************************************/  \n");

    fprintf(fp_h, "#ifdef DEF_%s_MODULE\n", name_upper);
    fprintf(fp_h, "#    define DEF_%s_MODULE\n", name_upper);
    fprintf(fp_h, "#else \n");
    fprintf(fp_h, "#    define DEF_%s_MODULE extern\n", name_upper);
    fprintf(fp_h, "#endif\n\n");

    fprintf(fp_h, "/*********************************************************************************   \n");
    fprintf(fp_h, " *                                   MACROS                                          \n");
    fprintf(fp_h, " *********************************************************************************/  \n\n");
    fprintf(fp_h, "/*********************************************************************************   \n");
    fprintf(fp_h, " *                                  TYPEDEFS                                         \n");
    fprintf(fp_h, " *********************************************************************************/  \n\n");
    fprintf(fp_h, "/*********************************************************************************   \n");
    fprintf(fp_h, " *                               GLOBAL FUNCTION                                     \n");
    fprintf(fp_h, " *********************************************************************************/  \n\n");
    fprintf(fp_h, "\n#endif /* __%s_H__ */ \n", name_upper);
}
