# Универсальный конвертер изображений
Основные возможности консольной программы включают в себя конвертацию изображений между форматами (BMP, JPEG, PPM), а также поддержку операций обработки изображений, таких как применение оператора Собеля для выделения границ.

## Структура 
```
cpp-image-converter/
├── ImgConverter/
|   ├── CMakeLists.txt
│   └── main.cpp              # Точка входа входа и реализация логики конвертации
├── ImgLib/
|   ├── CMakeLists.txt
│   ├── bmp_image.cpp         # Реализация работы с BMP изображениями
│   ├── bmp_image.h           
│   ├── img_lib.cpp           # Реализация общих функций обработки изображений
│   ├── img_lib.h             
│   ├── jpeg_image.cpp        # Реализация работы с JPEG изображениями
│   ├── jpeg_image.h          
│   ├── ppm_image.cpp         # Реализация работы с PPM изображениями
│   ├── ppm_image.h           
│   ├── pack_defines.h        # Утилитарные макросы для упаковки данных 
└── README.md                 # ← вы здесь
```
## Логика
Принимает два аргумента командной строки — входной и выходной файлы.
Определяет формат входного и выходного файлов с помощью функции GetFormatByExtension().
Загружает изображение с помощью соответствующего интерфейса.
Сохраняет изображение в нужный формат.

- `GetFormatByExtension()` принимает путь к файлу и возвращает формат изображения (BMP, JPEG, PPM или UNKNOWN), основываясь на расширении файла.
- `ImageFormatInterface` интерфейс, который определяет два виртуальных метода:
  - `LoadImage` загружает изображение из файла.
  - `SaveImage` сохраняет изображение в файл.

Классы-реализации интерфейса:

- `PPM`: для работы с изображениями формата PPM (функции SavePPM, LoadPPM).
- `JPEG`: для работы с JPEG (функции SaveJPEG, LoadJPEG).
- `BMP`: для работы с BMP (функции SaveBMP, LoadBMP).
`GetFormatInterface()` возвращает указатель на объект, реализующий интерфейс ImageFormatInterface в зависимости от формата файла.

## Требования
- CMake 3.10+
- C++17
