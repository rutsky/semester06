Для сборки проекта в MS Visial Studio 6 необходимы библиотеки SDL, SDL_image и 
ассемблер nasm, их можно поставить следующим образом:

1. Распаковать SDL-devel-1.2.13-VC6.zip [1] в C:\SDL
2. Распаковать SDL_image-devel-1.2.7-VC9.zip [2] в C:\SDL_image
3. Распаковать nasm-2.05-win32.zip [3] в C:\nasm

Следует распаковать так, чтобы в итоге существовали следующие файлы: 
  C:\SDL\include\SDL.h
  C:\SDL_image\include\SDL_image.h
  C:\nasm\nasm.exe

4. Скопировать dll-ки из SDL\lib\ и SDL_image\lib в C:\WINDOWS

Источники:
  [1] http://www.libsdl.org/release/SDL-1.2.13-win32.zip
  [2] http://www.libsdl.org/projects/SDL_image/release/SDL_image-devel-1.2.7-VC9.zip
  [3] http://www.nasm.us/pub/nasm/releasebuilds/2.05/nasm-2.05-win32.zip
