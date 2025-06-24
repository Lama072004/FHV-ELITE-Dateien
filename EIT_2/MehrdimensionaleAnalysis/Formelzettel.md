Klar! Hier ist **die vollständige Markdown-Datei** in einem einzigen Block – einfach kopieren und in eine `.md`-Datei einfügen:

````markdown
# Mehrdimensionale Analysis – Komplette Formelsammlung

## 1. Grundlegende Operationen

### Vektoroperationen

**Betrag (Norm) eines Vektors:**
```math
\|\vec{v}\| = \sqrt{v_x^2 + v_y^2 + v_z^2}
````

**Kreuzprodukt:**

```math
\vec{a} \times \vec{b} = \begin{vmatrix}
\mathbf{i} & \mathbf{j} & \mathbf{k} \\
a_x & a_y & a_z \\
b_x & b_y & b_z \\
\end{vmatrix} = (a_yb_z - a_zb_y)\mathbf{i} - (a_xb_z - a_zb_x)\mathbf{j} + (a_xb_y - a_yb_x)\mathbf{k}
```

**Skalarprodukt:**

```math
\vec{a} \cdot \vec{b} = a_xb_x + a_yb_y + a_zb_z = \|\vec{a}\|\|\vec{b}\|\cos\theta
```

### Jacobi-Determinante

Für Koordinatentransformationen:

```math
J = \frac{\partial(x,y,z)}{\partial(u,v,w)} = \begin{vmatrix}
\frac{\partial x}{\partial u} & \frac{\partial x}{\partial v} & \frac{\partial x}{\partial w} \\
\frac{\partial y}{\partial u} & \frac{\partial y}{\partial v} & \frac{\partial y}{\partial w} \\
\frac{\partial z}{\partial u} & \frac{\partial z}{\partial v} & \frac{\partial z}{\partial w} \\
\end{vmatrix}
```

---

## 2. Differentialrechnung in ℝⁿ

### Gradient

```math
\nabla f = \left(\frac{\partial f}{\partial x}, \frac{\partial f}{\partial y}, \frac{\partial f}{\partial z}\right)
```

**Eigenschaften:**

* Zeigt in Richtung des steilsten Anstiegs
* Senkrecht zu Höhenlinien/Niveauflächen

### Richtungsableitung

```math
D_{\vec{v}}f = \nabla f \cdot \frac{\vec{v}}{\|\vec{v}\|} = \lim_{h\to 0}\frac{f(\vec{x} + h\vec{v}) - f(\vec{x})}{h}
```

### Jacobi-Matrix

Für \$\vec{F}: ℝ^n → ℝ^m\$:

```math
J_F = \begin{pmatrix}
\frac{\partial F_1}{\partial x_1} & \cdots & \frac{\partial F_1}{\partial x_n} \\
\vdots & \ddots & \vdots \\
\frac{\partial F_m}{\partial x_1} & \cdots & \frac{\partial F_m}{\partial x_n} \\
\end{pmatrix}
```

---

## 3. Integralrechnung in ℝⁿ

### Transformationssatz

```math
\iiint\limits_V f(x,y,z) dxdydz = \iiint\limits_{V'} f(\Phi(u,v,w)) |J_\Phi| dudvdw
```

### Typische Koordinatentransformationen

**Polarkoordinaten (2D):**

```math
x = r\cos\phi, \quad y = r\sin\phi, \quad dA = r dr d\phi
```

**Zylinderkoordinaten:**

```math
x = r\cos\phi, \quad y = r\sin\phi, \quad z = z, \quad dV = r dr d\phi dz
```

**Kugelkoordinaten:**

```math
x = r\sin\theta\cos\phi, \quad y = r\sin\theta\sin\phi, \quad z = r\cos\theta, \quad dV = r^2 \sin\theta dr d\theta d\phi
```

---

## 4. Vektoranalysis

### Differentialoperatoren

**Divergenz:**

```math
\nabla \cdot \vec{F} = \frac{\partial F_x}{\partial x} + \frac{\partial F_y}{\partial y} + \frac{\partial F_z}{\partial z}
```

**Rotation:**

```math
\nabla \times \vec{F} = \begin{vmatrix}
\mathbf{i} & \mathbf{j} & \mathbf{k} \\
\frac{\partial}{\partial x} & \frac{\partial}{\partial y} & \frac{\partial}{\partial z} \\
F_x & F_y & F_z \\
\end{vmatrix}
```

**Laplace-Operator:**

```math
\Delta f = \nabla \cdot (\nabla f) = \frac{\partial^2 f}{\partial x^2} + \frac{\partial^2 f}{\partial y^2} + \frac{\partial^2 f}{\partial z^2}
```

### Integralsätze

**Gaußscher Integralsatz:**

```math
\iiint\limits_V (\nabla \cdot \vec{F}) dV = \oiint\limits_{\partial V} \vec{F} \cdot d\vec{A}
```

**Stokesscher Satz:**

```math
\iint\limits_S (\nabla \times \vec{F}) \cdot d\vec{A} = \oint\limits_{\partial S} \vec{F} \cdot d\vec{r}
```

---

## 5. Praktische Tricks und Erkennungsmerkmale

### Wann ist ein Integral Null?

1. **Symmetrie-Argumente:**

   ```math
   \int_{-a}^a f(x) dx = 0 \quad \text{wenn } f(-x) = -f(x)
   ```

2. **Vektorfeld senkrecht zur Fläche:**

   ```math
   \iint \vec{F} \cdot d\vec{A} = 0 \quad \text{wenn } \vec{F} \perp \vec{n}
   ```

3. **Rotation eines Gradientenfeldes:**

   ```math
   \nabla \times (\nabla f) = 0
   ```

4. **Divergenz einer Rotation:**

   ```math
   \nabla \cdot (\nabla \times \vec{F}) = 0
   ```

### Parametervertauschung bei Mehrfachintegralen

**Strategie:**

1. Integrationsgebiet skizzieren
2. Neue Grenzen bestimmen

Beispiel:

```math
\int_0^1 \int_0^x f(x,y) dy dx = \int_0^1 \int_y^1 f(x,y) dx dy
```

---

## 6. Anwendungen

### Elektrostatik

**Coulomb-Potential:**

```math
\varphi(\vec{r}) = \frac{1}{4\pi\varepsilon_0} \frac{q}{\|\vec{r}\|}
```

**Elektrisches Feld:**

```math
\vec{E} = -\nabla \varphi
```

### Flussberechnungen

**Durch geschlossene Oberfläche:**

```math
\Phi = \oiint \vec{F} \cdot d\vec{A} = \iiint (\nabla \cdot \vec{F}) dV \quad \text{(Gauß)}
```

### Kurvenintegrale

**Arbeit eines Kraftfeldes:**

```math
W = \int_C \vec{F} \cdot d\vec{r}
```

---

## 7. Wichtige Parametrisierungen

### Kurven

**Gerade von A nach B:**

```math
\vec{r}(t) = \vec{A} + t(\vec{B}-\vec{A}), \quad t \in [0,1]
```

**Kreis (Radius R):**

```math
\vec{r}(t) = (R\cos t, R\sin t), \quad t \in [0,2\pi]
```

**Schraubenlinie:**

```math
\vec{r}(t) = (R\cos t, R\sin t, kt), \quad t \in [0,T]
```

### Flächen

**Ebene:**

```math
\vec{r}(u,v) = \vec{p} + u\vec{a} + v\vec{b}
```

**Zylindermantel:**

```math
\vec{r}(\phi,z) = (R\cos\phi, R\sin\phi, z)
```

**Kugeloberfläche:**

```math
\vec{r}(\theta,\phi) = (R\sin\theta\cos\phi, R\sin\theta\sin\phi, R\cos\theta)
```

---

## 8. Extremwertprobleme

### Ohne Nebenbedingungen

1. Gradient Null setzen: \$\nabla f = 0\$
2. Hessematrix bestimmen:

```math
H = \begin{pmatrix}
\frac{\partial^2 f}{\partial x^2} & \frac{\partial^2 f}{\partial x \partial y} \\
\frac{\partial^2 f}{\partial y \partial x} & \frac{\partial^2 f}{\partial y^2} \\
\end{pmatrix}
```

3. Definitheit prüfen:

   * det(H) > 0 und \$f\_{xx} > 0\$: Minimum
   * det(H) > 0 und \$f\_{xx} < 0\$: Maximum
   * det(H) < 0: Sattelpunkt

### Mit Nebenbedingungen (Lagrange)

```math
\nabla f = \lambda \nabla g \quad \text{und} \quad g(\vec{x}) = 0
```

---

## 9. Spezielle Techniken

### Integration durch Substitution

```math
\int f(g(x)) g'(x) dx = \int f(u) du \quad \text{mit } u = g(x)
```

### Partielle Integration

```math
\int u dv = uv - \int v du
```

### Fourier-Transformation (für PDEs)

```math
\hat{f}(\xi) = \int_{-\infty}^\infty f(x) e^{-2\pi i x\xi} dx
```

---

## 10. Wichtige Konstanten und Einheiten

* \$\varepsilon\_0 = 8{,}854 \times 10^{-12}\$ F/m (elektrische Feldkonstante)
* \$\mu\_0 = 4\pi \times 10^{-7}\$ N/A² (magnetische Feldkonstante)
* \$k = \frac{1}{4\pi\varepsilon\_0} = 8{,}988 \times 10^9\$ Nm²/C²

```

Wenn du willst, kann ich dir das auch als `.md` oder `.pdf` bereitstellen. Sag einfach Bescheid!
```
