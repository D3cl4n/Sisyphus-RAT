# Sisyphus-RAT
Remote administration tool for offensive security researchers

# What is it?
A remote administration tool allows a user to control and manipulate a computer remotely, despite not having physical access to the machine in question. Sisyphus-RAT is a rudimentary RAT that aims to aid offensive security researchers on penetration testing engagements. 

# What went into making Sisyphus?
Sisyphus includes a Graphical User Interface (GUI) which is written in python. This GUI provides an abstraction layer over a backend that communicated directly to the client code running on the machine that is being remotely managed. The client code is written completely in C, and both windows and linux are supported. The client and the server communicate with end to end encryption, and I have implemented RSA in order to ensure this. The user has the options to read files, delete files, retrive system info, retrive networking info, execute a command, and spawn a shell on the computer being remotely managed. 
