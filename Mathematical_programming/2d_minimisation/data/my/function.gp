set xrange [-12:12]
set yrange [-12:12]

#set logscale z

set samples 100
set isosamples 100

set cntrparam levels 5
set cntrparam linear

set surface
set hidden3d

set contour
#unset clabel
load 'contours.dat'

#show palette palette 5#5n6#6

f(x, y) = x**2 + 2 * y**2 + 4 * (2 + x**2 + y**2)**0.5
splot f(x, y) title 'x**2 + 2 * y**2 + 4 * (2 + x**2 + y**2)**0.5', \
  'points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#0000FF" with lines, \
  'points.dat' using 1:2:(f($1, $2) - 1) notitle linewidth 3 linecolor rgb "#0000FF" with lines

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
#load 'contours.dat'

#splot 'points.dat' using 1:2:(f($1, $2)) notitle linewidth 3 linecolor rgb "#0000FF" with lines
