load 'input/function_def.gp'
load 'input/function_range.gp'

set samples 100
set isosamples 50

#set cntrparam levels 50
#set cntrparam linear
#set cntrparam cubicspline
#set cntrparam levels discrete 3,4,5,6,7,8,9,10,11,12,13

set surface
set hidden3d

set contour
#unset clabel
load '../output/gd_contours.gp'

#show palette palette 5#5n6#6

splot f(x, y) title fName, \
  '../output/gd_points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#0000FF" with lines, \
  '../output/gd_points.dat' using 1:2:(f($1, $2) - 1) notitle linewidth 3 linecolor rgb "#0000FF" with lines

#pause -1

#unset surface
#unset ztics
#unset zlabel

#set border 15

#set samples 100
#set isosamples 100

#set grid
#set contour
#unset clabel
#load 'gd_contours.dat'

#splot 'gd_points.dat' using 1:2:(f($1, $2)) notitle linewidth 3 linecolor rgb "#0000FF" with lines
