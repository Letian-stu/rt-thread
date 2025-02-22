#include "dev_version.h"
#include <stdio.h>
#include <string.h>

static const VersionEntry version_history[] = 
{
    {
        .date = "2025-02-22 23:03",
        .author = "Letian",
        .version = "1.0.0",
        .change_type = "first init",
        .description = "first init"
    },
    {
        .date = "2025-02-23 00:03",
        .author = "Letian",
        .version = "1.0.1",
        .change_type = "debug",
        .description = "debug"
    }    
};

const VersionEntry* get_version_history(void) 
{
    return version_history;
}

uint8_t get_version_count(void) 
{
    return sizeof(version_history)/sizeof(VersionEntry);
}

const int col_widths[] = {5, 12, 20, 15, 15, 30};

void print_separator() {
    putchar('+');
    for(int i = 0; i < sizeof(col_widths)/sizeof(int); i++ )
    {
        for(int j = 0; j < col_widths[i]; j++) 
            putchar('-');
        putchar('+');
    }
    putchar('\n');
}

void log_version_info(void) 
{
    const char* headers[] = {"No.", "Version", "Date", "Author", "Type", "Description"};

    print_separator();
    printf("|%-*s|%-*s|%-*s|%-*s|%-*s|%-*s|\n", col_widths[0], headers[0], col_widths[1], headers[1], col_widths[2], headers[2],
                                                col_widths[3], headers[3], col_widths[4], headers[4], col_widths[5], headers[5]);
    print_separator();

    for(uint8_t i = 0; i < get_version_count(); i++)
    {
        const VersionEntry* e = &version_history[i];
        char truncated_desc[31];
        strncpy(truncated_desc, e->description, 30);
        truncated_desc[30] = '\0';
        printf("|%-5d|%-12.12s|%-20.20s|%-15.15s|%-15.15s|%-30.30s|\n",i+1,e->version,e->date,e->author,e->change_type,truncated_desc);
        print_separator();
    }
}

