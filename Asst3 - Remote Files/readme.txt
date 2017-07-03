Only the base program was finished and submitted for this project.  

The base program essentially provides and alternative to the everyday open, read, write, and close commands used in C.  netopen, netread, netwrite, and netclose allow the user to communicate and manipulate files on a server across a network.  Our net wrapper files replace open, close, read and write in a user’s program to allow them to do so.  
A client server relationship is created via a socket and port number.  Since our net functions are very similar to their counterparts and take in similar arguments, after finding and replacing the original functions with our cross network functions the given inputs were taken and put into a buffer char array.  This is done for each net function.  The buffer function was passed through the socket and decoded on the other side, the server side.  
The buffer, once received on the server side is broken up and decoded to conduct the proper functions needed.

The overall purpose of the program isn’t the most complicated however implementation was tricky and created many errors.  Constant error checking was done with gdb and manually to make sure buffer statements were sent and decoded properly and also that they were interpreted properly which we found to be true. Our big issues were with sending error reports back to the client.

The overall efficiency of a program like this is not of the highest caliber, but in terms of reliability it does work.