
                   ***************************
                   *Технология Wolfenstein 3D*
                   ***************************

Список задач:

Обязательные задачи:
1) Дописать случай вывода вертикальной линии когда она 
   не помещается в пределах экрана
2) Переписать на asm процедуру вывода стенок.

Дополнительные задачи (для курсовика):
1) Избавиться от многочисленных инструкций ветвления
   в процедуре заливки вертикальных стен.
2) Объединить процедуры поиска пересечения лучей и стенок
   вместе с процедурой вывода вертикальной линии.
3) Сделать mipmap-ы для улучшения качества рендеринга
4) Перейти на целые в процедуре определения пересечения луча
   и стены
5) Разработать процедуру заливки горизонтальных полов и потолков
6) Сократить площадь проливки черным цветом буфера экрана
   перед подготовкой кадра. Если реализована процедура
   заливки пола и потолка то убрать очистку буфера экрана совсем.
7) Корректно отталкиваться от стенок не проходя сквозь них
8) Реализовать процедуру вывода карты из отрезков-стен для
   более удобной навигации в пространстве
9) Сделать более удобную реакцию на нажатия клавиш
   например с помощью ф-ции GetKeyboardState (а не сообщения Windows)
10)Добавить персонажей в виде спрайтов (прямоугольников паралельных 
   экрану)
