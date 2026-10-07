import time  # Import the time module
from PyQt5 import QtCore as qtc
from PyQt5 import QtWidgets as qtw

class MainWindow(qtw.QWidget):
    def __init__(self):
        super().__init__()
        
        self.windowtitle_org = "PyQt-GUI"  # Original window title
        
        self._build_gui_()  # Build the GUI components
        self.show()  # Show the main window
        return

    def _build_gui_(self):
        # Create and position a checkbox
        cb = qtw.QCheckBox("Show Window Title", self)
        cb.move(20, 20)
        cb.toggle()  # Default state is checked
        cb.stateChanged.connect(self._change_title_)  # Connect the signal to the slot
        
        # Set window geometry and title
        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle(self.windowtitle_org)
        return

    def _change_title_(self, state):
        if state == qtc.Qt.CheckState.Checked:
            # Update the window title with current time
            self.setWindowTitle(self.windowtitle_org + " - " + str(time.time()) + " seconds since 1970-01-01")
        else:
            # Clear the window title
            self.setWindowTitle("")
        return

# Boilerplate to run the application
if __name__ == "__main__":
    app = qtw.QApplication([])
    window = MainWindow()
    app.exec()
