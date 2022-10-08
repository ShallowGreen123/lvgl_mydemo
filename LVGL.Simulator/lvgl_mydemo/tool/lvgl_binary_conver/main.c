#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

static char *get_file_name(const char *path)
{
    size_t len   = strlen(path);
    size_t start = 0;
    size_t end   = len;
    char  *file_name;

    for (size_t i = 0; i < len; i++) {
        if (path[i] == '\\') {
            start = i + 1;
        }
    }
    for (size_t i = len - 1; i > start; i--) {
        if (path[i] == '.') {
            end = i;
        }
    }

    if (start >= end) {
        return NULL;
    }

    file_name = (char *)malloc(end - start + 1);
    memcpy(file_name, path + start, end - start);
    file_name[end - start] = '\0';
    return file_name;
}

static char *str_trim(const char *s)
{
    size_t len = strlen(s);
    size_t start;
    size_t end;
    char  *ts;
    char   c;

    for (start = 0; start < len; start++) {
        c = s[start];
        if (c != ' ' && c != '\t') {
            break;
        }
    }
    for (end = len; end > start; end--) {
        c = s[end - 1];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            break;
        }
    }

    ts = (char *)malloc(end - start + 1);
    memcpy(ts, s + start, end - start);
    ts[end - start] = '\0';
    return ts;
}

static char *str_replace(const char *s, const char *origin, const char *replace)
{
    char  *find        = strstr(s, origin);
    size_t s_len       = strlen(s);
    size_t origin_len  = strlen(origin);
    size_t replace_len = strlen(replace);
    size_t len         = s_len - origin_len + replace_len + 1;
    char  *ns;
    size_t pre, sub;

    if (find) {
        ns  = (char *)malloc(len);
        pre = find - s;
        sub = s_len - pre - origin_len;
        if (pre > 0) {
            memcpy(ns, s, pre);
        }
        memcpy(ns + pre, replace, replace_len);
        if (sub > 0) {
            memcpy(ns + pre + replace_len, s + s_len - sub, sub);
        }
        ns[len - 1] = '\0';
        return ns;
    }
    return (char *)s;
}
static void str_uppercase(char *s)
{
    size_t len = strlen(s);
    size_t i;
    int8_t diff = 'A' - 'a';

    for (i = 0; i < len; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] += diff;
        }
    }
}

static uint8_t hex_2_u8(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return 0;
}

static void process_glyph_bitmap(FILE *in, FILE *trim_file, const char *out_dir, const char *file_name)
{
    char    line_buf[1024];
    char   *line;
    FILE   *bin_file;
    char    bin_file_name[512];
    size_t  len;
    size_t  pos;
    uint8_t hex_buf[2];
    uint8_t n;

    snprintf(bin_file_name, sizeof(bin_file_name), "%s\\%s_glyph_bitmap.bin", out_dir, file_name);
    bin_file = fopen(bin_file_name, "wb+");
    if (!bin_file) {
        return;
    }
    printf("%% glyph bitmap\n");
    while ((line = fgets(line_buf, sizeof(line_buf), in)) != NULL) {
        fprintf(trim_file, "%s", line);
        line = str_trim(line);
        len  = strlen(line);
        if (len == 1 && line[0] == '{') {
            free(line);
            continue;
        }
        if (line[0] == '/' && line[1] == '*') {
            if (line[len - 2] == '*' && line[len - 1] == '/') {
                free(line);
                continue;
            } else {
                printf("[E] line format error: %s\n", line);
                free(line);
                continue;
            }
        }

        pos = 0;
        while (pos < len) {
            if (line[pos] == '0' && line[pos + 1] == 'x') {
                pos += 2;
                if ((pos + 1) == len || line[pos + 1] == ',') {
                    hex_buf[0] = 0;
                    hex_buf[1] = hex_2_u8(line[pos]);
                    n          = hex_buf[1];
                    fwrite(&n, 1, 1, bin_file);
                    pos += 2;
                    continue;
                } else if ((pos + 2) == len || line[pos + 2] == ',') {
                    hex_buf[0] = hex_2_u8(line[pos]);
                    hex_buf[1] = hex_2_u8(line[pos + 1]);
                    n          = hex_buf[0] * 16 + hex_buf[1];
                    fwrite(&n, 1, 1, bin_file);
                    pos += 3;
                    continue;
                } else {
                    printf("[E] hex string parse fail: %s (%" PRIuPTR ")\n", line, pos);
                }
            }
            pos++;
        }

        if (line[len - 1] == ';') {
            free(line);
            break;
        }
        free(line);
    }
    fclose(bin_file);
}

static void process_kern_class_values(FILE *in, FILE *trim_file, const char *out_dir, const char *file_name)
{
    char    line_buf[1024];
    char   *line;
    FILE   *bin_file;
    char    bin_file_name[512];
    size_t  len;
    size_t  pos;
    char    int_buf[12];
    size_t  int_buf_pos;
    uint8_t n;

    snprintf(bin_file_name, sizeof(bin_file_name), "%s\\%s_kern_class_values.bin", out_dir, file_name);
    bin_file = fopen(bin_file_name, "wb+");
    if (!bin_file) {
        return;
    }

    printf("%% kern class values\n");
    while ((line = fgets(line_buf, sizeof(line_buf), in)) != NULL) {
        fprintf(trim_file, "%s", line);
        line = str_trim(line);
        len  = strlen(line);

        if (len == 1 && line[0] == '{') {
            free(line);
            continue;
        }
        if (line[0] == '/' && line[1] == '*') {
            if (line[len - 2] == '*' && line[len - 1] == '/') {
                free(line);
                continue;
            } else {
                printf("[E] line format error: %s\n", line);
                free(line);
                continue;
            }
        }

        pos = 0;
        memset(int_buf, 0, sizeof(int_buf));
        int_buf_pos = 0;
        while (pos < len) {
            if ((line[pos] == ',' || pos == len - 1) && int_buf_pos > 0) {
                n = (uint8_t)atoi(int_buf);
                fwrite(&n, 1, 1, bin_file);
                memset(int_buf, 0, sizeof(int_buf));
                int_buf_pos = 0;
            } else {
                int_buf[int_buf_pos++] = line[pos];
                if (int_buf_pos >= sizeof(int_buf)) {
                    printf("[E] int string parse fail: %s (%" PRIuPTR ")\n", line, pos);
                    memset(int_buf, 0, sizeof(int_buf));
                    int_buf_pos = 0;
                }
            }
            pos++;
        }
        if (line[len - 1] == ';') {
            free(line);
            break;
        }
        free(line);
    }
    fclose(bin_file);
}

static void process_one_file(const char *path, const char *out_dir)
{
    FILE *file;
    FILE *trim_file;
    char  trim_file_name[512];
    char  line_buf[1024];
    char  replace_buf[256];
    char *line;
    char *trim_line;
    char *replace_line;
    char *file_name = get_file_name(path);

    if (file_name == NULL) {
        printf("Path err: %s\n", path);
        return;
    }
    printf("\nStart process: %s\n", file_name);

    file = fopen(path, "r");
    if (!file) {
        return;
    }

    snprintf(trim_file_name, sizeof(trim_file_name), "%s\\%s.c", out_dir, file_name);
    trim_file = fopen(trim_file_name, "w+");
    if (!trim_file) {
        return;
    }

    line = fgets(line_buf, sizeof(line_buf), file);
    while (line != NULL) {
        trim_line = str_trim(line);
        if (strcmp(trim_line, "#ifdef LV_LVGL_H_INCLUDE_SIMPLE") == 0) {
            fprintf(trim_file, "#include \"../bp_font_addr.h\"\n\n");
        } else if (strcmp(trim_line, "static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {") == 0) {
            fprintf(trim_file, "#ifdef DEF_BP_FONT_AT_EXT_FLASH\n");
            snprintf(replace_buf, sizeof(replace_buf), "%s_glyph_bitmap", file_name);
            str_uppercase(replace_buf);
            fprintf(trim_file, "// %s\n", replace_buf);
            fprintf(trim_file, "#else\n");
            fprintf(trim_file, "%s", line);
            process_glyph_bitmap(file, trim_file, out_dir, file_name);
            fprintf(trim_file, "#endif /* DEF_BP_FONT_AT_EXT_FLASH */\n");
            free(trim_line);
            line = fgets(line_buf, sizeof(line_buf), file);
            continue;
        } else if (strcmp(trim_line, "static const int8_t kern_class_values[] =") == 0) {
            fprintf(trim_file, "#ifdef DEF_BP_FONT_AT_EXT_FLASH\n");
            snprintf(replace_buf, sizeof(replace_buf), "%s_kern_class_values", file_name);
            str_uppercase(replace_buf);
            fprintf(trim_file, "// %s\n", replace_buf);
            fprintf(trim_file, "#else\n");
            fprintf(trim_file, "%s", line);
            process_kern_class_values(file, trim_file, out_dir, file_name);
            fprintf(trim_file, "#endif /* DEF_BP_FONT_AT_EXT_FLASH */\n");
            free(trim_line);
            line = fgets(line_buf, sizeof(line_buf), file);
            continue;
        }
        free(trim_line);

        str_uppercase(file_name);
        memset(replace_buf, 0, sizeof(replace_buf));
        strcat(replace_buf, " (const uint8_t *)(");
        strcat(replace_buf, file_name);
        strcat(replace_buf, "_GLYPH_BITMAP)");
        replace_line = str_replace(line, " glyph_bitmap", replace_buf);
        if (replace_line == line) {
            memset(replace_buf, 0, sizeof(replace_buf));
            strcat(replace_buf, " (const int8_t *)(");
            strcat(replace_buf, file_name);
            strcat(replace_buf, "_KERN_CLASS_VALUES)");
            replace_line = str_replace(line, " kern_class_values", replace_buf);
        }
        if (replace_line != line) {
            fprintf(trim_file, "%s", replace_line);
            free(replace_line);
        } else {
            fprintf(trim_file, "%s", line);
        }
        line = fgets(line_buf, sizeof(line_buf), file);
    }
    free(file_name);
    fclose(trim_file);
    fclose(file);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage:\n    %s <font.c> <output dir>\n", argv[0]);
        return 1;
    } else {
        process_one_file(argv[1], argv[2]);
    }
    printf("Done\n");

    return 0;
}
