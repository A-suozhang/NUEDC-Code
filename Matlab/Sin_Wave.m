fs = 625000;
N = 2048;
f0 = 20000;
Amp0 = 0.3;
f1 = 21112;
Amp1 = 0.5;
phase_shift = 0.4;

xs = 0:1/fs:(1/fs)*(N-1);

% -----------------The C Signal -----------------
ys0 = Amp0*sin(2*pi*f0*xs + phase_shift*pi) + Amp1*square(2*pi*f1*xs+phase_shift*pi);  % 注意因为两个相加了，要保证最后的结果量化正确, 所以需要Amp0 + Amp1 < 1,如果违反的话会出问题
y0 =  round(ys0*(2^13))
y0_bak =  round(ys0*(2^13));
y0(find(y0<0))=y0(find(y0<0))+2^14;   %负数转补码

% plot(y0);
 y0 = dec2hex(y0);
 
 %--------------- The B Signal -------------------
 ys1 = Amp1*square(2*pi*f1*xs + phase_shift*pi);
y1 =  round(ys1*(2^13))
y1_bak =  round(ys1*(2^13));
y1(find(y1<0))=y1(find(y1<0))+2^14;   %负数转补码

% plot(y0);
 y1 = dec2hex(y1);


f0= fopen("sin_wave0.txt","w");
for i = 1:length(y0)
    fprintf(f0, "%s\n", y0(i,1:4));
end
fclose(f0);

f1= fopen("sin_wave1.txt","w");
for i = 1:length(y1)
    fprintf(f1, "%s\n", y1(i,1:4));
end
fclose(f1);

dy0_fft = abs(fft(y0_bak - y1_bak,N));
y0_fft = abs(fft(y0_bak, N));
y1_fft = abs(fft(y1_bak, N));
dy = y0_fft - y1_fft;
plot(dy);
hold on;
plot(dy0_fft);
hold on; 
plot(dy - dy0_fft);

display('Writing Done');

% stem(y0_fft);
% hold on;
% stem(y1_fft);



