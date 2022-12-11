import locale
import os
import sys
from datetime import datetime

from PySide6.QtCore import Slot, Qt
from PySide6.QtGui import QCloseEvent
from PySide6.QtWidgets import QApplication, QPushButton, QLabel, QPlainTextEdit, \
    QMainWindow, QMessageBox, QWidget
from unidecode import unidecode
from collections.abc import Callable

from constants import *



class MainWindow(QMainWindow):
    """
        This class is the main window of the application.
    """
    def __init__(self):
        """
        Constructor of the class. It initializes the main window and all of its labels and text inputs.
        It also calls the setup function which sets up the layout of the window.
        """
        super().__init__()
        self.__aboutWindow = None
        self.__helpWindow = None
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

    def setup(self) -> None:
        """
        This function sets up the layout of the main window. It creates all the widgets and adds them to the main window.
        :return: None
        """
        def __buttonFactory(text: str, x: int, y: int, function: Callable[[], None]) -> QPushButton:
            """
            Creates a button with the given text, position and function.
            :param text: Text on the button.
            :param x: The x coordinate of the button in the window.
            :param y: The y coordinate of the button in the window.
            :param function: Slot function to be called when the button is clicked.
            :return: QPushButton object.
            """
            btn = QPushButton(text, self)
            btn.move(x, y)
            btn.resize(BUTTON_WIDTH, BUTTON_HEIGHT)
            btn.clicked.connect(function)
            btn.setFocusPolicy(Qt.FocusPolicy.NoFocus)
            return btn

        def __labelFactory(text: str, x: int, y: int) -> QLabel:
            """
            Creates a label with the given text and position.
            :param text: Label text.
            :param x: The x coordinate of the label in the window.
            :param y: The y coordinate of the label in the window.
            :return: QLabel object.
            """
            label = QLabel(text, self)
            label.move(x, y)
            label.resize(LABEL_WIDTH, LABEL_HEIGHT)
            return label

        def __textAreaFactory(textArea: QPlainTextEdit, x: int, y: int, placeholder: str, width: int, height: int) -> None:
            """
            Creates a text area with the given text, position, placeholder and size.
            :param textArea: QPlainTextEdit object that was initialized in constructor.
            :param x: The x coordinate of the text area in the window.
            :param y: The y coordinate of the text area in the window.
            :param placeholder: Placeholder text.
            :param width: Width of the text area.
            :param height: Height of the text area.
            :return: None
            """
            textArea.move(x, y)
            textArea.setPlaceholderText(placeholder)
            textArea.resize(width, height)
            textArea.setFocusPolicy(Qt.FocusPolicy.StrongFocus)
            textArea.setFocus(Qt.FocusReason.ActiveWindowFocusReason)

        def __normalizeText(text: str) -> str:
            """
            Normalizes the given text by removing all the special characters and converting it to lowercase.
            :param text: Text to be normalized.
            :return: Normalized text.
            """
            return unidecode(text).lower().replace(" ", "")

        @Slot()
        def __addFalsePositive() -> None:
            """
            Adds the text from the false positive text area to the whitelist.
            :return: None
            """
            try:
                text = __normalizeText(self.__falsePositive.toPlainText())
                if DEBUG:
                    print(text)
                with open(PATH_TO_WHITELIST, "a", encoding="UTF-8") as f:
                    f.write(text + " 0 0\n")
                QMessageBox.information(self, "Sukces", "Dodano do białej listy")
                self.__falsePositive.setPlainText("")
            except Exception as e:
                if DEBUG:
                    print(e)
                QMessageBox.critical(self, "Błąd", "Nie udało się zapisać.")

        @Slot()
        def __addFalseNegative() -> None:
            """
            Adds the text from the false negative text area to the list of profanities.
            :return: None
            """
            try:
                text = __normalizeText(self.__falseNegative.toPlainText())
                if DEBUG:
                    print(text)
                with open(PATH_TO_PROFANITIES, "a", encoding="UTF-8") as f:
                    f.write(text + " 0 0 0\n")
                QMessageBox.information(self, "Sukces", "Dodano do czarnej listy")
                self.__falseNegative.setPlainText("")
            except Exception as e:
                if DEBUG:
                    print(e)
                QMessageBox.critical(self, "Błąd", "Nie udało się zapisać.")

        @Slot()
        def __run() -> None:
            """
            Runs the main algorithm. It checks if the input text contains any profanities and if so, it replaces them
            with asterisks. For performance reasons, the algorithm was implemented in C++. The input text is passed
            to it through standard input using temporary text file. Similarly, the output text is read from standard output.
            The output then is displayed in the read-only output text area.
            :return: None
            """
            timestamp = str(datetime.now().strftime("%Y%m%d%H%M%S"))
            with open("input" + timestamp + ".txt", "w", encoding="UTF-8") as fileInput:
                fileInput.write(self.__input.toPlainText())
            try:
                os.system(PATH_TO_FILTER + " < input" + timestamp + ".txt > output" + timestamp + ".txt")
                QMessageBox.information(self, "Filtr wulgaryzmów", "Filtrowanie zakończone pomyślnie.")
                with open("output" + timestamp + ".txt", "r", encoding="UTF-8") as fileOutput:
                    self.__output.setPlainText(fileOutput.read())
            except Exception as e:
                if DEBUG:
                    print(e)
                QMessageBox.critical(self, "Filtr wulgaryzmów", "Nie znaleziono pliku wykonywalnego. "
                                                                "Ponowne pobranie aplikacji może rozwiązać ten problem")
            finally:
                try:
                    os.remove("input" + timestamp + ".txt")
                    os.remove("output" + timestamp + ".txt")
                except FileNotFoundError:
                    pass

        @Slot()
        def __exit() -> None:
            """
            Exits the application.
            :return: None
            """
            QApplication.instance().quit()

        @Slot()
        def __help() -> None:
            """
            Displays the "Help" widget window.
            :return: None
            """
            self.__helpWindow = HelpWindow()
            self.__helpWindow.show()

        @Slot()
        def __about() -> None:
            """
            Displays the "About" widget window.
            :return: None
            """
            self.__aboutWindow = AboutWindow()
            self.__aboutWindow.show()

        # Labels
        self.__labelInput = __labelFactory("Tekst wejściowy:", 5, 10)
        self.__labelOutput = __labelFactory("Tekst wyjściowy:", 5, 210)
        self.__labelFalseNegative = __labelFactory("Filtr nie wykrył wulgaryzmu? Dodaj go do listy:",
                                                   TEXTAREA_WIDTH + 15, 120)
        self.__labelFalsePositive = __labelFactory("Filtr ocenzurował niewulgarne słowo? Dodaj je do wyjątków:",
                                                   TEXTAREA_WIDTH + 15, 10)

        # Text Areas
        __textAreaFactory(self.__input, 5, 40, "Wpisz tekst do przefiltrowania...", TEXTAREA_WIDTH, TEXTAREA_HEIGHT)
        __textAreaFactory(self.__output, 5, 240, "Tekst po przefiltrowaniu...", TEXTAREA_WIDTH, TEXTAREA_HEIGHT)
        __textAreaFactory(self.__falsePositive, TEXTAREA_WIDTH + 15, 40, "Wpisz nowy wyjątek...",
                          TEXTINPUT_WIDTH, TEXTINPUT_HEIGHT)
        __textAreaFactory(self.__falseNegative, TEXTAREA_WIDTH + 15, 150, "Wpisz niewykryty wulgaryzm...",
                          TEXTINPUT_WIDTH, TEXTINPUT_HEIGHT)

        self.__output.setReadOnly(True)

        # Buttons
        runButton = __buttonFactory("Uruchom", (WINDOW_WIDTH - 10) / 2 - BUTTON_WIDTH, 200, __run)
        falsePositiveButton = __buttonFactory("Dodaj do wyjątków", TEXTAREA_WIDTH + 15 + TEXTINPUT_WIDTH - BUTTON_WIDTH,
                                              80, __addFalsePositive)
        falseNegativeButton = __buttonFactory("Dodaj do wulgaryzmów",
                                              TEXTAREA_WIDTH + 15 + TEXTINPUT_WIDTH - BUTTON_WIDTH,
                                              190, __addFalseNegative)
        helpButton = __buttonFactory("Pomoc", 5, WINDOW_HEIGHT - BUTTON_HEIGHT - 5, __help)
        aboutButton = __buttonFactory("O programie", 165, WINDOW_HEIGHT - BUTTON_HEIGHT - 5, __about)
        exitButton = __buttonFactory("Wyjście", TEXTAREA_WIDTH + 15 + TEXTINPUT_WIDTH - BUTTON_WIDTH,
                                     WINDOW_HEIGHT - BUTTON_HEIGHT - 5, __exit)

        self.setFocus(Qt.FocusReason.ActiveWindowFocusReason)

    def closeEvent(self, event: QCloseEvent) -> None:
        """
        Overridden method from QWidget class. It is called when the main window is closed. It closes all the other
        windows and exits the application.
        :param event:
        :return: None
        """
        for w in [self.__helpWindow, self.__aboutWindow]:
            if w:
                w.close()
        self.close()


class AboutWindow(QWidget):
    """
        "About" window class
    """
    def __init__(self):
        """
        Constructor of the class. It creates the "About" window and sets its properties.
        """
        super().__init__()
        self.setWindowTitle("O programie")
        self.setFixedSize(WIDGET_WIDTH, ABOUT_WIDGET_HEIGHT)
        self.__aboutText = QLabel(self)
        try:
            with open(PATH_TO_ABOUT, "r") as f:
                self.__aboutText.setText(f.read())
        except FileNotFoundError:
            self.__aboutText.setText("Nie znaleziono pliku zawierającego informacje o programie. "
                                     "Ponowne pobranie programu powinno rozwiązać problem.")
        self.__aboutText.move(WIDGET_MARGIN, WIDGET_MARGIN)
        self.__aboutText.resize(WIDGET_TEXT_WIDTH, ABOUT_TEXT_HEIGHT)
        self.__aboutText.setWordWrap(True)
        self.__aboutText.setAlignment(Qt.AlignmentFlag.AlignTop | Qt.AlignmentFlag.AlignJustify)


class HelpWindow(QWidget):
    """
        "Help" window class
    """
    def __init__(self):
        """
        Constructor of the class. It creates the "Help" window and sets its properties.
        """
        super().__init__()
        self.setWindowTitle("Pomoc")
        self.setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT)
        self.__helpText = QLabel(self)
        try:
            with open(PATH_TO_HELP, "r") as file:
                self.__helpText.setText(file.read())
        except FileNotFoundError:
            self.__helpText.setText("Nie znaleziono pliku zawierającego pomoc. "
                                    "Ponowne pobranie programu powinno rozwiązać problem.")
        self.__helpText.move(WIDGET_MARGIN, WIDGET_MARGIN)
        self.__helpText.resize(WIDGET_TEXT_WIDTH, WIDGET_TEXT_HEIGHT)
        self.__helpText.setWordWrap(True)
        self.__helpText.setAlignment(Qt.AlignmentFlag.AlignTop | Qt.AlignmentFlag.AlignJustify)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
