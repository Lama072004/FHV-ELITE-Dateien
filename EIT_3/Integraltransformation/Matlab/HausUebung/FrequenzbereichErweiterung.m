%% Systemdefinition
% Zähler: (s + 1e6) * (s^2 + 5600s + 1e8)
num = conv([1 1e6], [1 5600 1e8]);

% Nenner: (s + 1) * (s + 100) * (s + 1e8)^2
den = conv(conv([1 1], [1 100]), conv([1 1e8], [1 1e8]));

% Übertragungsfunktion
G = tf(num, den)

%% a) PN-Plan mit MATLAB
figure(1)
pzmap(G)
grid on
title('Pol-Nullstellen-Plan von G(s)')

% Pole und Nullstellen explizit berechnen
pole_sys = pole(G)
zero_sys = zero(G)

%% b) Bode-Diagramm
figure(2)
bode(G)
grid on
title('Bode-Diagramm von G(s)')

%% Zusätzlich: Amplitudengang allein für bessere Lesbarkeit
figure(3)
h = bodeplot(G);
setoptions(h, 'FreqUnits','Hz', 'PhaseVisible','off');
title('Amplitudengang von G(s)')
grid on

%% Frequenzgang auch als Betrag und Phase getrennt
w = logspace(0, 10, 1000); % Frequenzvektor von 1 bis 10^10 rad/s
[mag, phase] = bode(G, w);

figure(4)
subplot(2,1,1)
semilogx(w, 20*log10(squeeze(mag)))
grid on
ylabel('Betrag [dB]')
title('Amplitudengang')
xlabel('Frequenz [rad/s]')

subplot(2,1,2)
semilogx(w, squeeze(phase))
grid on
ylabel('Phase [°]')
xlabel('Frequenz [rad/s]')
title('Phasengang')

%% System-Info ausgeben
fprintf('Pole des Systems:\n');
disp(pole_sys);
fprintf('Nullstellen des Systems:\n');
disp(zero_sys);
fprintf('Statische Verstärkung: %.2e\n', dcgain(G));