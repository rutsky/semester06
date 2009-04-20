# Creating Octave's functions:

func = inline('cos(x)./x.^2');
save("function.mat", "func");

funcDer = inline('-sin(x)./x.^2 .- 2 .* cos(x)./x.^3');
save("function_der.mat", "funcDer");

funcDer2 = inline('-cos(x)./x.^2 .+ 4 .* sin(x)./x.^3 .+ 6 .* cos(x) ./ x.^4');
save("function_der2.mat", "funcDer2");

range = [7, 11];
save("segment.mat", "range");
