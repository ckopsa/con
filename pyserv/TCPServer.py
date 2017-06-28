#############################################################################
# Program:
#    Lab PythonWebServer, Computer Communication and Networking
#    Brother Jones, CS 460
# Author:
#    Colton Kopsa
# Summary:
#    This program is a web server handling basic text and image files.
#
# Note: If you put #!/usr/bin/python as the first line of this file and
#       make the program executable, the submit command will not be happy.
#       Run your Python program using:  python yourCode.py
#
#############################################################################
#
# Changes made to my code for the re-submission:
#   - Fixed a print statement
#
#
##############################################################################
from socket import *
import sys
import os

# Return proper content type according to file extension
def contentType(filepath):
   fileExtension = os.path.splitext(filepath)[1]
   contentTypes = {
      ".htm" : "text/html",
      ".html" : "text/html",
      ".txt" : "text/plain",
      ".gif" : "image/gif",
      ".jpeg" : "image/jpeg",
      ".jpg" : "image/jpeg"
   }
   return contentTypes.get(fileExtension, "text/plain")

# Server Connection Setup
serverPort = int(sys.argv[1]) if len(sys.argv) == 2 else 6789
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print "Server is running on port " + str(serverPort)

CRLF = "\r\n"
try:
   # Main Server Loop
   while 1:
      connectionSocket, addr = serverSocket.accept()
      request = connectionSocket.recv(1024)
      print request
      request = request.split()
      # Parse tokens from request string
      filepath = "." + request[1]
      httptype = request[2]
      # Ensure file exists
      fileExists = os.path.isfile(filepath)
      # Handle status/content types
      if fileExists:
         statusLine = "HTTP/1.0 200 OK" + CRLF
         contentTypeLine = "Content-type: " + contentType( filepath ) + CRLF
      else:
         statusLine = "HTTP/1.0 404 Not Found" + CRLF
         contentTypeLine = "Content-type: text/html" + CRLF
         entityBody = "<HTML>" + "<HEAD><TITLE>Not Found</TITLE></HEAD>" + "<BODY>Not Found</BODY></HTML>"
      # Send response back
      connectionSocket.send(statusLine)
      connectionSocket.send(contentTypeLine)
      connectionSocket.send(CRLF)
      if fileExists:
         requestedFile = open(filepath, "r")
         connectionSocket.send(requestedFile.read())
         requestedFile.close()
      else:
         connectionSocket.send(entityBody)
      connectionSocket.close()

except KeyboardInterrupt:
   print "\nClosing Server"
   serverSocket.close()
