set xrange [-10:10]
set yrange [-10:10]

splot x**2 + 2 * y**2 + 4 * (2 + x**2 + y**2)**0.5
