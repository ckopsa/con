/*************************************************************************
* Program:
*    Lab Webserver, Computer Communication and Networking
*    Brother Jones, CS 460
* Author:
*    Your Name
* Summary:
*    This program ...
************************************************************************** */
/*** NOTE: The header needs to be changed for second submission (take 2) ***/
/*** NOTE: The header needs to be changed for second submission (take 2) ***/

import java.io.*;
import java.net.*;
import java.util.*;

public final class WebServer
{
  public static void main(String argv[]) throws Exception
  {
    // sets the port number to be used (default: 6789; optional: argv[0])
    int port = argv.length > 0 ? Integer.parseInt(argv[0]) : 6789;
    
    // Establish the listen socket.
    ??
    
    // Process HTTP service requests in an infinite loop
    while (true)
    {
      // Listen for a TCP connection request.
      ??
      
      // Construct an object to process the HTTP request message.
      HttpRequest request = new HttpRequest( ?? );
      
      // Create a new thread to process the request.
      Thread thread = new Thread(request);
      
      // Start the thread.
      thread.start();
    }
  }
}

final class HttpRequest implements Runnable
{
  final static String CRLF = "\r\n";
  Socket socket;
  
  // Constructor
  public HttpRequest(Socket socket) throws Exception
  {
    this.socket = socket;
  }
  
  // Implement the run() method of the Runnable interface.
  public void run()
  {
    try
    {
      processRequest();
    }
    catch (Exception e)
    {
      System.out.println(e);
    }
  }
  
  private void processRequest() throws Exception
  {
    // Get a reference to the socket's input and output streams.
    InputStream is = ??;
    DataOutputStream os = ??;
    
    // Set up input stream filters.
    ??
    BufferedReader br = ??;
    
    // Get the request line of the HTTP request message.
    String requestLine = ??;
    
    // Display the request line.
    System.out.println();
    System.out.println(requestLine);
    
    // Get and display the header lines.
    String headerLine = null;
    while ((headerLine = br.readLine()).length() != 0)
    {
      System.out.println(headerLine);
    }
    
    // Close streams and socket.
    os.close();
    br.close();
    socket.close();
  }
}
