from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QLineEdit, QLabel, QPushButton, QScrollArea, QWidget
from PyQt5.QtGui import QPen, QColor, QPainter
from PyQt5.QtCore import Qt, QTimer, QPointF
import numpy as np
import sys
import cmath


#Dieses Gui akzeptiert die Eingabe komplexer Zahlen kartesisch und Polar in den Formen:
    #z.B. 50+60j in kartesicher Form, 120*exp(1j) Polarform mit Bogenmaß, 120*exp(120gj) Polarform in Gradmaß
class MyWindow(QMainWindow):
    def __init__(self, xpos, ypos, width, height):
        super(MyWindow, self).__init__()
        # Initialisierung des Hauptfensters mit Position und Größe
        self.setGeometry(xpos, ypos, width, height)
        self.setWindowTitle("AC Visual")  # Fenstertitel
        self.initUI()  # Benutzeroberfläche erstellen

    def initUI(self):
        # Hauptlayout für das Fenster
        self.central_widget = QWidget(self)
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        # Hinzufügen von Labels und Eingabefeldern
        self.addLabel(20, 20, 55, 16, "Phase 1")
        self.addLabel(20, 60, 55, 16, "Phase 2")
        self.addLabel(20, 100, 55, 16, "Phase 3")
        self.addLabel(20, 140, 55, 16, "Frequenz")

        self.phase1_input = self.addInputLine(80, 20, 113, 22)  # Eingabe für Phase 1
        self.phase2_input = self.addInputLine(80, 60, 113, 22)  # Eingabe für Phase 2
        self.phase3_input = self.addInputLine(80, 100, 113, 22)  # Eingabe für Phase 3
        self.frequency_input = self.addInputLine(80, 140, 113, 22)  # Eingabe für Frequenz

        # Hinzufügen von Buttons und deren Funktionen
        self.addButton(20, 180, 120, 28, "Animation starten", self.start_animation)
        self.addButton(20, 220, 150, 28, "Resultierende animieren", self.calculate_resultant)
        self.addButton(20, 260, 150, 28, "Animationen beenden", self.stop_animation)

        # Grafikbereiche für Animationen
        self.pointer_area = self.addGraphicArea(0, 0, 1921, 400)  # Bereich für Zeigeranimation
        self.sinus_area = self.addGraphicArea(0, 400, 1921, 680)  # Bereich für Sinuskurvenanimation

        # Zuweisung von Widgets zu den Grafikbereichen
        self.pointer_widget = AnimationWidget()
        self.pointer_area.setWidget(self.pointer_widget)

        self.sinus_widget = AnimationWidget()
        self.sinus_area.setWidget(self.sinus_widget)

        # Hinzufügen der Grafikbereiche zum Hauptlayout
        self.layout.addWidget(self.pointer_area)
        self.layout.addWidget(self.sinus_area)

    def addInputLine(self, x, y, width, height):
        # Erstellt ein Eingabefeld an der angegebenen Position und Größe
        line = QLineEdit(self)
        line.setGeometry(QtCore.QRect(x, y, width, height))
        return line

    def addLabel(self, x, y, width, height, string) -> QLabel:
        # Erstellt ein Textlabel an der angegebenen Position und Größe
        label = QLabel(self)
        label.setGeometry(QtCore.QRect(x, y, width, height))
        label.setText(string)
        return label

    def addButton(self, x, y, width, height, string, func) -> QPushButton:
        # Erstellt einen Button an der angegebenen Position und verbindet ihn mit einer Funktion
        button = QPushButton(self)
        button.setText(string)
        button.setGeometry(QtCore.QRect(x, y, width, height))
        button.clicked.connect(func)
        return button

    def addGraphicArea(self, x, y, width, height) -> QScrollArea:
        # Erstellt einen Scrollbereich an der angegebenen Position und Größe
        area = QScrollArea(self)
        area.setGeometry(QtCore.QRect(x, y, width, height))
        area.setWidgetResizable(True)  # Inhalt wird skaliert
        return area

    def start_animation(self):
        # Startet die Animation basierend auf den eingegebenen Werten
        phase1 = self.parse_complex(self.phase1_input.text())
        phase2 = self.parse_complex(self.phase2_input.text())
        phase3 = self.parse_complex(self.phase3_input.text())
        frequency = float(self.frequency_input.text())

        # Hinzufügen von Zeigern und Sinuskurven
        self.pointer_widget.add_pointer(QColor(Qt.green), frequency, abs(phase1), np.angle(phase1, deg=True), self.pointer_area.width() / 2, self.pointer_area.height() / 2)
        self.pointer_widget.add_pointer(QColor(Qt.yellow), frequency, abs(phase2), np.angle(phase2, deg=True), self.pointer_area.width() / 2, self.pointer_area.height() / 2)
        self.pointer_widget.add_pointer(QColor(Qt.red), frequency, abs(phase3), np.angle(phase3, deg=True), self.pointer_area.width() / 2, self.pointer_area.height() / 2)

        self.sinus_widget.add_sinus_curve(QColor(Qt.green), frequency, abs(phase1), np.angle(phase1, deg=True), 0, self.sinus_area.height() / 2)
        self.sinus_widget.add_sinus_curve(QColor(Qt.yellow), frequency, abs(phase2), np.angle(phase2, deg=True), 0, self.sinus_area.height() / 2)
        self.sinus_widget.add_sinus_curve(QColor(Qt.red), frequency, abs(phase3), np.angle(phase3, deg=True), 0, self.sinus_area.height() / 2)

    def calculate_resultant(self):
        # Berechnet die resultierende komplexe Zahl aus den Phasen
        phase1 = self.parse_complex(self.phase1_input.text())
        phase2 = self.parse_complex(self.phase2_input.text())
        phase3 = self.parse_complex(self.phase3_input.text())
        frequency = float(self.frequency_input.text())

        resultant = phase1 + phase2 + phase3  # Summe der Phasen

        # Löschen der alten Animationen
        self.pointer_widget.clear_pointers()
        self.sinus_widget.clear_sinus_curves()

        # Hinzufügen der resultierenden Animation
        self.pointer_widget.add_pointer(QColor(Qt.blue), frequency, abs(resultant), np.angle(resultant, deg=True), self.pointer_area.width() / 2, self.pointer_area.height() / 2)
        self.sinus_widget.add_sinus_curve(QColor(Qt.blue), frequency, abs(resultant), np.angle(resultant, deg=True), 0, self.sinus_area.height() / 2)

    def stop_animation(self):
        # Beendet die Animation, indem die Zeiger und Kurven gelöscht werden
        self.pointer_widget.clear_pointers()
        self.sinus_widget.clear_sinus_curves()

    def parse_complex(self, text):
        # Parst eine komplexe Zahl aus einem Textstring
        if 'exp' in text:
            parts = text.split('*')
            if len(parts) == 2 and 'j' in parts[1]:
                r = float(parts[0])  # Betrag
                exp_part = parts[1].replace('exp', '').replace('(', '').replace(')', '').replace('j', '')
                if 'g' in exp_part:
                    exp_part = exp_part.replace('g', '')
                    theta = float(exp_part)
                    theta = np.radians(theta)  # Grad in Bogenmaß umwandeln
                else:
                    theta = float(exp_part)
                return cmath.rect(r, theta)  # Umwandlung in komplexe Zahl
            else:
                raise ValueError("Ungültiges Format für komplexe Zahl mit 'exp'.")
        else:
            return complex(text)  # Standardkomplexe Zahl parsen

class AnimationWidget(QWidget):
    def __init__(self):
        super().__init__()
        # Initialisierung der Listen für Sinuskurven und Zeiger
        self.sinus_curves = []
        self.pointers = []
        self.time = 0  # Zeitvariable
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_animation)  # Aktualisierungsintervall
        self.timer.start(20)  # 50 FPS
        self.history_length = 400  # Länge der Sinuskurven-Historie

    def add_sinus_curve(self, color, frequency, amplitude, phase, x_offset, y_offset):
        # Fügt eine neue Sinuskurve zur Liste hinzu
        sinus_curve = {
            'color': color,            # Farbe der Kurve
            'frequency': frequency,    # Frequenz der Kurve
            'amplitude': amplitude,    # Amplitude der Kurve
            'phase': phase,            # Phase der Kurve
            'x_offset': x_offset,      # Horizontale Verschiebung
            'y_offset': y_offset,      # Vertikale Verschiebung
            'values': [0] * self.history_length  # Initialisierte Werte für die Kurve
        }
        self.sinus_curves.append(sinus_curve)  # Hinzufügen zur Liste

    def add_pointer(self, color, frequency, amplitude, phase, x_center, y_center):
        # Fügt einen neuen Zeiger zur Liste hinzu
        pointer = {
            'color': color,            # Farbe des Zeigers
            'frequency': frequency,    # Frequenz des Zeigers
            'amplitude': amplitude,    # Länge des Zeigers
            'phase': phase,            # Phase des Zeigers
            'x_center': x_center,      # X-Position des Drehzentrums
            'y_center': y_center       # Y-Position des Drehzentrums
        }
        self.pointers.append(pointer)  # Hinzufügen zur Liste

    def clear_pointers(self):
        # Entfernt alle Zeiger aus der Liste
        self.pointers = []

    def clear_sinus_curves(self):
        # Entfernt alle Sinuskurven aus der Liste
        self.sinus_curves = []

    def update_animation(self):
        # Aktualisiert die Animationen (wird vom Timer aufgerufen)
        self.time += 0.02  # Zeit um 20 ms erhöhen

        # Aktualisiere die Werte aller Sinuskurven
        for curve in self.sinus_curves:
            # Berechne den neuen Wert basierend auf der Frequenz, Amplitude und Phase
            new_value = curve['amplitude'] * np.sin(
                2 * np.pi * curve['frequency'] * self.time + np.radians(curve['phase'])
            )
            curve['values'].pop(0)  # Entferne den ältesten Wert
            curve['values'].append(new_value)  # Füge den neuen Wert hinzu

        self.update()  # Löse ein Repaint-Ereignis aus

    def paintEvent(self, event):
        # Zeichnet die Animationen
        painter = QPainter(self)
        self.draw_sinus_curves(painter)  # Zeichne die Sinuskurven
        self.draw_pointers(painter)     # Zeichne die Zeiger

    def draw_sinus_curves(self, painter):
        # Zeichnet alle Sinuskurven
        x_scale = self.width() / self.history_length  # Skalierung der X-Achse

        for curve in self.sinus_curves:
            painter.setPen(QPen(curve['color'], 2))  # Setze die Farbe und Breite des Stifts
            for j in range(len(curve['values']) - 1):
                # Berechne die Koordinaten der Punkte
                x1 = curve['x_offset'] + j * x_scale
                y1 = curve['y_offset'] - curve['values'][j]
                x2 = curve['x_offset'] + (j + 1) * x_scale
                y2 = curve['y_offset'] - curve['values'][j + 1]

                # Zeichne eine Linie zwischen den Punkten
                painter.drawLine(QPointF(x1, y1), QPointF(x2, y2))

    def draw_pointers(self, painter):
        # Zeichnet alle Zeiger
        for pointer in self.pointers:
            # Berechne die aktuelle Richtung des Zeigers basierend auf der Zeit, Frequenz und Phase
            angle = 2 * np.pi * pointer['frequency'] * self.time + np.radians(pointer['phase'])
            x = pointer['x_center'] + pointer['amplitude'] * np.cos(angle)  # Endpunkt X-Koordinate
            y = pointer['y_center'] - pointer['amplitude'] * np.sin(angle)  # Endpunkt Y-Koordinate
            end_point = QPointF(x, y)  # Endpunkt des Zeigers

            # Zeichne den Zeiger als Linie vom Zentrum zum Endpunkt
            painter.setPen(QPen(pointer['color'], 4))  # Setze die Farbe und Breite des Stifts
            painter.drawLine(QPointF(pointer['x_center'], pointer['y_center']), end_point)

            # Zeichne die Pfeilspitze
            arrow_size = 10  # Größe der Pfeilspitze
            angle_offset = np.pi / 6  # Winkel der Pfeilspitze relativ zum Zeiger
            x1 = x - arrow_size * np.cos(angle - angle_offset)
            y1 = y + arrow_size * np.sin(angle - angle_offset)
            x2 = x - arrow_size * np.cos(angle + angle_offset)
            y2 = y + arrow_size * np.sin(angle + angle_offset)
            painter.drawLine(end_point, QPointF(x1, y1))  # Linie zur ersten Spitze
            painter.drawLine(end_point, QPointF(x2, y2))  # Linie zur zweiten Spitze


# Hauptfunktion zur Erstellung des Fensters
def window(xpos, ypos, width, height):
    # Erstellt eine QApplication und das Hauptfenster
    app = QApplication(sys.argv)
    win = MyWindow(xpos, ypos, width, height)  # Initialisiert das Fenster
    win.show()  # Zeigt das Fenster an
    sys.exit(app.exec())  # Führt die Anwendung aus und beendet sie bei Schließen


# Startet das Programm mit den angegebenen Fenstermaßen
window(0, 0, 1921, 1080)

