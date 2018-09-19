This assignment represents a simple construct for a public-key cryptographic system that employs the use of digital signatures. The encryption and decryption of the plaintext and ciphertext is done using the DES algorithm from Programming Assignment 2.

Both programs can be compiled by going into the 'Part1' or
'Part2' directories and typing 'make'. 

To run Part1 (encryption), type './encrypt' and then use the following flags:

	-in	The next argument entered is interpreted as
		the filename for the plaintext.

	-sess	The next argument entered is interpreted as
		the filename for the encrypted session key.

	-pub	The next argument entered is interpreted as
		the filename for the third party public key.

	-priv	The next argument entered is interpreted as
		the filename for the user's private key.

Part1 produces a session key (decrypted) file, an IV file,
a signature file, and an encrypted plaintext file (which is
always of the form "enc_" and then the name of the original
plaintext file).

To run Part2 (decryption), type './decrypt' and then use the following flags:

	-in	The next argument entered is interpreted as
		the filename for the encrypted plaintext.

	-sess	The next argument entered is interpreted as
		the filename for the decrypted session key.

	-pub	The next argument entered is interpreted as
		the filename for the user's public key.

	-s	The next argument entered is interpreted as
		the filename for the signature.

	-iv	The next argument entered is interpreted as
		the filename for the IV.

Part2 will tell you if the signature is verified or not. It will also produce
the decrypted ciphertext.

