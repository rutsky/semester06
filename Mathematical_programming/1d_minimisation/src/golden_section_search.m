# golden_section_search.m
# Golden section search algorithm.
# Vladimir Rutsky <altsysrq@gmail.com>
# 17.03.2009

precPows = [-3:-1:-6];
#precPows = [-3:-1:-3];
#func = inline("sin(1./x)");
#range = [0.15, 0.6];

load data/function.mat
load data/segment.mat

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
  #plotFunction(func, a, b, 1e-3); # drawing graphic
  
  # Searching minimum with golden section search (with visualization).
  phi = (5^0.5 + 1) / 2;
  alpha = 1 / phi;
  
  originalRange = [a, b];
  
  x1 = a + (1 - alpha) * (b - a);
  x2 = a + (    alpha) * (b - a);
  y1 = func(x1);
  y2 = func(x2);
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
    #plot(x, y, "+k"); # drawing graphic
    
    if (y1 <= y2)
      b  = x2;
      x2 = x1;
      x1 = a + (1 - alpha) * (b - a);
      y2 = y1;
      y1 = func(x1);
    else
      a  = x1;
      x1 = x2;
      x2 = a + alpha * (b - a);
      y1 = y2;
      y2 = func(x2);
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
  
  resStr = sprintf("Minimum at x=%9.7f, with precision of %5.2e", res, precision)
  title(resStr);
  
  #drawnow(); # drawing graphic
  
  retval = [res, i];
endfunction

# Iterating through precisions.
precs = 10 .^ precPows;
for prec = precs
  retval = goldenSectionSearch(func, range(1), range(2), prec)
  
  # TODO: output to a file.
  #filename = "../output/result.tex";
  #fid = fopen(filename, "w");
  #fputs (fid, "Free Software is needed for Free Science");
  #fclose(fid);
endfor

#input("Press enter to quit."); # drawing graphic 