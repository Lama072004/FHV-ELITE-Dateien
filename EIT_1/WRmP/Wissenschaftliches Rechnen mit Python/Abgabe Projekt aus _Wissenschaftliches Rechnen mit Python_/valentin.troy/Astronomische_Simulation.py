#Projekt Astronomische Simulation

#Simulation des inneren Sonnensystems, Simulation der Bahnen der Planeten mit Hilfe des Newtonschen Gravitationsgesetzes.

#Projektaufwand max. 12h
#Tatsächlicher Aufwand 10h




# Importiert die Klassen für das Sonnensystem, die Sonne und die Planeten
from Solarsystem import SolarSystem, Sun, Planet
import time  # Verzögerung der Simulationsgeschwindigkeit

# Erzeugt ein neues Sonnensystem mit einer Fenstergröße von 1400 x 900 Pixeln
solar_system = SolarSystem(width=1920, height=1080)

# Erzeugt die Sonne mit einer großen Masse
sun = Sun(solar_system, mass= 10_000)

# Erzeugt mehrere Planeten mit unterschiedlichen Massen, Positionen und Geschwindigkeiten
planets = (
    # Merkur
    Planet(
        solar_system,
        mass=0.055,  # Masse des Planeten relativ zur Erde (3,30 x 10^23 kg)
        position=(-120, 0),  # Anfangsposition auf der x-Achse (0.4 AU)
        velocity=(0, 10),  # Anfangsgeschwindigkeit in Richtung der y-Achse (172.332 km/h = 10 Simulationseinheiten)
    ),
    # Venus
    Planet(
        solar_system,
        mass=0.815,  # Masse des Planeten relativ zur Erde (4,87 x 10^24 kg)
        position=(-215, 0),  # Anfangsposition auf der x-Achse (0.7 AU)
        velocity=(0, 7.4),  # Anfangsgeschwindigkeit (126.072 km/h)
    ),
    # Erde
    Planet(
        solar_system,
        mass=1,  # Masse des Planeten (5,97 x 10^24 kg = 1 Simulationsmasse)
        position=(-290, 0),  # Anfangsposition auf der x-Achse (-290 = 1 AU Referenzabstand)
        velocity=(0, 6.2),  # Anfangsgeschwindigkeit (107.208 km/h)
    ),
    # Mars
    Planet(
        solar_system,
        mass=0.107,  # Masse des Planeten relativ zur Erde (6,42 x 10^23 kg)
        position=(-440, 0),  # Anfangsposition auf der x-Achse (1.5 AU)
        velocity=(0, 5),  # Anfangsgeschwindigkeit (86.868 km/h)
    ),
)

# Endlosschleife zur Simulation des Sonnensystems
while True:
    # Berechnet alle Gravitationskräfte und Interaktionen zwischen den Himmelskörpern
    solar_system.calculate_all_body_interactions()

    # Aktualisiert die Positionen der Himmelskörper und zeichnet sie neu
    solar_system.update_all()
    
    # Fügt eine kurze Pause von 0,03 Sekunden ein, um die Simulation zu verlangsamen
    time.sleep(0.03)
    
    
    