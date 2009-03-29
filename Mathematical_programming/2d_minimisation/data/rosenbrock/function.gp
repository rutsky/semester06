set title "Rosenbrock Function"

set xrange [-1.5:1.5]
set yrange [-0.5:1.5]

set samples 100
set isosamples 30

set logscale z

set cntrparam levels 5
set cntrparam linear

#set border 15

splot (1 - x)**2 + 100 * (y - x**2)**2
