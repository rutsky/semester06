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

#set contour surface
set contour
#unset clabel
load '../output/ne_contours.gp'

splot f(x, y) title fName, \
  '../output/ne_points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#0000FF" with lines,      \
  '../output/ne_points.dat' using 1:2:(f($1, $2) - 40) notitle linewidth 3 linecolor rgb "#0000FF" with lines,      \
  'input/real_points.dat' using 1:(constrain1Line($1)):(constrainHeight) notitle linewidth 1 pointtype 0 linecolor rgb "#FF0000" with lines, \
  'input/real_points.dat' using 1:(constrain2Line($1)):(constrainHeight) notitle linewidth 1 pointtype 0 linecolor rgb "#FF0000" with lines
