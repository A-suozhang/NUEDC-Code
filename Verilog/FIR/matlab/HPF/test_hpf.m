fs = 10000000;

xs = 0:1/fs:1/(fs/100000);
% ys = sin(2*pi*500000*xs);
ys = sin(2*pi*100000*xs) + sin(2*pi*10000*xs);
hd = hpf;   
y = filter(HPF,1, ys);
plot(xs,ys,'r')
hold on;
plot(xs,y,'g')
