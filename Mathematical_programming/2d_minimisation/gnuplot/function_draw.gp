# function_draw.gp
# Drawing function to find local minimum area.
# Vladimir Rutsky <altsysrq@gmail.com>
# 07.04.2009

# X11 faster than `wxt' on my machine
#set terminal x11
#set terminal latex size 10cm, 10cm
#set terminal postscript size 10cm, 10cm
#set terminal svg 
set terminal pslatex size 10cm, 10cm

set auto

set pal gray

set xlabel "x"
set ylabel "y"
set zlabel "f(x,y)"

set border 4095
set ticslevel 0

load "input/draw_function.gp"
#pause -1
