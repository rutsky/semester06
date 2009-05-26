f = inline("x^3+2*y+4*(2+x^2+y^2)^(1/2)");
dfdx1 = inline("(3*x^2*sqrt(y^2+x^2+2)+4*x)/sqrt(y^2+x^2+2)");
dfdx2 = inline("(2*sqrt(y^2+x^2+2)+4*y)/sqrt(y^2+x^2+2)");
df2dx1dx1 = inline("(6*x*y^4+sqrt(y^2+x^2+2)*(4*y^2+8)+(12*x^3+24*x)*y^2+6*x^5+24*x^3+24*x)/(y^4+(2*x^2+4)*y^2+x^4+4*x^2+4)");
df2dx2dx2 = inline("(4*x^2+8)*sqrt(y^2+x^2+2)/(y^4+(2*x^2+4)*y^2+x^4+4*x^2+4)");
df2dx1dx2 = inline("-4*x*y/(y^2+x^2+2)^(3/2)");
