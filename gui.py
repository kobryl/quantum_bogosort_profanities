import os
import sys

from PySide6.QtCore import Slot, Qt
from PySide6.QtWidgets import QApplication, QPushButton, QLabel, QPlainTextEdit, \
    QMainWindow

from constants import *


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.__labelInput = QLabel(self)
        self.__labelOutput = QLabel(self)
        self.__labelFalsePositive = QLabel(self)
        self.__labelFalseNegative = QLabel(self)
        self.__input = QPlainTextEdit(self)
        self.__output = QPlainTextEdit(self)
        self.__falsePositive = QPlainTextEdit(self)
        self.__falseNegative = QPlainTextEdit(self)
        self.setFocusPolicy(Qt.StrongFocus)
        self.setWindowTitle("Filtr wulgaryzmów - Quantum Bogosort")
        self.resize(WINDOW_WIDTH, WINDOW_HEIGHT)
        self.move(100, 100)
        self.setFocus(Qt.FocusReason.ActiveWindowFocusReason)
        self.setup()

    def setup(self):
        @Slot()
        def __addFalsePositive():
            f = open("whitelist_debug.txt", "a")
            f.write(self.__falsePositive.toPlainText() + "\n")
            f.close()

        @Slot()
        def __addFalseNegative():
            f = open("profanity_list_debug.txt", "a")
            f.write(self.__falseNegative.toPlainText() + "\n")
            f.close()

        @Slot()
        def __run():
            pass

        @Slot()
        def __exit():
            QApplication.instance().quit()

        @Slot()
        def __help():
            os.startfile("help.txt")                    # TODO: Change to program window (not notepad.exe)

        @Slot()
        def __about():
            os.startfile("README.md")                   # TODO: Change to program window (not notepad.exe)

        self.__labelInput.setText("Tekst wejściowy:")
        self.__labelInput.move(5, 10)
        self.__labelInput.resize(LABEL_WIDTH, LABEL_HEIGHT)

        self.__input.move(5, 40)
        self.__input.resize(TEXTAREA_WIDTH, TEXTAREA_HEIGHT)
        self.__input.setPlaceholderText("Wpisz tekst do przefiltrowania...")
        self.__input.setFocusPolicy(Qt.FocusPolicy.StrongFocus)
        self.__input.setFocus(Qt.FocusReason.ActiveWindowFocusReason)

        runButton = QPushButton("Uruchom", self)
        runButton.move((WINDOW_WIDTH - 10) / 2 - BUTTON_WIDTH, 200)
        runButton.resize(BUTTON_WIDTH, BUTTON_HEIGHT)
        runButton.clicked.connect(__run)
        runButton.setFocusPolicy(Qt.FocusPolicy.NoFocus)

        self.__labelOutput.setText("Tekst wyjściowy:")
        self.__labelOutput.move(5, 210)
        self.__labelOutput.resize(LABEL_WIDTH, LABEL_HEIGHT)

        self.__output.move(5, 240)
        self.__output.resize(TEXTAREA_WIDTH, TEXTAREA_HEIGHT)
        self.__output.setPlaceholderText("Tekst po przefiltrowaniu...")
        self.__output.setFocusPolicy(Qt.FocusPolicy.NoFocus)
        self.__output.setReadOnly(True)

        self.__labelFalsePositive.setText("Filtr ocenzurował niewulgarne słowo? Dodaj je do wyjątków:")
        self.__labelFalsePositive.move(TEXTAREA_WIDTH + 15, 10)
        self.__labelFalsePositive.resize(LABEL_WIDTH, LABEL_HEIGHT)

        self.__falsePositive.move(TEXTAREA_WIDTH + 15, 40)
        self.__falsePositive.resize(TEXTINPUT_WIDTH, TEXTINPUT_HEIGHT)
        self.__falsePositive.setPlaceholderText("Wpisz nowy wyjątek...")
        self.__input.setFocusPolicy(Qt.FocusPolicy.StrongFocus)
        self.__input.setFocus(Qt.FocusReason.ActiveWindowFocusReason)

        falsePositiveButton = QPushButton("Dodaj do wyjątków", self)
        falsePositiveButton.move(TEXTAREA_WIDTH + 15 + TEXTINPUT_WIDTH - BUTTON_WIDTH, 80)
        falsePositiveButton.resize(BUTTON_WIDTH, BUTTON_HEIGHT)
        falsePositiveButton.clicked.connect(__addFalsePositive)
        falsePositiveButton.setFocusPolicy(Qt.FocusPolicy.NoFocus)

        self.__labelFalseNegative.setText("Filtr nie wykrył wulgaryzmu? Dodaj go do listy:")
        self.__labelFalseNegative.move(TEXTAREA_WIDTH + 15, 120)
        self.__labelFalseNegative.resize(LABEL_WIDTH, LABEL_HEIGHT)

        self.__falseNegative.move(TEXTAREA_WIDTH + 15, 150)
        self.__falseNegative.resize(TEXTINPUT_WIDTH, TEXTINPUT_HEIGHT)
        self.__falseNegative.setPlaceholderText("Wpisz niewykryty wulgaryzm...")
        self.__input.setFocusPolicy(Qt.FocusPolicy.StrongFocus)
        self.__input.setFocus(Qt.FocusReason.ActiveWindowFocusReason)

        falseNegativeButton = QPushButton("Dodaj do wulgaryzmów", self)
        falseNegativeButton.move(TEXTAREA_WIDTH + 15 + TEXTINPUT_WIDTH - BUTTON_WIDTH, 190)
        falseNegativeButton.resize(BUTTON_WIDTH, BUTTON_HEIGHT)
        falseNegativeButton.clicked.connect(__addFalseNegative)
        falseNegativeButton.setFocusPolicy(Qt.FocusPolicy.NoFocus)

        helpButton = QPushButton("Pomoc", self)
        helpButton.move(5, WINDOW_HEIGHT - BUTTON_HEIGHT - 5)
        helpButton.resize(BUTTON_WIDTH, BUTTON_HEIGHT)
        helpButton.clicked.connect(__help)
        helpButton.setFocusPolicy(Qt.FocusPolicy.NoFocus)

        aboutButton = QPushButton("O programie", self)
        aboutButton.move(165, WINDOW_HEIGHT - BUTTON_HEIGHT - 5)
        aboutButton.resize(BUTTON_WIDTH, BUTTON_HEIGHT)
        aboutButton.clicked.connect(__about)
        aboutButton.setFocusPolicy(Qt.FocusPolicy.NoFocus)

        exitButton = QPushButton("Wyjście", self)
        exitButton.move(TEXTAREA_WIDTH + 15 + TEXTINPUT_WIDTH - BUTTON_WIDTH, WINDOW_HEIGHT - BUTTON_HEIGHT - 5)
        exitButton.resize(BUTTON_WIDTH, BUTTON_HEIGHT)
        exitButton.clicked.connect(__exit)
        exitButton.setFocusPolicy(Qt.FocusPolicy.NoFocus)

        self.setFocus(Qt.FocusReason.ActiveWindowFocusReason)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
