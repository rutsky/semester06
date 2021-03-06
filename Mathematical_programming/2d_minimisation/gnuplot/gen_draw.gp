# draw.gp
# Drawing program running result with Gnuplot.
# Vladimir Rutsky <altsysrq@gmail.com>
# 30.03.2009

# X11 faster than `wxt' on my machine
set terminal x11

set auto

set xlabel "x"
set ylabel "y"
set zlabel "f(x,y)"

set view 180,0

set border 4095
set ticslevel 0

load "input/gen_function.gp"
pause -1
