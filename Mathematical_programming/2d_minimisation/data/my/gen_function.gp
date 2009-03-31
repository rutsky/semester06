set xrange [-3:3]
set yrange [-3:3]

#set logscale z

set samples 100
set isosamples 50

set surface
set hidden3d

set contour
#unset clabel
load 'gd_contours.gp'

f(x, y) = x**3 + 2 * y + 4 * (2 + x**2 + y**2)**0.5
splot f(x, y) title 'x**3 + 2 * y + 4 * (2 + x**2 + y**2)**0.5', \
  'gen_selected_points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#000000" with points, \
  'gen_selected_points.dat' using 1:2:(f($1, $2) - 1) notitle linewidth 3 linecolor rgb "#000000" with points, \
  'gen_not_selected_points.dat' using 1:2:(f($1, $2) + 1) notitle linewidth 3 linecolor rgb "#0000FF" with points, \
  'gen_not_selected_points.dat' using 1:2:(f($1, $2) - 1) notitle linewidth 3 linecolor rgb "#0000FF" with points
