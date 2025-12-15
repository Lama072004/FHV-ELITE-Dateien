# Python Befehle & Code-Patterns - Kompendium
## Statistik & Wahrscheinlichkeitsrechnung mit Python

Dieses Dokument enthält alle wichtigen Python-Befehle, Funktionen und Code-Patterns aus den Notebooks der Wahrscheinlichkeitsrechnung-Vorlesung.

---

## 1. IMPORTS - Standard-Bibliotheken

### Grundlegende Imports
```python
import pandas as pd                    # Datenmanipulation, DataFrames
import numpy as np                     # Numerische Berechnungen, Arrays
import matplotlib.pyplot as plt        # Plotting-Bibliothek
import seaborn as sea                  # Erweiterte Visualisierung (auch: sns, sn)
import scipy.stats as stats            # Statistische Funktionen
import statsmodels.api as sm           # Statistische Modelle, Regression
import warnings
warnings.filterwarnings("ignore")      # Warnungen unterdrücken
```

### Spezifische scipy.stats Imports
```python
from scipy.stats import norm           # Normalverteilung
from scipy.stats import t              # t-Verteilung (Student)
from scipy.stats import chi2           # Chi-Quadrat-Verteilung
from scipy.stats import binom          # Binomialverteilung
from scipy.stats import hypergeom      # Hypergeometrische Verteilung
from scipy.stats import gmean, hmean   # Geometrischer/Harmonischer Mittelwert
from scipy.stats import contingency    # Kontingenzanalyse
from scipy.stats import skew, kurtosis # Schiefe und Wölbung
from scipy.special import factorial    # Fakultät für Kombinatorik
```

### Machine Learning & Regression
```python
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import r2_score, mean_squared_error
from sklearn.linear_model import LinearRegression
```

### Weitere nützliche Imports
```python
import pingouin as pg                  # Statistische Tests (t-Tests)
from matplotlib import cm, colors      # Colormaps
import ipywidgets as widgets           # Interaktive Widgets
from IPython.display import display    # Display-Funktionen
```

---

## 2. DATENMANIPULATION - Pandas

### Daten Einlesen
```python
# CSV-Dateien
df = pd.read_csv('datei.csv')
df = pd.read_csv('datei.csv', index_col=0)  # Erste Spalte als Index

# Excel-Dateien
df = pd.read_excel('datei.xlsx')
df = pd.read_excel('datei.xlsx', index_col=0)
```

### DataFrame Grundoperationen
```python
# Übersicht
df.head()                              # Erste 5 Zeilen
df.head(3)                             # Erste 3 Zeilen
df.tail(5)                             # Letzte 5 Zeilen
df.shape                               # (Zeilen, Spalten)
df.info()                              # Datentypen und Info
df.columns                             # Spaltennamen
df.index                               # Index-Werte

# Datentypen
df.dtypes                              # Alle Datentypen
pd.api.types.is_numeric_dtype(df['col'])  # Prüfen ob numerisch

# Statistiken
df.describe()                          # Deskriptive Statistik
df['col'].value_counts()              # Häufigkeiten
df['col'].value_counts(normalize=True) # Relative Häufigkeiten
df['col'].value_counts().sort_index()  # Sortiert nach Index
df['col'].value_counts().sort_values() # Sortiert nach Werten
```

### Filtern und Auswählen
```python
# Spalten auswählen
df['Spalte']                           # Eine Spalte
df[['Spalte1', 'Spalte2']]            # Mehrere Spalten

# Zeilen filtern
df[df['Spalte'] > 100]                # Bedingung
df[(df['A'] > 5) & (df['B'] < 10)]    # UND-Verknüpfung
df[(df['A'] == 'X') | (df['B'] == 'Y')] # ODER-Verknüpfung

# loc und iloc
df.loc['Index']                        # Zugriff über Label
df.iloc[0]                            # Zugriff über Position
df.loc[:, 'Spalte']                   # Alle Zeilen, eine Spalte
df.iloc[:, 0]                         # Alle Zeilen, erste Spalte
```

### Gruppierung
```python
# Gruppierung nach Spalte
grouped = df.groupby('Kategorie')

# Iteration über Gruppen
for name, group in df.groupby('Species'):
    print(name)
    print(group.mean())

# Aggregation
df.groupby('Kategorie')['Wert'].mean()
df.groupby('Kategorie').agg(['mean', 'std', 'count'])
```

### Datenbereinigung
```python
# NaN-Werte
df.dropna()                            # Entfernt Zeilen mit NaN
df.dropna(subset=['Spalte1', 'Spalte2']) # Nur für bestimmte Spalten
df.fillna(0)                          # Ersetzt NaN mit 0
df['col'].fillna(0).astype(int)       # Ersetzen und Typ ändern

# Duplikate
df.duplicated().sum()                  # Anzahl Duplikate
df.drop_duplicates(keep='first')       # Entfernt Duplikate

# Index löschen
df.drop(indices)                       # Bestimmte Indices löschen
df.drop(df.index[0:5])                # Erste 5 Zeilen löschen
```

### Daten Transformieren
```python
# Neue Spalten
df['neue_spalte'] = df['A'] + df['B']
df['quadrat'] = df['Wert']**2

# Apply-Funktionen
df['col'].apply(lambda x: x*2)
df['col'].map({'alt': 'neu', 'a': 'b'})

# String-Operationen
df['Text'].str.lower()                 # Kleinbuchstaben
df['Text'].str.upper()                 # Großbuchstaben
df['Text'].str.strip()                 # Leerzeichen entfernen
df['Text'].str.split(';')             # Aufteilen

# Dummy-Variablen (One-Hot-Encoding)
pd.get_dummies(df['Kategorie'], dtype=int)
tmp = pd.get_dummies(df['Sex'], dtype=int)
df_new = pd.concat([df, tmp], axis=1)  # DataFrames zusammenführen
```

### Kreuztabellen
```python
# Einfache Kreuztabelle
ct = pd.crosstab(df['Zeile'], df['Spalte'])
ct = pd.crosstab(df['A'], df['B'], margins=True)  # Mit Randsummen

# Normalisierte Kreuztabellen
pd.crosstab(df['A'], df['B'], normalize=True)      # Gesamt
pd.crosstab(df['A'], df['B'], normalize='index')   # Zeilenweise
pd.crosstab(df['A'], df['B'], normalize='columns') # Spaltenweise
```

---

## 3. STATISTIK - Lagemaße und Streumaße

### Lagemaße
```python
# Mittelwert
df['col'].mean()                       # Arithmetisches Mittel
np.mean(data)                          # NumPy-Version
data.mean()                            # Auf Array

# Geometrischer und Harmonischer Mittelwert
from scipy.stats import gmean, hmean
gmean(values)                          # Geometrischer Mittelwert
hmean(values, weights=weights)         # Harmonischer Mittelwert

# Median und Quantile
df['col'].median()                     # Median (50%-Quantil)
np.median(data)                        # NumPy-Version
np.quantile(data, 0.25)               # 25%-Quantil
np.quantile(data, [0.25, 0.5, 0.75])  # Mehrere Quantile
np.quantile(data, q, method='nearest') # Mit Methode

# Modus
df['col'].mode()                       # Häufigster Wert
df['col'].mode().iloc[0]              # Erster Modus-Wert

# Min/Max
df['col'].min()                        # Minimum
df['col'].max()                        # Maximum
```

### Streumaße
```python
# Varianz und Standardabweichung
df['col'].var()                        # Varianz (n-1)
df['col'].var(ddof=0)                 # Varianz (n)
df['col'].std()                        # Standardabweichung (n-1)
df['col'].std(ddof=1)                 # Explizit (n-1)
np.std(data, ddof=1)                  # NumPy-Version

# Spannweite
range_val = df['col'].max() - df['col'].min()

# Interquartilsabstand (IQR)
q75 = np.quantile(data, 0.75)
q25 = np.quantile(data, 0.25)
iqr = q75 - q25
```

### Standardisierung
```python
# Manuelle Standardisierung
mean = data.mean()
std = data.std(ddof=1)
standardized = (data - mean) / std

# Mit sklearn
from sklearn.preprocessing import StandardScaler
scaler = StandardScaler()
scaler.fit(X_train)
X_train_scaled = scaler.transform(X_train)
X_test_scaled = scaler.transform(X_test)  # Mit selben Parametern!

# Als DataFrame
X_train_st = pd.DataFrame(scaler.transform(X_train), 
                          columns=X_train.columns, 
                          index=X_train.index)
```

### Korrelation und Kovarianz
```python
# Kovarianz
np.cov(x, y)                          # Kovarianzmatrix

# Korrelation
df.corr()                             # Korrelationsmatrix
df['A'].corr(df['B'])                 # Korrelation zwischen zwei Spalten
np.corrcoef(x, y)                     # NumPy-Version
```

### Schiefe und Wölbung
```python
from scipy.stats import skew, kurtosis

skewness = skew(data)                  # Schiefe
kurt = kurtosis(data, fisher=True)     # Wölbung (Excess Kurtosis)
```

### Kontingenzkoeffizient
```python
from scipy.stats import contingency

# Chi-Quadrat-Test
chi2, pValue, dof, expFreq = contingency.chi2_contingency(ktab, correction=False)

# Pearson-Kontingenzkoeffizient
npoints = df.shape[0]
nExpr = min(ktab.shape)
pearson = np.sqrt(chi2/(npoints+chi2))
pearson_korr = pearson * np.sqrt(nExpr/(nExpr-1))

# Alternativ mit association
pearson = contingency.association(ktab, method='pearson')
```

---

## 4. VISUALISIERUNG

### Matplotlib Grundlagen
```python
# Figure und Axes erstellen
fig, ax = plt.subplots(figsize=(10, 6))
fig, axs = plt.subplots(2, 2, figsize=(12, 10))  # 2x2 Grid

# Verschiedene Subplot-Syntaxen
ax1 = fig.add_subplot(111)             # Einzelner Plot
ax1 = fig.add_subplot(2, 2, 1)         # Erster von 2x2

# Plot-Befehle
ax.plot(x, y, label='Linie')          # Linienplot
ax.scatter(x, y)                      # Scatterplot
ax.bar(x, height, width=0.5)          # Balkendiagramm
ax.barh(y, width, height=0.5)         # Horizontale Balken
ax.hist(data, bins=20)                # Histogramm

# Beschriftungen
ax.set_xlabel('X-Achse')
ax.set_ylabel('Y-Achse')
ax.set_title('Titel')
ax.legend()                           # Legende anzeigen
ax.grid()                             # Gitternetz
ax.grid(True)

# Limits und Formatierung
ax.set_xlim(0, 100)
ax.set_ylim(0, 1)
xlims = ax.get_xlim()
ylims = ax.get_ylim()
ax.invert_yaxis()                     # Y-Achse umkehren

# Mehrere Linien
ax.plot(x, y1, label='Linie 1')
ax.plot(x, y2, linestyle='--', label='Linie 2', linewidth=2)
ax.plot(x, y3, marker='o', markevery=10, markersize=5)

# Farben
ax.plot(x, y, color='firebrick')
ax.plot(x, y, color='aqua')
ax.plot(x, y, color='tab:blue')

# Anzeigen
fig.autofmt_xdate()                   # X-Achsen-Labels rotieren
plt.tight_layout()                    # Layout optimieren
plt.show()
```

### Seaborn Plots
```python
# Scatterplot
sea.scatterplot(x=x, y=y, data=df, ax=ax)
sea.scatterplot(data=df, x='A', y='B', hue='Kategorie', ax=ax)

# Linienplot
sea.lineplot(x=x, y=y, ax=ax, label='Linie')
sea.lineplot(x=x, y=y, linestyle='--', ax=ax)
sea.lineplot(data=df, x='x', y='y', ax=ax, drawstyle='steps-mid')

# Histogramm
sea.histplot(data=df, x='Wert')
sea.histplot(data=df, x='Wert', bins=20, kde=True)

# Boxplot
sea.boxplot(data=df, x='Kategorie', y='Wert', ax=ax)

# Violin-Plot
sea.violinplot(data=df, x='Kategorie', y='Wert', ax=ax)

# Barplot
sea.barplot(data=df, x='Kategorie', y='Wert', ax=ax, color='red')

# KDE-Plot (Dichte)
sea.kdeplot(data=df, x='Wert', ax=ax, fill=True)
sea.kdeplot(data, ax=ax, label='Dichte')

# ECDF-Plot (Empirische Verteilungsfunktion)
sea.ecdfplot(data=df, x='Wert', ax=ax)
sea.ecdfplot(data=df, x='Wert', hue='Kategorie', ax=ax)

# Regression-Plot
sea.regplot(x=x_train, y=y_train, ax=ax)

# Pairplot
sea.pairplot(data=df, hue='Kategorie', corner=True, height=1.75)
sea.pairplot(data=df, kind='scatter', diag_kind='hist')
sea.pairplot(data=df, kind='kde', diag_kind='hist')

# Jointplot (mit Randverteilungen)
fig = sea.jointplot(data=df, x='A', y='B', kind='hist')
fig.ax_joint.set_xticklabels(fig.ax_joint.get_xticklabels(), rotation=45)
```

### Spezielle Plots
```python
# Histogramm mit matplotlib
ax.hist(data, bins=20, rwidth=0.75)
ax.hist(data, bins=20, density=True)          # Normalisiert
ax.hist(data, bins=20, cumulative=True)       # Kumulativ

# Tortendiagramm
ax.pie(values, labels=labels, startangle=90, autopct='%1.2f%%')

# Colorbar und Colormaps
from matplotlib import cm, colors
norm = colors.Normalize(vmin=minC, vmax=maxC, clip=True)
mapper = cm.ScalarMappable(norm=norm, cmap=cm.hot)
ax.scatter(x, y, c=mapper.to_rgba(z))
fig.colorbar(mapper, ax=ax, label='Z-Werte')

# QQ-Plot (Normalverteilung)
sm.qqplot(residuals, distribution, line='45', ax=ax)
```

---

## 5. WAHRSCHEINLICHKEITSRECHNUNG

### Kombinatorik
```python
from scipy.special import factorial

# Fakultät
n_fak = factorial(n)

# Binomialkoeffizient (n über k)
binom_coeff = factorial(n) / (factorial(k) * factorial(n-k))
```

### Zufallszahlen
```python
# Random Number Generator
rng = np.random.default_rng(seed=2024)  # Mit Seed für Reproduzierbarkeit

# Verschiedene Verteilungen
random_vals = rng.normal(loc=0, scale=1, size=100)  # Normalverteilung
random_ints = rng.integers(1, 7, size=100)          # Würfel (1-6)
random_uniform = rng.uniform(0, 1, size=100)        # Gleichverteilung

# Stichproben ziehen
sample = df.sample(n=30)                # 30 zufällige Zeilen
sample = df.sample(frac=0.8)            # 80% der Daten
sample = df.sample(n=30, random_state=42)  # Mit Seed
```

---

## 6. VERTEILUNGEN

### Normalverteilung
```python
from scipy.stats import norm

# Parameter
mu = 145      # Erwartungswert
sigma = 2     # Standardabweichung

# PDF (Probability Density Function)
x = np.linspace(mu - 4*sigma, mu + 4*sigma, 1000)
pdf = norm.pdf(x, loc=mu, scale=sigma)

# CDF (Cumulative Distribution Function)
cdf = norm.cdf(x, loc=mu, scale=sigma)

# Wahrscheinlichkeiten berechnen
p_less_145 = norm.cdf(145, loc=mu, scale=sigma)
p_greater_155 = 1 - norm.cdf(155, loc=mu, scale=sigma)
p_between = norm.cdf(155, loc=mu, scale=sigma) - norm.cdf(150, loc=mu, scale=sigma)

# Quantile (inverse CDF)
x_90 = norm.ppf(0.9, loc=mu, scale=sigma)  # 90%-Quantil
z_value = norm.ppf(1 - alpha/2)            # z-Wert für Konfidenzintervall

# Eigenschaften
mean = norm.mean(loc=mu, scale=sigma)      # = mu
std = norm.std(loc=mu, scale=sigma)        # = sigma

# Standardnormalverteilung
z = (x - mu) / sigma                       # Standardisierung
p = norm.cdf(z, loc=0, scale=1)           # N(0,1)
```

### t-Verteilung (Student)
```python
from scipy.stats import t

# Parameter
df = n - 1    # Freiheitsgrade

# PDF und CDF
pdf = t.pdf(x, df=df)
cdf = t.cdf(x, df=df)

# Quantile
t_crit = t.ppf(1 - alpha/2, df=df)        # Kritischer Wert

# Eigenschaften
mean = t.mean(df=df)                       # Nur für df > 1
std = t.std(df=df)                         # Nur für df > 2
skew = t.stats(df=df, moments='s')        # Schiefe (df > 3)
kurt = t.stats(df=df, moments='k')        # Wölbung (df > 4)
```

### Chi-Quadrat-Verteilung
```python
from scipy.stats import chi2

# Parameter
df = n - 1    # Freiheitsgrade

# PDF und CDF
pdf = chi2.pdf(x, df=df)
cdf = chi2.cdf(x, df=df)

# Quantile
chi2_lower = chi2.ppf(alpha/2, df=df)
chi2_upper = chi2.ppf(1 - alpha/2, df=df)

# Eigenschaften
mean = chi2.mean(df=df)                    # = df
std = chi2.std(df=df)                      # = sqrt(2*df)
```

### Binomialverteilung
```python
from scipy.stats import binom

# Parameter
n = 100       # Anzahl Versuche
p = 0.05      # Erfolgswahrscheinlichkeit

# PMF (Probability Mass Function)
k = np.arange(0, n+1)
pmf = binom.pmf(k, n, p)                   # P(X = k)

# CDF
cdf = binom.cdf(k, n, p)                   # P(X ≤ k)

# Wahrscheinlichkeiten
p_exactly_5 = binom.pmf(5, n, p)
p_at_most_5 = binom.cdf(5, n, p)
p_at_least_5 = 1 - binom.cdf(4, n, p)
p_between = binom.cdf(10, n, p) - binom.cdf(5, n, p)

# Erwartungswert und Varianz
mean = n * p
variance = n * p * (1 - p)
std = np.sqrt(variance)
```

### Hypergeometrische Verteilung
```python
from scipy.stats import hypergeom

# Parameter
N = 100       # Grundgesamtheit
M = 5         # Anzahl Erfolge in Grundgesamtheit
n = 3         # Stichprobengröße

# PMF und CDF
k = np.arange(0, n+1)
pmf = hypergeom.pmf(k, N, M, n)
cdf = hypergeom.cdf(k, N, M, n)

# Erwartungswert
expected = n * M / N
```

### Normalapproximation der Binomialverteilung
```python
# Voraussetzung prüfen: n*p > 5 und n*(1-p) > 5

# Parameter
n = 500
p = 0.04
mean = n * p
std = np.sqrt(n * p * (1 - p))

# Mit Stetigkeitskorrektur
# P(a ≤ X ≤ b) ≈ P(a-0.5 ≤ X ≤ b+0.5)
p_approx = norm.cdf((b + 0.5 - mean) / std) - norm.cdf((a - 0.5 - mean) / std)
```

---

## 7. KONFIDENZINTERVALLE UND TESTS

### Konfidenzintervall für Mittelwert (σ bekannt)
```python
# Parameter
alpha = 0.05
n = len(data)
x_bar = np.mean(data)
sigma = 0.1   # Bekannte Standardabweichung

# Kritischer Wert (Normalverteilung)
z = norm.ppf(1 - alpha/2)

# Konfidenzintervall
margin = z * sigma / np.sqrt(n)
ci_lower = x_bar - margin
ci_upper = x_bar + margin

print(f"95%-KI: [{ci_lower:.4f}, {ci_upper:.4f}]")
```

### Konfidenzintervall für Mittelwert (σ unbekannt)
```python
# Parameter
alpha = 0.05
n = len(data)
x_bar = np.mean(data)
s = np.std(data, ddof=1)   # Geschätzte Standardabweichung

# Kritischer Wert (t-Verteilung)
t_crit = t.ppf(1 - alpha/2, df=n-1)

# Konfidenzintervall
margin = t_crit * s / np.sqrt(n)
ci_lower = x_bar - margin
ci_upper = x_bar + margin

print(f"95%-KI: [{ci_lower:.4f}, {ci_upper:.4f}]")
```

### Konfidenzintervall für Standardabweichung
```python
# Parameter
alpha = 0.05
n = len(data)
s = np.std(data, ddof=1)

# Kritische Werte (Chi-Quadrat)
chi2_lower = chi2.ppf(alpha/2, df=n-1)
chi2_upper = chi2.ppf(1 - alpha/2, df=n-1)

# Konfidenzintervall für σ
sigma_lower = np.sqrt((n-1) * s**2 / chi2_upper)
sigma_upper = np.sqrt((n-1) * s**2 / chi2_lower)

print(f"95%-KI für σ: [{sigma_lower:.4f}, {sigma_upper:.4f}]")
```

### t-Test (mit pingouin)
```python
import pingouin as pg

# Einstichproben-t-Test
mu0 = 190  # Hypothetischer Mittelwert
result = pg.ttest(data, mu0)
display(result[["T", "p-val", "CI95%"]])

# Interpretation
if result["p-val"].values[0] < 0.05:
    print("H0 wird verworfen (signifikant)")
else:
    print("H0 wird nicht verworfen")

# Zweistichproben-t-Test
result = pg.ttest(group1, group2)
```

---

## 8. REGRESSION

### Train-Test-Split
```python
from sklearn.model_selection import train_test_split

# Daten aufteilen
X_train, X_test, y_train, y_test = train_test_split(
    X, y, 
    train_size=0.8,      # 80% Training, 20% Test
    random_state=1234    # Für Reproduzierbarkeit
)

print(f'Training: {X_train.shape[0]}, Test: {X_test.shape[0]}')
```

### Einfache Lineare Regression (statsmodels)
```python
import statsmodels.api as sm

# Feature und Target
X = df['Feature']
y = df['Target']

# Train-Test-Split
X_train, X_test, y_train, y_test = train_test_split(X, y, train_size=0.8)

# Konstante hinzufügen (Intercept)
X_train_const = sm.add_constant(X_train, prepend=True)

# Modell erstellen und fitten
model = sm.OLS(y_train, X_train_const)
result = model.fit()

# Zusammenfassung
display(result.summary())

# Vorhersage
X_test_const = sm.add_constant(X_test, prepend=True)
y_pred = result.predict(X_test_const)

# R²
print(f"R²: {result.rsquared:.4f}")

# Residuen
residuals = result.resid
fitted_values = result.fittedvalues
```

### Multiple Lineare Regression
```python
# Mehrere Features
features = ['Feature1', 'Feature2', 'Feature3']
X = df[features]
y = df['Target']

# Train-Test-Split
X_train, X_test, y_train, y_test = train_test_split(X, y, train_size=0.8)

# Standardisierung (optional aber empfohlen)
scaler = StandardScaler()
scaler.fit(X_train)
X_train_scaled = pd.DataFrame(
    scaler.transform(X_train),
    columns=X_train.columns,
    index=X_train.index
)
X_test_scaled = pd.DataFrame(
    scaler.transform(X_test),
    columns=X_test.columns,
    index=X_test.index
)

# Regression
X_train_const = sm.add_constant(X_train_scaled, prepend=True)
model = sm.OLS(y_train, X_train_const)
result = model.fit()
display(result.summary())

# Vorhersage und Bewertung
X_test_const = sm.add_constant(X_test_scaled, prepend=True)
y_pred = result.predict(X_test_const)

# Adjustiertes R² (manuell)
n = y_test.shape[0]
p = len(features)
r2_adj = 1 - (n - 1)/(n - 1 - p) * (1 - r2_score(y_test, y_pred))
print(f'R² Adjusted (Test): {r2_adj:.4f}')
```

### Regression mit kategorischen Variablen
```python
# One-Hot-Encoding
dummies = pd.get_dummies(df['Kategorie'], dtype=int)
df_new = pd.concat([df, dummies], axis=1)

# Features kombinieren
features_metric = ['Feature1', 'Feature2']
features_categorical = ['KATEGORIE_A', 'KATEGORIE_B']
all_features = features_metric + features_categorical

# Standardisierung nur für metrische Features
scaler = StandardScaler()
scaler.fit(X_train[features_metric])
X_train_scaled = pd.DataFrame(
    scaler.transform(X_train[features_metric]),
    columns=features_metric,
    index=X_train.index
)

# Kategorische Spalten hinzufügen
for cat in features_categorical:
    X_train_scaled[cat] = X_train[cat].values
```

### Nicht-lineare Features
```python
# Polynomielle Features
df['feature_square'] = df['Feature']**2
df['feature_cube'] = df['Feature']**3

# Interaktionen
df['feature1_x_feature2'] = df['Feature1'] * df['Feature2']

# Logarithmus
df['log_feature'] = np.log(df['Feature'])
```

### Regression mit sklearn
```python
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import cross_val_score

# Daten vorbereiten
X_train_vec = X_train.values.reshape(-1, 1)
y_train_vec = y_train.values.reshape(-1, 1)

# Modell fitten
reg = LinearRegression().fit(X_train_vec, y_train_vec)

# Score
print(f"R²: {reg.score(X_train_vec, y_train_vec):.4f}")

# Koeffizienten
print(f"Slope: {reg.coef_[0][0]:.4f}")
print(f"Intercept: {reg.intercept_[0]:.4f}")

# Cross-Validation
scores = cross_val_score(reg, X_train_vec, y_train_vec, cv=5)
print(f"CV R² scores: {scores}")
print(f"Mean CV R²: {scores.mean():.4f}")
```

### Residuenanalyse
```python
# Residuen-Plot
fig, ax = plt.subplots(figsize=(10, 5))
sea.scatterplot(x=X_train, y=result.resid, ax=ax)
ax.axhline(0, linestyle='--', color='red')
ax.set_xlabel('Feature')
ax.set_ylabel('Residuen')
ax.grid()

# QQ-Plot für Normalverteilung der Residuen
rv = stats.norm(loc=0, scale=np.std(result.resid, ddof=2))
sm.qqplot(result.resid, rv, line='45', ax=ax)

# KDE-Plot der Residuen
sea.kdeplot(result.resid, ax=ax)
```

---

## 9. NÜTZLICHE CODE-PATTERNS

### DataFrame Zusammenfassung erstellen
```python
# Leeres DataFrame mit Index und Spalten
report = pd.DataFrame(
    index=['Kategorie1', 'Kategorie2'],
    columns=['Metrik1', 'Metrik2', 'Metrik3']
)

# Befüllen
for category, group in df.groupby('Category'):
    report.loc[category, 'Mean'] = group['Value'].mean()
    report.loc[category, 'Std'] = group['Value'].std()

display(report)
```

### Formatierte Ausgabe
```python
# f-Strings
print(f"Wert: {value:.2f}")           # 2 Dezimalstellen
print(f"Wert: {value:.4f}")           # 4 Dezimalstellen
print(f"Prozent: {value:.2%}")        # Als Prozent
print(f"Wissenschaftlich: {value:.4g}") # Wissenschaftliche Notation

# Mit Pandas
pd.options.display.precision = 4       # Genauigkeit setzen
```

### Schleifen über Gruppen
```python
# Gruppierung und Iteration
for species, group in df.groupby('Species'):
    print(f"\n{species}")
    mean = group['Value'].mean()
    std = group['Value'].std()
    print(f"Mean: {mean:.2f}, Std: {std:.2f}")
```

### Bedingte Formatierung
```python
# Werte basierend auf Bedingung setzen
df['Category'] = df['Value'].apply(lambda x: 'High' if x > 100 else 'Low')

# Map mit Dictionary
mapping = {'A': 1, 'B': 2, 'C': 3}
df['Numeric'] = df['Category'].map(mapping)

# Replace
df['Column'].replace({'old': 'new', 'x': 'y'}, inplace=True)
```

### Mehrere Plots in einem Figure
```python
# Grid von Subplots
fig, axs = plt.subplots(2, 2, figsize=(12, 10))

# Zugriff
axs[0, 0].plot(x, y)      # Erste Zeile, erste Spalte
axs[0, 1].scatter(x, y)   # Erste Zeile, zweite Spalte
axs[1, 0].hist(data)      # Zweite Zeile, erste Spalte

# Gemeinsame Achsen
fig, axs = plt.subplots(2, 2, sharex=True, sharey=True)

# Titel für gesamte Figure
fig.suptitle('Haupttitel')

plt.tight_layout()
plt.show()
```

### Interaktive Widgets (Jupyter)
```python
import ipywidgets as widgets
from IPython.display import display

# Slider
slider = widgets.IntSlider(
    value=10,
    min=1,
    max=100,
    step=1,
    description='Parameter:',
    continuous_update=False
)

# Button
button = widgets.Button(description="Berechnen")

def on_button_click(b):
    print(f"Slider-Wert: {slider.value}")

button.on_click(on_button_click)

# Anzeigen
display(slider, button)
```

### Zeitmessung
```python
import time

start = time.time()
# ... Code ...
end = time.time()
print(f"Laufzeit: {end - start:.2f} Sekunden")
```

---

## 10. HÄUFIGE FEHLER UND LÖSUNGEN

### Problem: SettingWithCopyWarning
```python
# FALSCH
subset = df[df['A'] > 5]
subset['B'] = 10  # Warning!

# RICHTIG
subset = df[df['A'] > 5].copy()
subset['B'] = 10

# ODER mit loc
df.loc[df['A'] > 5, 'B'] = 10
```

### Problem: Index zurücksetzen
```python
# Nach Filterung Index zurücksetzen
df_filtered = df[df['A'] > 5].reset_index(drop=True)
```

### Problem: Datentypen konvertieren
```python
# String zu numerisch
df['Zahl'] = pd.to_numeric(df['String'], errors='coerce')

# String zu Datum
df['Datum'] = pd.to_datetime(df['String'])

# Explizite Konvertierung
df['Int'] = df['Float'].astype(int)
```

### Problem: NaN-Werte in Berechnungen
```python
# NaN ignorieren
df['Column'].dropna().mean()

# NaN mit Wert ersetzen
df['Column'].fillna(0).mean()
```

---

## ENDE

**Hinweis:** Dieses Kompendium basiert auf den Jupyter Notebooks der Vorlesung "Wahrscheinlichkeitsrechnung und Statistik" und deckt alle wichtigen Befehle und Patterns ab, die im Kurs verwendet wurden.

**Datum:** Dezember 2025
