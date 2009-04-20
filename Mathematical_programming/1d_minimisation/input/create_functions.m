# Creating Octave's functions:

func = inline('cos(x)./x.^2');
save("function.mat", "func");

funcDer = inline('(-sin(x) .* x.^2 .- cos(x) .* 2 .* x)./x.^4');
save("function_der.mat", "funcDer");

range = [7, 11];
save("segment.mat", "range");
