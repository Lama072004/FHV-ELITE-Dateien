clear
clc
close all


%% Parameter
T0  = 4;        % Periodendauer
p   = 0.25;      % Tastverhältnis
OW  = 20;       % Anzahl der Oberwellen
N   = 2*OW + 3; % Gesamtanzahl Fourier-Koeffizienten (inkl. DC-Anteil + Grundschwingung 2x)
w0  = 1*pi/T0;  % Kreisfrequenz

%% Symbolische Variablen
syms t x(t) C(nn)
nn = -OW-1 : OW+1;   % Indizes

%% Rechtecksignal
f(t) = heaviside(t) - heaviside(t-T0*p);
tt = 0:0.001:T0;

figure
plot(tt, double(subs(f(t), t, tt)), 'LineWidth', 1.5)
xlabel('t [s]'); ylabel('f(t)'); title('Rechtecksignal'); grid on

%% Fourierkoeffizienten
C_vals = sym(zeros(size(nn)));
for k = 1:length(nn)
    n_k = nn(k);
    C_vals(k) = 1/T0 * int(f(t) * exp(-1j*n_k*w0*t), t, 0, T0);
end

figure
stem(nn, abs(C_vals), 'filled')
xlabel('n'); ylabel('|C_n|'); title('Amplitudenspektrum'); grid on

%% Rücktransformation
x(t) = sum(C_vals .* exp(1j*nn*w0*t));

figure
plot(tt, double(subs(x(t), t, tt)), 'r', 'LineWidth', 1.5)
hold on
plot(tt, double(subs(f(t), t, tt)), 'b--', 'LineWidth', 1)
xlabel('t [s]'); ylabel('Signal')
title('Rekonstruktion des Rechtecksignals')
legend('Fourierreihe','Originalsignal'); grid on


%Kommentare wurden mit KI hinzugefügt