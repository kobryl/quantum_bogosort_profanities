import sys
from PySide6.QtCore import QRect, Slot, Qt
from PySide6.QtWidgets import QWidget, QApplication, QToolTip, QPushButton, QLabel, QInputDialog, QPlainTextEdit, \
    QTextEdit, QErrorMessage, QMessageBox, QMainWindow
from PySide6.QtGui import QFont, QMouseEvent, QKeyEvent


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.__labelInput = QLabel(self)
        self.__labelOutput = QLabel(self)
        self.__labelFalsePositive = QLabel(self)
        self.__labelFalseNegative = QLabel(self)
        self.__input = QPlainTextEdit(self)
        self.__output = QPlainTextEdit(self)
        self.setFocusPolicy(Qt.StrongFocus)
        self.setWindowTitle("Filtr wulgaryzmów - Quantum Bogosort")
        self.resize(900, 600)
        self.move(100, 100)
        self.setFocus(Qt.FocusReason.ActiveWindowFocusReason)
        self.setup()

    def setup(self):
        pass


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
