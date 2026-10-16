# PWSE64 SDK

Шаблон проекта плагина PWSE64.

## Состав

- `include/types.h` — типы `int8`…`int64`, `uint8`…`uint64`, `single`, `uintptr`; подключает `<stddef.h>` (`NULL`, `size_t`).
- `include/pwse.h` — `PwseApi` и точка входа `PwsePluginLoad`.
- `include/field.h` — `field_at(object, offset)`: указатель на поле структуры движка по смещению.
- `src/plugin.c` — шаблон плагина.
- `toolchain/Dockerfile` — образ сборки `pwse64-toolchain`.
- `build.bat` — сборка `src/*.c` → `build/<имя_папки>.so`.

## Создание плагина

1. Скопировать папку `SDK` под именем плагина.
2. Реализовать `src/plugin.c`.
3. Запустить `build.bat` → `build/<ИмяПлагина>.so`. Имя `.so` — имя папки.
4. Скопировать `.so` в `gamed/plugins/` и перезапустить карту.

Нужные функции движка должны быть в `plugins/_addresses.ini`
(`../Addresses/<версия игры>/_addresses.ini`). Формат и правила — `../README.md`.

## Стиль кода

Язык — чистый C, стандарт `gnu11` (`gcc -std=gnu11`).

- Константы — `#define ИМЯ значение`.
- Структуры объявляются через `typedef struct Name Name;`, затем определяются `struct Name { ... };`. Дальше используется имя `Name`.
- `typedef` также для типов-синонимов (`types.h`) и типов функций движка.
- Структуры инициализируются обозначенными инициализаторами: `{ .text = message, .length = length }`.
- `NULL`, `bool` из `<stdbool.h>`. Функции без аргументов объявляются как `(void)`. Параметры функций всегда с именами.
- Функция принимает не больше трёх аргументов. Сигнатуры хуков задаёт движок, внутри плагина аргументы объединяются в структуру.
- Функции чистые, где это возможно. Изменяемое состояние только в хуках: указатели на оригиналы и разрешённые функции движка.
- Функции как данные: таблицы указателей на функции, массивы `Type name[]`.
- Результат: указатель или `NULL`, либо код `0` / `-1`.
- Комментарии только для неочевидных значений и вызовов. Комментарии и логи на английском.

Частичное описание структур движка: описываются только известные поля, доступ по смещению.

```c
#define PARENT_CHILD_OFFSET 0x10
#define CHILD_VALUE_OFFSET  0x20

static void* parent_child(void* parent)
{
    return *(void**)field_at(parent, PARENT_CHILD_OFFSET);
}

static int32 parent_child_value(void* parent)
{
    return *(int32*)field_at(parent_child(parent), CHILD_VALUE_OFFSET);
}
```

Таблица команд из указателей на функции:

```c
typedef struct Command Command;

struct Command
{
    const char* name;
    void (*run)(void* player, const char* argument);
};

static void command_first(void* player, const char* argument);
static void command_second(void* player, const char* argument);

static const Command COMMANDS[] =
{
    { "first", command_first },
    { "second", command_second },
};

#define COMMAND_COUNT (sizeof(COMMANDS) / sizeof(COMMANDS[0]))
```
