load 'input/function_def.gp'
load 'input/function_range.gp'

set view 39, 348

set samples 25
set isosamples 25

unset clabel
set cntrparam levels 70
#set cntrparam linear
#set cntrparam cubicspline
#set cntrparam levels discrete 3,4,5,6,7,8,9,10,11,12,13

#set surface
#set hidden3d

set contour surface

splot f(x, y) linecolor rgb "#000000" notitle
