import sympy as sp

v1 = 490
v2 = 368
s = 3805

t = sp.symbols('t')

equation = (( s / (v1 + v2))) - t

t_solution = sp.solve(equation, t)

print(f"Time as fraction of hours: {t_solution[0]}")

def hhmmss(t_solution):
    t_value = float(t_solution[0])  
    total_seconds = int(t_value * 3600)  
    
    
    h = total_seconds // 3600  
    m = (total_seconds % 3600) // 60  
    s = (total_seconds % 3600) % 60  

    return f"{h:02d}:{m:02d}:{s:02d}"

print(hhmmss(t_solution))