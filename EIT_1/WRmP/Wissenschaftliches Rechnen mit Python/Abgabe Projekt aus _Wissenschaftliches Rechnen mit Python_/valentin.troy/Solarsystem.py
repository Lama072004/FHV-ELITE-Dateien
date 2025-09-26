import itertools  # Hilft bei der Auswahl verschiedener Farben für Planeten
import math       
import turtle     # Grafische Bibliothek zur Darstellung des Sonnensystems


#  Basis-Klasse für Himmelskörper (z.B. Sonne, Planeten, Monde)
# Diese Klasse beschreibt allgemeine Eigenschaften aller Körper im Sonnensystem.
class SolarSystemBody(turtle.Turtle):
    min_display_size = 25  # Mindestgröße, die ein Himmelskörper in der Darstellung haben kann
    display_log_base = 1.1  # Basis für die logarithmische Berechnung der Darstellungsgröße
    
    # Konstruktor: Erzeugt einen neuen Himmelskörper mit Masse, Position und Geschwindigkeit
    def __init__(self, solar_system, mass, position=(0, 0), velocity=(0, 0)):
        super().__init__()  # Ruft den Konstruktor der Basisklasse (turtle.Turtle) auf
        self.mass = mass  # Masse des Körpers
        self.setposition(position)  # Position im Raum
        self.velocity = velocity  # Anfangsgeschwindigkeit des Körpers

        # Die Darstellungsgröße wird anhand der Masse berechnet (logarithmische Skalierung)
        self.display_size = max(
            math.log(self.mass, self.display_log_base),
            self.min_display_size,
        )
        
        self.penup()  # Verhindert, dass eine Linie gezeichnet wird, wenn sich der Körper bewegt
        self.hideturtle()  # Versteckt den "Stift", sodass nur der Punkt gezeichnet wird
        
        solar_system.add_body(self)  # Fügt den Körper dem Sonnensystem hinzu
    
    # Methode, um den Körper als Punkt auf dem Bildschirm zu zeichnen
    def draw(self):
        self.clear()  # Löscht den vorherigen Punkt
        self.dot(self.display_size)  # Zeichnet den Punkt mit der berechneten Größe
    
    # Methode, um den Körper basierend auf seiner Geschwindigkeit zu bewegen
    def move(self):
        self.setx(self.xcor() + self.velocity[0])  # Aktualisiert die X-Position
        self.sety(self.ycor() + self.velocity[1])  # Aktualisiert die Y-Position


# Klasse für die Sonne (fixiert an einem Punkt)
class Sun(SolarSystemBody):
    def __init__(self, solar_system, mass, position=(0, 0), velocity=(0, 0)):
        super().__init__(solar_system, mass, position, velocity)
        self.color("yellow")  # Setzt die Farbe der Sonne auf Gelb
    
    # Die Sonne bewegt sich nicht, daher wird die Move-Methode überschrieben
    def move(self):
        pass


# Klasse für Planeten (beweglich und farbig)
class Planet(SolarSystemBody):
    colors = itertools.cycle(["brown", "orange", "blue", "red"])
    
    def __init__(self, solar_system, mass, position=(0, 0), velocity=(0, 0)):
        super().__init__(solar_system, mass, position, velocity)
        self.color(next(Planet.colors))  # Wählt die nächste Farbe aus der Liste aus


# Klasse für das Sonnensystem selbst (Verwaltung aller Himmelskörper)
class SolarSystem:
    def __init__(self, width, height):
        self.solar_system = turtle.Screen()  # Erzeugt einen Bildschirm für die Darstellung
        self.solar_system.tracer(0)  # Deaktiviert die automatische Aktualisierung des Bildschirms
        self.solar_system.setup(width, height)  # Setzt die Fenstergröße
        self.solar_system.bgcolor("black")  # Hintergrundfarbe schwarz (Weltraum)
        
        self.bodies = []  # Liste aller Himmelskörper im Sonnensystem
    
    # Methode zum Hinzufügen eines Körpers zum Sonnensystem
    def add_body(self, body):
        self.bodies.append(body)
    
    # Methode zum Entfernen eines Körpers aus dem Sonnensystem
    def remove_body(self, body):
        body.clear()  # Entfernt den Körper von der Anzeige
        self.bodies.remove(body)
    
    # Aktualisiert die Position und Darstellung aller Himmelskörper
    def update_all(self):
        for body in self.bodies:
            body.move()
            body.draw()
        self.solar_system.update()  # Aktualisiert den Bildschirm
    
    # Berechnet die Beschleunigung durch die Gravitationskraft zwischen zwei Körpern
    @staticmethod
        
    def accelerate_due_to_gravity(first: SolarSystemBody, second: SolarSystemBody):
        force =first.mass * second.mass / first.distance(second) ** 2  # Gravitationskraft
        angle = first.towards(second)  # Winkel zum anderen Körper
        reverse = 1  # Für den Richtungswechsel der Kräfte
        
        for body in first, second:
            acceleration = force / body.mass  # Beschleunigung = Kraft / Masse
            acc_x = acceleration * math.cos(math.radians(angle))  # X-Komponente der Beschleunigung
            acc_y = acceleration * math.sin(math.radians(angle))  # Y-Komponente der Beschleunigung
            body.velocity = (
                body.velocity[0] + (reverse * acc_x),
                body.velocity[1] + (reverse * acc_y),
            )
            reverse = -1  # Wechsel der Richtung für den zweiten Körper
    
    # Prüft, ob zwei Körper kollidieren, und entfernt sie ggf. aus dem System
    def check_collision(self, first, second):
        if isinstance(first, Planet) and isinstance(second, Planet):
            return  # Planeten kollidieren nicht miteinander
        
        # Wenn der Abstand kleiner ist als die Summe ihrer Radien, gibt es eine Kollision
        if first.distance(second) < first.display_size / 2 + second.display_size / 2:
            for body in first, second:
                if isinstance(body, Planet):
                    self.remove_body(body)
    
    # Berechnet alle Wechselwirkungen zwischen den Himmelskörpern
    def calculate_all_body_interactions(self):
        bodies_copy = self.bodies.copy()  # Kopie der Liste, um Probleme beim Entfernen zu vermeiden
        for idx, first in enumerate(bodies_copy):
            for second in bodies_copy[idx + 1:]:
                self.accelerate_due_to_gravity(first, second)  # Gravitationskraft
                self.check_collision(first, second)  # Kollisionsprüfung
