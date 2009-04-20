# golden_section_search.m
# Drawing graphic.
# Vladimir Rutsky <altsysrq@gmail.com>
# 07.04.2009

load data/function.mat
load data/segment.mat

# Rendering graphic.
figure(1, "visible", "off");
#hold on;
x = [range(1):1e-3:range(2)];
y = func(x);
plot(x, y, "-k");
drawnow();

print -dpng "../output/graphic.png"
print -dpslatex "../output/graphic.tex"
