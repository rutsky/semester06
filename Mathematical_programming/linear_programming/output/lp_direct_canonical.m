c = [ -3 0 -1 1 0]';
A = [-1 3 0 1 0 0; 4 1 0 0 1 0; -2 1 1 0 0 0; 1 -3 0 0 0 1];
b = [ 26 1 0]';
lb = [ 0 0 0 0 0]';
ub = [ 7.23778e-317 6.95336e-310 2.61433e-310 1.97626e-323 1.63042e-322]';
ctype = "SSSS";
vartype = "CCCCCC";
sense = 1;

[xmin, fmin, status, extra] = glpk(c, A, b, lb, ub, ctype, vartype)
