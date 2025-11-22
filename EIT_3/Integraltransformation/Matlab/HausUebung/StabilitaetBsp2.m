clear
clc

syms t;

%% Plotten Impulsantwort

% Parameter
tau = 1;  % Zeitkonstante (z.B. 1 s)

% Übertragungsfunktion G(s) = 1 / (tau*s + 1)
s = tf('s');
G = 1 / (tau*s + 1);

% Impulsantwort plotten
figure;
impulse(G);
title('Impulsantwort g(t)');
grid on;

%% PN-Plan

figure;
pzmap(G);
title('Pol-Nullstellen-Plan des Systems');

%% Stabilität anhand des PN-Plans
p = pole(G);

if all(real(p) < 0)
    disp('System ist stabil.');
else
    disp('System ist instabil.');
end