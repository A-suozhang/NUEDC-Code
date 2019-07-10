fs = 10000000;

xs = 0:1/fs:1/(fs/10000);
% ys = sin(2*pi*500000*xs);
ys = 0.2* sin(2*pi*500000*xs) + sin(2*pi*10000*xs);
hd = fir;   
y = filter(Num,1, ys);
plot(xs,ys,'r')
hold on;
plot(xs,y,'g')
