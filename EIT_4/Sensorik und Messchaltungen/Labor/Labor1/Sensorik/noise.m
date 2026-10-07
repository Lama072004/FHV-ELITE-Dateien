time1 = NOISE1.Time;
voltage1 = NOISE1.Voltage;

figure;
histfit(voltage1');
pd1=fitdist(voltage1,'normal');
sigma1 = pd1.sigma;
mu1 = pd1.mu;
Vpp1=6*sigma;
disp(Vpp1); % Peak−Peak Spannung
disp(mu1); % Mittelwert
disp(sigma1); % Standardabweichung
fprintf('Vpp1: %e\n', Vpp1);
fprintf('mu1: %e\n', mu1);
fprintf('sigma1: %e\n', sigma1);

time2 = NOISE2.Time;
voltage2 = NOISE2.Voltage;

figure;
histfit(voltage2');
pd2=fitdist(voltage2,'normal');
sigma2 = pd2.sigma;
mu2 = pd2.mu;
Vpp2=6*sigma;
disp(Vpp2); % Peak−Peak Spannung
disp(mu2); % Mittelwert
disp(sigma2); % Standardabweichung
fprintf('Vpp2: %e\n', Vpp2);
fprintf('mu2: %e\n', mu2);
fprintf('sigma2: %e\n', sigma2);

time3 = NOISE3.Time;
voltage3 = NOISE3.Voltage;

figure;
histfit(voltage3');
pd3=fitdist(voltage3,'normal');
sigma3 = pd3.sigma;
mu3 = pd3.mu;
Vpp3=6*sigma;
disp(Vpp3); % Peak−Peak Spannung
disp(mu3); % Mittelwert
disp(sigma3); % Standardabweichung
fprintf('Vpp3: %e\n', Vpp3);
fprintf('mu3: %e\n', mu3);
fprintf('sigma3: %e\n', sigma3);


time4 = NOISE4.Time;
voltage4 = NOISE4.Voltage;

figure;
histfit(voltage4');
pd4 = fitdist(voltage4,'normal');
sigma4 = pd4.sigma;
mu4 = pd4.mu;
Vpp4 = 6*sigma4;

disp(Vpp4); % Peak−Peak Spannung
disp(mu4); % Mittelwert
disp(sigma4); % Standardabweichung

fprintf('Vpp4: %e\n', Vpp4);
fprintf('mu4: %e\n', mu4);
fprintf('sigma4: %e\n', sigma4);

time5 = NOISE5.Time;
voltage5 = NOISE5.Voltage;

figure;
histfit(voltage5');
pd5 = fitdist(voltage5,'normal');
sigma5 = pd5.sigma;
mu5 = pd5.mu;
Vpp5 = 6*sigma5;

disp(Vpp5); % Peak−Peak Spannung
disp(mu5); % Mittelwert
disp(sigma5); % Standardabweichung

fprintf('Vpp5: %e\n', Vpp5);
fprintf('mu5: %e\n', mu5);
fprintf('sigma5: %e\n', sigma5);