c = [4 -3 0 -1 1 0]';
A = [-1 3 0 1 0 0; 4 1 0 0 1 0; -2 1 1 0 0 0; 1 -3 0 0 0 1];
b = [13 26 1 0]';
lb = [0 0 0 0 0 0]';
ub = [1e+10 1e+10 1e+10 1e+10 1e+10 1e+10]';
ctype = "SSSS";
vartype = "CCCCCC";
sense = 1;

[xmin, fmin, status, extra] = glpk(c, A, b, lb, ub, ctype, vartype, sense)
