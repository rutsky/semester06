c = [ 26 1 0]';
A = [-1 4 -2 1; 3 1 1 -3; 0 0 1 0; 1 0 0 0; 0 1 0 0; 0 0 0 1];
b = [ -3 0 -1 1 0]';
lb = [ 1e+10 1e+10 1e+10]';
ub = [ 2.47033e-323 6.95336e-310 3.56611e+179]';
ctype = "LLLLLL";
vartype = "CCCC";
sense = -1;

[xmin, fmin, status, extra] = glpk(c, A, b, lb, ub, ctype, vartype)
