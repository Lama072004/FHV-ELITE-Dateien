import random, sys

from PyQt5 import QtCore as qtc
from PyQt5 import QtWidgets as qtw

from pie.pieDataManagement import PieBox


class ValueView( qtw.QLabel):

    """Widget, das **mit einer Box verbunden** ist.

    Warning:
        Dieses Widget ist direkt nicht verwendbar, sondern muss abgeleitet werden.

    See also:
        `ActValueView` und `NomValueView`.
    """

    def __init__( self, box):
        super().__init__( str( box.value()))
        self.__box = box
        return

    def box( self) -> PieBox:
        """**Liefert die darunterliegende Box.**
        """
        return self.__box


class ActValueView( ValueView):

    """Widget für die **Anzeige von Istwerten**.

    Das Widget, das mit einer Box verbunden ist, holt sich dazu in einem [Timer](https://doc.qt.io/qt-5/qtimer.html)
    -Callback laufend den `pyqt.pandora.Box.value` und aktualisiert sich mit diesem Wert.
    """

    def __init__( self, box: PieBox):
        super().__init__( box)

        self.__is_box_dirty = True
        box.reg_callback_on_value_changed( self._on_box_was_modified_)

        self.__timer = qtc.QTimer()
        self.__timer.setInterval( 100)
                                        # Intervalle werden hier in ms angegeben
        self.__timer.timeout.connect( self._on_timer_)
        self.__timer.start()
        return

    def _on_box_was_modified_( self, *args):
        """Callback der Box, der ausgeführt wird, wenn sich der Value der Box ändert.

        In dieser Methode setzen wir ``self.__is_box_dirty`` auf ``True``, was wir in `_on_timer_`
        ausnützen, um die Anzeige nur dann zu aktualisieren, wenn sich der Wert der Box wirklich
        geändert hat.
        """
        self.__is_box_dirty = True
        return

    def _on_timer_( self, *args):
        """Timer-Slot, in dem wir die Anzeige aktualisieren.

        **Variante 1**

        Diese Variante aktualisiert die Anzeige auf jeden Fall, ganz gleich ob sich der Wert der
        Box verändert hat oder nicht.

        ```python
        def _on_timer_( self, *args):
            self.setText( str( self.box().value()))
            return
        ```

        **Variante 2**

        Diese Variante aktualisiert die Anzeige nur, wenn sich der Wert der Box geändert hat.
        ```python
        def _on_timer_( self, *args):
            if self.__is_box_dirty:
                self.setText( str( self.box().value()))
                self.__is_box_dirty = False

            return
        ```
        Damit das funktioniert, müssen wir uns bei der Box registrieren (s. Ctor):

        ```python
        self.__is_box_dirty = True
        box.regS_on_value_was_modified( self._on_box_was_modified_)
        ```

        Und `_on_box_was_modified_` ist so definiert:

        ```python
        def _on_box_was_modified_( self, *args):
            self.__is_box_dirty = True
            return
        ```
        """
        if self.__is_box_dirty:
            self.setText( str( self.box().value()))
            self.__is_box_dirty = False

        return


class NomValueView( ValueView):

    """Widget für die **Anzeige von Sollwerten** (in Arbeit).
    """

    pass


class EliteGUI( qtw.QMainWindow):

    """Main-Window der App.

    Example:
        So starten Sie das Programm:
        ```python
        if __name__ == "__main__":

            app = qtw.QApplication( sys.argv)
            win = EliteGUI()
            win.show()
            sys.exit( app.exec())
        ```
    """

    def __init__( self):
        super().__init__()

        self.__actvalueboxes = {}
        self.__timer_for_changing_actvalues = qtc.QTimer()
        self.__timer_for_changing_actvalues.setInterval( 100)
                                        # Intervall muss in ms angegeben werden.
        self.__timer_for_changing_actvalues.timeout.connect( self._on_timer_for_changing_actvalues_)
        self.__timer_for_changing_actvalues.start()

        ### Titel und Größe des Windows
        #
        self.setWindowTitle( "KOMMUNIKATIONSSYSTEME")
        self.setMinimumSize( qtc.QSize( 800, 600))

        ### Die Tabs
        #
        mlout = qtw.QHBoxLayout()
        tabs = self._add_tabs_( mlout)
        self._add_tab_ACTVALUES_( tabs)
        self._add_tab_NOMVALUES_( tabs)
        self._add_tab_SETUP_( tabs)
        self._add_tab_LOG_( tabs)

        ### Layout
        #
        mainW = qtw.QWidget()
        mainW.setLayout( mlout)
        self.setCentralWidget( mainW)
        return

    def _add_tab_ACTVALUES_( self, tabs):
        """Tab mit den Istwerten.

        See also:
            `ActValue`.
        """
        ### Das  Tab
        #
        tab = qtw.QWidget()
        tabs.addTab( tab, "Istwerte")

        ### Der Layouter
        #
        grid = qtw.QGridLayout()
        tab.setLayout( grid)

        ### Die Daten
        #
        for i in range( 10):
            box = PieBox( value=0.0, label="Istwert %d" % i)
            self.__actvalueboxes[ "istwert %d" % i] = box

            lbl = qtw.QLabel( box.label())
            lbl.resize( lbl.sizeHint())
            grid.addWidget( lbl, i, 0, qtc.Qt.AlignmentFlag.AlignRight)

            grid.addWidget( ActValueView( box), i, 1, qtc.Qt.AlignmentFlag.AlignRight)

        return

    def _add_tab_NOMVALUES_( self, tabs):
        """.
        """
        tab = qtw.QWidget()
        tabs.addTab( tab, "Sollwerte")
        return

    def _add_tab_SETUP_( self, tabs):
        """.
        """
        tab = qtw.QWidget()
        tabs.addTab( tab, "Setup")
        return

    def _add_tab_LOG_( self, tabs):
        """.
        """
        tab = qtw.QWidget()
        tabs.addTab( tab, "Log")
        return

    def _add_tabs_( self, mlout):
        """.
        """
        tabs = qtw.QTabWidget()
        tabs.setTabPosition( qtw.QTabWidget.East)
        tabs.setMovable( True)
        mlout.addWidget( tabs, stretch=1)
        return tabs

    def _on_timer_for_changing_actvalues_( self, *args):
        """Timer-Slot, in dem wir die `pandora.Box`-Instanzen verändern, damit sich im GUI etwas tut.
        """
        for px in self.__actvalueboxes.values():
            px.value( random.uniform( -100, 100))

        return


if __name__ == "__main__":
    app = qtw.QApplication( sys.argv)
    win = EliteGUI()
    win.show()
    sys.exit( app.exec())