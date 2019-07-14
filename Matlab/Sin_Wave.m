fs = 312500;
N = 128;
f0 = 10000;

xs = 0:1/fs:(1/fs)*(N-1);
ys = sin(2*pi*f0*xs);
y =  round(ys*(2^13))
y0 =  round(ys*(2^13));
y(find(y<0))=y(find(y<0))+2^14;   %¸ºÊý×ª²¹Âë
plot(y);
 y = dec2hex(y);
 


f = fopen("sin_wave.txt","w");
for i = 1:length(y)
    fprintf(f, "%s\n", y(i,1:4));
end
% plot(y);