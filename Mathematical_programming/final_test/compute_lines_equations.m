# compute_lines_equations.m
# Computes lines equations for final test 
# Vladimir Rutsky <altsysrq@gmail.com>
# 28.05.2009

x = [0 0 5 10 8 0];
y = [0 6 8  4 0 0];

pInsideX = 1;
pInsideY = 1;

printf("LP problem constraints:\n");
for i = 1:(size(x)(2) - 1)
  # Line equation: (x - x1) / (x1 - x2) = (y - y1) / (y1 - y2)
  # In common form: (y1 - y2) * x + (x2 - x1) * y + (x1 * y2 - x2 * y1) = 0

  x1 = x(i);
  y1 = y(i);
  x2 = x(i + 1);
  y2 = y(i + 1);
  
  A = y1 - y2;
  B = x2 - x1;
  C = x1 * y2 - x2 * y1;
  
  s = sign(A * pInsideX + B * pInsideY + C);
  
  # Outputting in form of LP problem constraints.
  printf("%3g %3g  (%+g)  %3g\n", A, B, s, -C);
endfor
