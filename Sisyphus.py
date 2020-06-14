import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

class AppHandler(QWidget):
    def __init__(self):
        super().__init__()
        self.title = "Sisyphus RAT v1.0"
        self.width = 1500
        self.height = 800
        self.init_ui()

    def init_ui(self):
        self.setWindowTitle(self.title)
        self.setGeometry(10, 10, self.width, self.height)

        ## Buttons
        compile_button = QPushButton("Compile", self)
        compile_button.setToolTip("Compile binary for selected platform")
        compile_button.move(100, 70)
        compile_button.clicked.connect(self.compile)

        execute_button = QPushButton("Execute", self)
        execute_button.setToolTip("Execute the entered command")
        execute_button.move(300, 100)
        execute_button.clicked.connect(self.execute_command)

        ## Show interface
        self.show()

    def compile(self):
        print("Compiling binary now")

    def execute_command(self):
        print("Running a command now")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = AppHandler()
    sys.exit(app.exec_())
