set title "Rosenbrock function"

set xrange [-1.5:1.5]
set yrange [-0.5:1.5]

set view 50, 100

set samples 100
set isosamples 30

set logscale z

#set cntrparam levels 5
#set cntrparam linear

#set border 15
#set dgrid3d

set contour
unset clabel

#set pm3d at s hidden3d 100
#set style line 100 lt 5 lw 0.5
#unset hidden3d
#unset surf

f(x,y) = (1 - x)**2 + 100 * (y - x**2)**2
splot f(x, y) with pm3d title '(1 - x)**2 + 100 * (y - x**2)**2', 'points.dat' using 1:2:(f($1, $2)) notitle lw 3 lc rgb "#0000FF" with lines
