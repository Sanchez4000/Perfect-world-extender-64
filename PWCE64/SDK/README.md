# PWCE64 SDK

Шаблон проекта плагина PWCE64.

## Состав

- `include/types.h` — типы `int8`…`int64`, `uint8`…`uint64`, `single`, `uintptr` (64 бита); подключает `<stddef.h>` (`NULL`, `size_t`).
- `include/pwce.h` — `PwceApi`, `PWCE_EXPORT` и точка входа `PwcePluginLoad`.
- `include/field.h` — `field_at(object, offset)`: указатель на поле структуры клиента по смещению.
- `src/plugin.c` — шаблон плагина.
- `build.bat` — сборка `src/plugin.c` → `build/<имя_папки>.dll` (`gcc` из mingw-w64 x86-64).

## Создание плагина

1. Скопировать папку `SDK` под именем плагина.
2. Реализовать `src/plugin.c`.
3. Запустить `build.bat` → `build/<ИмяПлагина>.dll`. Имя `.dll` — имя папки.
4. Скопировать `.dll` в `element/x64/plugins/` и перезапустить клиент.

Нужные функции клиента должны быть в `plugins\_addresses.ini`
(`../Addresses/<версия игры>/_addresses.ini`). Формат и правила — `../README.md`.

## Стиль кода

Те же правила, что у PWSE64: чистый C, стандарт `gnu11`, раздел «Стиль кода» в `../../PWSE64/SDK/README.md`.

Отличия платформы:

- Точка входа и всё, что должно быть видно загрузчику, объявляется с `PWCE_EXPORT`.
- `uintptr` — `unsigned long long` (64 бита).
- Функции клиента вызываются по соглашению Microsoft x64, отдельных атрибутов не требуется.
