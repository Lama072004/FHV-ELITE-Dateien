%% -------------------------------
%  Übertragungsfunktion: G(s) = (1/5)*(s-200)/((s+2)*(s-20))
%% -------------------------------

clc; clear; close all;

% Zähler und Nenner polynome
num = 100*[1 +0.1];       % Zähler: (1/5)*(s - 200)
den = 1*(conv([1 1], [1 100])); % Nenner: (s+2)*(s-20)

% Übertragungsfunktion erstellen
G = tf(num, den);

disp('Übertragungsfunktion G(s):');
G

%% -------------------------------
%  Pol-Nullstellen-Plan
%% -------------------------------
figure('Name', 'Pol-Nullstellen-Plan');
pzmap(G);
grid on;
title('Pol-Nullstellen-Plan (PN-Plan)');

%% -------------------------------
%  Amplituden- und Phasengang (Bode)
%% -------------------------------
figure('Name', 'Bode-Diagramm');
bode(G);
grid on;
title('Amplituden- und Phasengang');

%% -------------------------------
%  Optional: Nyquist-Diagramm
%% -------------------------------
figure('Name', 'Nyquist-Diagramm');
nyquist(G);
grid on;
title('Nyquist-Diagramm');
