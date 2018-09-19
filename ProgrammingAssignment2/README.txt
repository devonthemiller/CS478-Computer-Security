This program is a complete implementation of the DES encryption/decryption algorithm using 
cipher block chaining. This implementation is fully compatible with the openSSL implementation 
of DES in CBC mode.

Compile by entering the "files" directory and typing "make" in command line.
Input should follow the form:
	
	./descbc [-e]or[-d] [-K][key] [-IV][iv] [-i][input filename] [-o][output filename]

The input and output are required, aswell as the -e or -d value which determines 
whether to encrypt or decrypt, respectively. The -e or -d flag should be the first parameter 
given, but the other parameters can be in whatever order you want, as long as you use the 
correct flags. The [key] and [iv] values should be strings composed of 16 hexadecimal digits 
(uppercase). If no [key] and [iv] values are entered, then the program will automatically 
generate values for both and give them to the user. The program does not support the ability 
to have only one manually inputted value.

The files directory has a prepared input file, and the validity of the algorithm can be
proven with the following commands:

	./descbc -e input.txt [filename].txt
	
	./descbc -d [given key] [given iv] [filename].txt [otherfilename].txt

The input.txt and [otherfilename].txt files should be identical.
