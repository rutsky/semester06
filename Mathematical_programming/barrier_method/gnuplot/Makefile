# Makefile
# Vladimir Rutsky <altsysrq@gmail.com>
# 07.04.2009

all: output/function_graph.tex

output/function_graph.tex: function_draw.gp input/draw_function.gp
	gnuplot function_draw.gp > output/function_graph.tex
	#gnuplot function_draw.gp > function_graph.ps
	#gnuplot function_draw.gp > function_graph.svg
	#ps2pdf function_graph.ps output/function_graph.pdf
	#rm function_graph.ps

#TODO:
#clean:
#	rm -f *~
#distclean: clean
#	rm -f ...
