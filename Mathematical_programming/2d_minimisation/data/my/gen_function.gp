load 'input/function_def.gp'
load 'input/function_range.gp'

set samples 100
set isosamples 50

set surface
set hidden3d

set contour
#unset clabel
load '../output/gd_contours.gp'

splot f(x, y) title fName, \
  '../output/gen_selected_points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#000000" with points, \
  '../output/gen_selected_points.dat' using 1:2:(f($1, $2) - 1) notitle linewidth 3 linecolor rgb "#000000" with points, \
  '../output/gen_not_selected_points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#0000FF" with points, \
  '../output/gen_not_selected_points.dat' using 1:2:(f($1, $2) - 1) notitle linewidth 3 linecolor rgb "#0000FF" with points
