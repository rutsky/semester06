# golden_section_search.m
# Golden section search algorithm.
# Vladimir Rutsky <altsysrq@gmail.com>
# 17.03.2009

precPows = [-3:-1:-8];

load data/function.mat      # func
load data/function_der.mat  # funcDer
load data/function_der2.mat # funcDer2
load data/segment.mat       # range

function plotFunction( func, a, b, step )
  assert(a <= b);
  
  x = [a:step:b];
  y = func(x);
  
  plot(x, y, "-");
endfunction

function retval = goldenSectionSearch( func, a, b, precision )
  assert(a <= b);
  
  #figure(); # drawing graphic
  #hold on; # drawing graphic
  
  # Plotting function.
  plotFunction(func, a, b, 1e-3); # drawing graphic
  
  # Searching minimum with golden section search (with visualization).
  phi = (5^0.5 + 1) / 2;
  alpha = 1 / phi;
  
  originalRange = [a, b];

  fcalls = 0;
  
  x1 = a + (1 - alpha) * (b - a);
  x2 = a + (    alpha) * (b - a);
  y1 = func(x1); fcalls++;
  y2 = func(x2); fcalls++;
  i = 0;
  while (abs(b - a) >= precision)
    assert(a < x1);
    assert(x1 < x2);
    assert(x2 < b);
    oldDist = b - a;
    
    # Drawing.
    yMax = max([func(a), func(x1), func(x2), func(b)]);
    #yMax = max([func(x1), func(x2)]);
    x = [a,    x1,   x2,   b   ];
    y = [yMax, yMax, yMax, yMax];
    plot(x, y, "+k"); # drawing graphic
    
    if (y1 <= y2)
      b  = x2;
      x2 = x1;
      x1 = a + (1 - alpha) * (b - a);
      y2 = y1;
      y1 = func(x1); fcalls++;
    else
      a  = x1;
      x1 = x2;
      x2 = a + alpha * (b - a);
      y1 = y2;
      y2 = func(x2); fcalls++;
    endif

    newDist = b - a;
    assert(oldDist > newDist);
    
    i++;
  endwhile
  
  if (y1 <= y2)
    res = x1;
  else
    res = x2;
  endif
  
  resStr = sprintf("Minimum at x=%11.9f, with precision of %5.2e", res, precision);
  title(resStr);
  
  #drawnow(); # drawing graphic
  
  retval = [res, i, fcalls];
endfunction

# Outputting to a file by the way.
filename = "../output/result.tex";
fid = fopen(filename, "w");

# Iterating through precisions.
precs = 10 .^ precPows;

prevFuncInit = 0;
prevFunc = 0;

for prec = precs
  retval = goldenSectionSearch(func, range(1), range(2), prec)
  
  res = retval(1);
  fres = func(res);
  
  # Table: precision, iterations, function calls, result.
  buf = sprintf("%2.1e & %d & %d & %11.9f & %11.9f & ", prec, retval(2), retval(3), res, fres);
  fputs(fid, buf);
  
  # Table: function value delta.
  if (prevFuncInit)
    buf = sprintf("%e", fres - prevFunc);
    fputs(fid, buf);
  endif
  
  # Table: function derivative, function second derivative.
  buf = sprintf(" & %7.3e & %7.3e \\\\\n", funcDer(res), funcDer2(res));
  fputs(fid, buf);
  
  prevFuncInit = 1;
  prevFunc = fres;
endfor

fclose(fid);

#input("Press enter to quit."); # drawing graphic 
