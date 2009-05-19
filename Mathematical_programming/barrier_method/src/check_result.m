x = [0.666667,0.666667]'

% y' * phi(x) = 0
% grad phi0(x) + y[0] * (grad phi[0]) + y[0] * (grad phi[1]) = 0
% grad phi0(x) + y[0] * (grad phi[0]) + y[0] * (grad phi[1]) = 0
%   grad phi[0]                        - grad phi0(x)
%                *   ( y[0] y[1] )  = 
%   grad phi[1]                        - grad phi0(x)

f = inline("x^2 + y^2 - 10 * x - 8 * y");
constraintA = inline("1 * x + 2 * y - 2");
constraintB = inline("2 * x + 1 * y - 2");

b = -[2 * x(1) - 10, 2 * x(2) - 8]'

dConstraintA = [1 2]';
dConstraintB = [2 1]';

A(:,1) = dConstraintA;
A(:,2) = dConstraintB;

A

y = inv(A) * b
A * y - b

phix = [constraintA(x(1), x(2)), constraintB(x(1), x(2))]'
y' * phix
