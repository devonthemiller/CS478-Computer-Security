This program is designed to allow a user to perform basic cryptanalysis on text that is encrypted using a simple substition cipher.

Compile the program by typing 'make' into the command line. Then run the program by typing './substitutioncracker'.

1.) For displaying the frequency analysis, the program should automatically output all the characters in the ciphertext in order of decreasing frequency alongside the number of times each each of them appears.

2.) The program should prompt you to either run manual or automatic cryptanalysis. Running automatic cryptanalysis will generate a key that is fairly close to the actual key and then begin the manual cryptanalysis process.

3.) Manual cryptanalysis will allow the user to continually substitute different letters in the message for each other. This process can be repeated as much as desired and the decryption key will be updated in real time with each substitution. When the user is satisfied with the resulting plaintext, they can stop manual cryptanalysis and the program will output the final decryption key along with the final message that was deciphered.
