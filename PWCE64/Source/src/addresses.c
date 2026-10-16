#include "addresses.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define ADDRESS_COUNT_MAX 1024
#define NAME_LENGTH_MAX   192
#define LINE_LENGTH_MAX   1024

typedef struct AddressEntry AddressEntry;

struct AddressEntry
{
    char name[NAME_LENGTH_MAX];
    uintptr address;
};

static AddressEntry entries[ADDRESS_COUNT_MAX];
static int32 entry_count = 0;
static uintptr module_base = 0;
static uintptr image_base = 0;

static uintptr read_image_base(uintptr module)
{
    const IMAGE_DOS_HEADER* dos = (const IMAGE_DOS_HEADER*)module;
    const IMAGE_NT_HEADERS64* headers = (const IMAGE_NT_HEADERS64*)(module + dos->e_lfanew);

    return headers->OptionalHeader.ImageBase;
}

static char* skip_whitespace(char* text)
{
    while (*text == ' ' || *text == '\t')
        text++;

    return text;
}

static void strip_trailing(char* text)
{
    size_t length = strlen(text);

    while (length > 0 && (text[length - 1] == ' ' || text[length - 1] == '\t' || text[length - 1] == '\r' || text[length - 1] == '\n'))
        text[--length] = 0;
}

static bool is_ignored(const char* text)
{
    return *text == 0 || *text == '#' || *text == ';' || *text == '\r' || *text == '\n';
}

static void add_entry(const char* name, uintptr address)
{
    const size_t length = strlen(name);

    if (entry_count >= ADDRESS_COUNT_MAX || length >= NAME_LENGTH_MAX)
        return;

    memcpy(entries[entry_count].name, name, length + 1);
    entries[entry_count].address = address;
    entry_count++;
}

static void parse_line(char* line)
{
    char* text = skip_whitespace(line);

    if (is_ignored(text))
        return;

    char* separator = strchr(text, '=');

    if (separator == NULL)
        return;

    *separator = 0;
    strip_trailing(text);

    const uintptr address = strtoull(skip_whitespace(separator + 1), NULL, 0);

    if (address == 0)
        return;

    add_entry(text, address);
}

int32 pwce_addresses_load(const char* path)
{
    FILE* file = fopen(path, "r");

    if (file == NULL)
        return -1;

    // Addresses in the library are virtual addresses at the preferred image base; the module may be mapped elsewhere.
    module_base = (uintptr)GetModuleHandleA(NULL);
    image_base = read_image_base(module_base);
    entry_count = 0;

    char line[LINE_LENGTH_MAX];

    while (fgets(line, sizeof(line), file) != NULL)
        parse_line(line);

    fclose(file);

    return entry_count;
}

uintptr pwce_addresses_resolve(const char* name)
{
    if (name == NULL)
        return 0;

    for (int32 i = 0; i < entry_count; i++)
        if (strcmp(entries[i].name, name) == 0)
            return module_base + entries[i].address - image_base;

    return 0;
}
