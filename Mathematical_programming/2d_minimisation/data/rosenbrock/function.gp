set title "Rosenbrock function"

set xrange [-1.5:1.5]
set yrange [-0.5:1.5]

set samples 100
set isosamples 30

set logscale z

set cntrparam levels 5
set cntrparam linear

set border 15
#set dgrid3d

#unset contour

f(x,y) = (1 - x)**2 + 100 * (y - x**2)**2
splot f(x, y), 'points.dat' using 1:2:(f($1, $2)) notitle lw 2
