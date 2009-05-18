c = [-13 -26 -1 -0 13 26 1 0 -0 -0 -0 -0 -0 -0]';
A = [1 -4 2 -1 -1 4 -2 1 -1 -0 -0 -0 -0 -0; -3 -1 -1 3 3 1 1 -3 -0 -1 -0 -0 -0 -0; -0 -0 -1 -0 0 0 1 0 -0 -0 -1 -0 -0 -0; -1 -0 -0 -0 1 0 0 0 -0 -0 -0 -1 -0 -0; -0 -1 -0 -0 0 1 0 0 -0 -0 -0 -0 -1 -0; -0 -0 -0 -1 0 0 0 1 -0 -0 -0 -0 -0 -1];
b = [-4 3 -0 1 -1 -0]';
lb = [0 0 0 0 0 0 0 0 0 0 0 0 0 0]';
ub = [1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10 1e+10]';
ctype = "SSSSSS";
vartype = "CCCCCCCCCCCCCC";
sense = 1;

[xmin, fmin, status, extra] = glpk(c, A, b, lb, ub, ctype, vartype)