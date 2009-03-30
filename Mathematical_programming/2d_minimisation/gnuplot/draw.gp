# draw.gp
# Drawing program running result with GNU Plot.
# Vladimir Rutsky <altsysrq@gmail.com>
# 30.03.2009

# X11 faster than `wxt' on my machine
set terminal x11

set auto

#set surface
# set multiplot

#set pal color
#set pal maxcolors 0
#set grid layerdefault

#set hidden3d
#set pm3d at s

set xlabel "x"
set ylabel "y"

set border 4095
set ticslevel 0

#unset clabel
#set contour

#set style data lines

load "function.gp"

#unset multiplot

pause -1
