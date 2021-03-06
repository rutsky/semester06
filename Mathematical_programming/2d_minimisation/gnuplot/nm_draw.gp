# nm_draw.gp
# Newton method running results with Gnuplot.
# Vladimir Rutsky <altsysrq@gmail.com>
# 26.05.2009

# X11 faster than `wxt' on my machine
set terminal x11

set auto

#set surface

#set pal color
#set pal maxcolors 0
#set grid layerdefault

#set hidden3d
#set pm3d at s

set xlabel "x"
set ylabel "y"
set zlabel "f(x,y)"

set border 4095
set ticslevel 0

#unset clabel
#set contour

#set style data lines

load "input/nm_function.gp"
pause -1
