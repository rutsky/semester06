# draw.gp
# Drawing program running result with GNU Plot.
# Vladimir Rutsky <altsysrq@gmail.com>
# 30.03.2009

#set title "set hidden3d"

set terminal x11
set samples 30; set isosamples 30
set grid layerdefault
set hidden3d
set pm3d
set surface
set view 50,220
set xrange [-10:10]
set yrange [-10:10]

load "function.gp"

pause -1
