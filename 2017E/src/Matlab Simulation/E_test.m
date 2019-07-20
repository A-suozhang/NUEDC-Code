fs = 625000;     % Sample Rate - 300k
N = 4096;          % How Many Points To Calc
N_fft = 4096 ;
f0 = 10000;          % The Wave Frequency
f1 = 11000;

phase_shift = 0.5;  % Pretend Using Delay To Create Phase Shit, Ranging from (0,1)

xs = 0:1/fs:N*(1/fs);  
amp_a = 1;
amp_b = 1;
%

y_a = amp_a*sin(2*pi*f0*xs);  % A - 10k Sin Wave 
y_b = amp_b*sin(2*pi*f1*xs);  % B -12k Sin Wave
y_c = y_a + y_b;    % Signal C

dfft = abs(fft(y_c,N_fft)) - abs(fft(y_b,N_fft)) - abs(fft(y_a,N_fft));

y_a_r = amp_a*sin(2*pi*f0*xs + phase_shift*pi);  % A - 10k Sin Wave After Phase Shift - Unavailable 
y_a_r_n = awgn(y_a_r, 10);  % adding noise, will cause Amp(POWER) Wrong
y_b_r = amp_b*sin(2*pi*f1*xs + phase_shift*pi);  % B -12k Sin Wave After Phase Shift - Unavailable
y_b_r_n = awgn(y_b_r, 10);
y_d = y_a_r + y_b_r;    % Signal D

dfft1 = abs(fft(y_d,N_fft) - abs(fft(y_b_r,N_fft))) - abs(fft(y_a_r,N_fft));
dfft2 = abs(fft(y_d,N_fft) - fft(y_b,N_fft)) - abs(fft(y_a_r, N_fft));
plot(dfft2)