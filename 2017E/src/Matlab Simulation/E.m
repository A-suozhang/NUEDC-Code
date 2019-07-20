fs = 625000;     % Sample Rate - 300k
N = 4096;          % How Many Points To Calc
N_fft = 4096 ;
f0 = 10000;          % The Wave Frequency
f1 = 11000;

phase_shift = 0.9;  % Pretend Using Delay To Create Phase Shit, Ranging from (0,1)

xs = 0:1/fs:N*(1/fs);  
amp_a = 1;
amp_b = 1;
%

y_a = amp_a*sin(2*pi*f0*xs);  % A - 10k Sin Wave 
y_b = amp_b*sin(2*pi*f1*xs);  % B -12k Sin Wave
y_c = y_a + y_b;    % Signal C

dfft = abs(fft(y_c,N_fft)) - abs(fft(y_b,N_fft)) - abs(fft(y_a,N_fft));



% --------------------- Ideal Phase Shift -------------------

y_a_r = amp_a*sin(2*pi*f0*xs + phase_shift*pi);  % A - 10k Sin Wave After Phase Shift - Unavailable 
y_a_r_n = awgn(y_a_r, 10);  % adding noise, will cause Amp(POWER) Wrong
y_b_r = amp_b*sin(2*pi*f1*xs + phase_shift*pi);  % B -12k Sin Wave After Phase Shift - Unavailable
y_b_r_n = awgn(y_b_r, 10);
y_d = y_a_r + y_b_r;    % Signal D

% ---------------------   Testing FFT Accuracy -----------------------
% plot(abs(fft(y_a,N_fft)))
% hold on;
% plot(abs(fft(y_a_r,N_fft)))
% hold on;
% plot(abs(fft(y_a,N_fft)) - abs(fft(y_a_r,N_fft)))
% max(abs(fft(y_a,N_fft)) - abs(fft(y_a_r,N_fft)))/max(abs(fft(y_a,N_fft)))
% p = sum(abs(fft(y_a,N_fft)).^2) - sum(abs(fft(y_a_r,N_fft)).^2)
% pp = p/sum(abs(fft(y_a,N_fft)).^2)

% -----------------   Test & Plot Signal C -----------------
% plot(xs, y_c,);

% fft_c = fft(y_c, N_fft);
% abs_fft_c = abs(fft_c);
% stem(abs_fft_c(1:N_fft*0.1));
% [M,i] = max(abs_fft_c);
% amp = M*N_fft/2
% f = i/N_fft*fs
% 


% Amps =[];
% % --------- 部分定标 (Freq)-----------
% for f0 = 10000:100:100000 
%     Amp = 1;
%     ys = Amp*sin(2*pi*f0*xs);
%     Abs_fft = abs(fft(ys,N_fft));
%     [M,i] = max(Abs_fft);
%     Amp_fft = sqrt(Abs_fft(i).^2 + Abs_fft(i-1).^2 + Abs_fft(i-2).^2 + Abs_fft(i+1).^2 + Abs_fft(i +2).^2);
% %     Amp_fft = sqrt(sum(abs(fft(ys, N_fft)).^2))
%     Amps = [Amps, Amp_fft];
% end

% ------------Only The B & D Is Available ---------------
% ------------------------------------------------------------
% --------------Method 1 ---------------------------------
% ---- Since Only Phase Shifft Will Not Affect Amp ---
%----- Using FFT Amp To Determine ------------------

fft_b = fft(y_b_r, N_fft);
fft_d = fft(y_d, N_fft);

fft_a_r = fft_d - fft_b;
abs_fft_b = abs(fft_b);
% stem(abs_fft_b);      % Plot Signal A's FFT 
abs_fft_d = abs(fft_d);
% plot(xs, y_b,xs,yc_d);
% stem(fft_b(1:N_fft*0.1));
abs_fft_a_r = abs_fft_b - abs_fft_d;
abs_fft_a_r2 = abs(fft(y_a_r, N_fft));
dfft = abs_fft_a_r - abs_fft_a_r2;
% stem(abs_fft_a_r);
% hold on;
% stem(abs_fft_a_r2);
stem(abs_fft_a_r(1:N_fft*0.5));
[M,i] = max(abs_fft_a_r);
% amp = M*N_fft/2;
% p = sum(abs_fft_a_r.^2);
p = (abs_fft_a_r(i).^2 + abs_fft_a_r(i+1).^2 + abs_fft_a_r(i-1).^2);
% new_amp = sqrt(p)/( 1447.776440678686)  % Calibration For FFT
new_amp = sqrt(p)/(1017.664540853342)  % Calibration For FFT 
%This Variable Concerned with FFT_N!!

% new_amp = sqrt(p)/(Amps(round((f - 10000)/100) + 1))  % Calibration For FFT
da = (new_amp - amp_a)/amp_a
f = i/N_fft*fs
df = (f0 - f)/f0







