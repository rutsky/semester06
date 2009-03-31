set xrange [-3:3]
set yrange [-3:3]

#set logscale z

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
load 'gd_contours.gp'

#show palette palette 5#5n6#6

f(x, y) = x**3 + 2 * y + 4 * (2 + x**2 + y**2)**0.5
splot f(x, y) title 'x**3 + 2 * y + 4 * (2 + x**2 + y**2)**0.5', \
  'gd_points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#0000FF" with lines, \
  'gd_points.dat' using 1:2:(f($1, $2) - 1) notitle linewidth 3 linecolor rgb "#0000FF" with lines

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
