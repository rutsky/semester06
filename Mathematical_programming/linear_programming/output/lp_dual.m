c = [13 26 1 0]';
A = [-1 4 -2 1; 3 1 1 -3; 0 0 1 0; 1 0 0 0; 0 1 0 0; 0 0 0 1];
b = [4 -3 0 -1 1 0]';
lb = [-1e+10 -1e+10 -1e+10 -1e+10]';
ub = [1e+10 1e+10 1e+10 1e+10]';
ctype = "UUUUUU";
vartype = "CCCC";
sense = -1;

[xmin, fmin, status, extra] = glpk(c, A, b, lb, ub, ctype, vartype)
