# PWE64 — Perfect World Extender 64

Загрузчики плагинов для Perfect World 1.7.6. Плагины добавляют или меняют поведение игры
отдельными библиотеками, исполняемые файлы игры не изменяются.

| Компонент | Расшифровка | Цель |
|---|---|---|
| [`PWSE64`](PWSE64/README.md) | Perfect World Server Extender 64 | игровой сервер `gs` (ELF64 PIE x86-64, Linux) |
| [`PWCE64`](PWCE64/README.md) | Perfect World Client Extender 64 | клиент `elementclient_64.exe` (PE32+ x86-64, Windows) |

## Структура

```
PWE64/
  PWSE64/
    Source/           Исходники загрузчика → pwse_loader.so
    SDK/              Шаблон проекта плагина
    Release/          Готовые сборки по версиям
    Addresses/<версия игры>/_addresses.ini
  PWCE64/
    Source/           Исходники загрузчика → FTDriver_64.dll
    SDK/              Шаблон проекта плагина
    Release/          Готовые сборки по версиям
    Addresses/<версия игры>/_addresses.ini
```

## Версия игры и библиотеки адресов

Версия Perfect World — 1.7.6. Адреса функций относятся к конкретному бинарнику. Библиотеки
лежат в `<компонент>/Addresses/<версия игры>/_addresses.ini`; в заголовке файла указаны размер и
SHA-256 бинарника, для которого сняты адреса.

Формат библиотеки и правила её пополнения — в `README.md` компонента.

## Требования

- `PWSE64` — Docker Desktop. Сборка выполняется в контейнере, компилятор на хосте не нужен.
- `PWCE64` — Windows и mingw-w64 x86-64 (`gcc` в `PATH`).

## Разработка плагина

Плагин создаётся из шаблона `SDK` нужного компонента: копия папки, реализация `src/plugin.c`,
`build.bat`. Правила кода — `PWSE64/SDK/README.md`, раздел «Стиль кода».

## Лицензия

GNU General Public License v3.0 — файл `LICENSE`.

## Сторонний код

`PWSE64/Source/vendor/subhook` и `PWCE64/Source/vendor/subhook` — библиотека установки хуков
[subhook](https://github.com/Zeex/subhook), форк savushkin-r-d, лицензия BSD 2-Clause. Текст
лицензии — `THIRD-PARTY-NOTICES.md` и `LICENSE.txt` в каталоге библиотеки.
