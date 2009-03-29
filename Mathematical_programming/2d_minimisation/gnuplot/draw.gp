# draw.gp
# Drawing program running result with GNU Plot.
# Vladimir Rutsky <altsysrq@gmail.com>
# 30.03.2009

# X11 faster than `wxt' on my machine
set terminal x11

set auto

set surface

#set sample 31; set isosamples 30

set pal color
set pal maxcolors 0
set grid layerdefault

set hidden3d
set pm3d at s
set view 50,220

set xlabel "x"
set ylabel "y"

set border 4095
set ticslevel 0

unset clabel
set contour

load "function.gp"

pause -1
