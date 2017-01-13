/*
James Vaughan
Professor Shengli Yuan
CS 3326 Network Security
10 November 2014

JRV_RC4.cpp

Purposes:
Implement both encryption and decryption in RC4.

Encryption Input: 8 byte hex as the plaintext, one 7-byte hex as the key
Encryption Output: 8 bytes hex
Decryption Input: 8 bytes hex as the cipher text, one 7-byte hex as the key
Decryption Output: 8 byte hex characters
You must submit your source code, screen shot with "AABBCCDD11223344" as the plain text and "71322185720000" as the key. You will also need to do a demo.
*/

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>

#include<cstdio>
#include<cstdlib>

using std::stringstream;

using namespace std;
using std::string;

typedef unsigned char uns8;
typedef unsigned short uns16;


string RC4_XOR(string, string);
string Uns8ToBinary(uns8);
std::string RC4_Decrypt(string, string);
std::string RC4_Encrypt(string, string);
std::string HexToBinary(string);
std::string BinaryToHex(string);
void EndOfProgram();
void InvalidResponseGiven();
bool IsValidText(string);
bool IsValidKey(string);


static uns8 state[256], x, y; /* 258 octets of state information */
static int initMutex = 0;


string RC4_XOR(string text1, string text2)
{

	for (int bitIndex = 0; bitIndex < 8; bitIndex++) // bits 0-7
	{
		if (text1[bitIndex] == text2[bitIndex])
		{
			text1[bitIndex] = '0';
		}
		else
		{
			text1[bitIndex] = '1';
		}
	}

	return text1;
}


string Uns8ToBinary(uns8 Uns8ToConvert)
{
	int remainder = 0;
	int dividend = Uns8ToConvert;
	int array[8];
	string Output = "";


	for (int i = 7; i >= 0; i--)
	{
		remainder = dividend % 2;
		array[i] = remainder;
		dividend = dividend / 2;
	}


	for (int j = 0; j < 8; j++)
	{
		Output += (array[j] + 48);
	}


	return Output;
}


void rc4init(uns8 key[], uns16 length) /* initialize for encryption / decryption */
{
	int i;
	uns8 t;
	uns8 j;
	uns8 k = 0;

	//for (i = 256; i--;)
	//	state[i] = i;

	for (i = 0; i < 256; i++)
	{
		state[i] = i;
	}

	/*
	for (i = 0, j = 0; i < 256; i++, j = (j + 1) % length)
		t = state[i], state[i] = state[k += key[j] + t], state[k] = t;
		*/
	for (i = 0, j = 0; i < 256; i++)
	{
		j = ((j + state[i] + key[i % length]) % 256);
		t = state[i];
		state[i] = state[j];
		state[j] = t;
	}

	while (initMutex < 100)
	{
		initMutex++; // bring the mutex from 0 to 100
	}
	x = 0;
	y = 0;
}


uns8 rc4step() /* return next pseudo-random octet */
{
	uns8 t;

	// ERROR PRODUCING CODE
	/*t = state[y += state[++x]], state[y] = state[x], state[x] = t;
	return (state[(state[x] + state[y] )]);
	cout << "a";*/

	t = state[(y += state[((++x) % 256)]) % 256], state[y] = state[x], state[x] = t;
	return (state[(state[x] + state[y]) % 256]);

	// CODE FROM WIKIPEDIA BELOW, PRODUCES SAME OUTPUT
	//x = (x + 1) % 256;
	//y = (y + state[x]) % 256;
	//t = state[x];
	//state[x] = state[y];
	//state[y] = t;
	//return state[(state[x] + state[y]) % 256];
}


std::string RC4_Decrypt(string CipherText, uns8 uns8_Key[7])
{
	string CipherText_String;
	string RC4_String;
	string XOR_Output;
	string Output;

	rc4init(uns8_Key, 7); // Initializing the state array

	// 0-1, 2-3, 4-5, 6-7, 8-9, 10-11, 12-13, 14-15

	for (int CipherTextIndex = 0; CipherTextIndex < 8; CipherTextIndex++) // from byte 0-7 of the plaintext
	{
		CipherText_String = ""; // clear CipherText_String
		CipherText_String.append(CipherText, CipherTextIndex * 2, 2); // obtain 2 hex characters from beginning of CipherText
		CipherText_String = HexToBinary(CipherText_String); // convert those 2 hex chars into binary
		RC4_String = Uns8ToBinary(rc4step()); // convert the RC4 step into binary
		XOR_Output = RC4_XOR(CipherText_String, RC4_String); // XOR the ciphertext with the RC4.
		Output.append(XOR_Output);
	}

	return BinaryToHex(Output);
}


std::string RC4_Encrypt(string PlainText, uns8 uns8_Key[7])
{
	string PlainText_String = "";
	string RC4_String = "";
	string XOR_Output = "";
	string Output = "";

	rc4init(uns8_Key, 7); // Initializing the state array

	while (initMutex < 100)
	{
	}

	// 0-1, 2-3, 4-5, 6-7, 8-9, 10-11, 12-13, 14-15

	uns8 RC4_Input;

	for (int PlainTextIndex = 0; PlainTextIndex < 8; PlainTextIndex++) // from byte 0-7 of the plaintext
	{
		PlainText_String = ""; // clear PlainText_String
		PlainText_String.append(PlainText, PlainTextIndex*2, 2); // obtain 2 hex characters from beginning of PlainText
		PlainText_String = HexToBinary(PlainText_String); // convert those 2 hex chars into binary
		RC4_Input = rc4step();
		//cout << endl << (int)RC4_Input << " " << (int)x << " " << (int)y << " " << (int)state[x] << " " << (int)state[y]  << " " << (int)(state [(state[x] + state[y])]) << endl;
		RC4_String = Uns8ToBinary(RC4_Input); // convert the RC4 step into binary
		XOR_Output = RC4_XOR(PlainText_String, RC4_String); // XOR the plaintext with the RC4.
		Output.append(XOR_Output);
	}
	//cout << endl << Output << endl;

	return BinaryToHex(Output);
}


std::string HexToBinary(string HexToConvert)
{
	string OutputBinary = "";

	for (int CharIndex = 0; CharIndex < HexToConvert.length(); CharIndex++)
	{
		switch (HexToConvert[CharIndex])
		{
		case '0': OutputBinary.append("0000"); break;
		case '1': OutputBinary.append("0001"); break;
		case '2': OutputBinary.append("0010"); break;
		case '3': OutputBinary.append("0011"); break;
		case '4': OutputBinary.append("0100"); break;
		case '5': OutputBinary.append("0101"); break;
		case '6': OutputBinary.append("0110"); break;
		case '7': OutputBinary.append("0111"); break;
		case '8': OutputBinary.append("1000"); break;
		case '9': OutputBinary.append("1001"); break;
		case 'A': OutputBinary.append("1010"); break;
		case 'B': OutputBinary.append("1011"); break;
		case 'C': OutputBinary.append("1100"); break;
		case 'D': OutputBinary.append("1101"); break;
		case 'E': OutputBinary.append("1110"); break;
		case 'F': OutputBinary.append("1111"); break;
		}


	}


	return OutputBinary;
}


std::string BinaryToHex(string BinaryToConvert)
{
	// Input is 64 bits long
	// 16 chunks of 4 bits each = 16 hex characters

	string OutputHex = "";
	string FourBits = "";
	int FirstBit;

	for (int Round = 0; Round < 16; Round++) // Round 0-15
	{
		// 0-3, 4-7, 8-11, 12-15, ...
		FirstBit = (0 + Round * 4);
		FourBits.assign(BinaryToConvert, FirstBit, 4);
		if (FourBits == "0000") { OutputHex.append("0"); }
		if (FourBits == "0001") { OutputHex.append("1"); }
		if (FourBits == "0010") { OutputHex.append("2"); }
		if (FourBits == "0011") { OutputHex.append("3"); }
		if (FourBits == "0100") { OutputHex.append("4"); }
		if (FourBits == "0101") { OutputHex.append("5"); }
		if (FourBits == "0110") { OutputHex.append("6"); }
		if (FourBits == "0111") { OutputHex.append("7"); }
		if (FourBits == "1000") { OutputHex.append("8"); }
		if (FourBits == "1001") { OutputHex.append("9"); }
		if (FourBits == "1010") { OutputHex.append("A"); }
		if (FourBits == "1011") { OutputHex.append("B"); }
		if (FourBits == "1100") { OutputHex.append("C"); }
		if (FourBits == "1101") { OutputHex.append("D"); }
		if (FourBits == "1110") { OutputHex.append("E"); }
		if (FourBits == "1111") { OutputHex.append("F"); }
	}


	return OutputHex;
}


void EndOfProgram()
{
	int EndOfProgramVariable;
	cout << "To exit the program, press any key followed by the ENTER key: ";
	cin >> EndOfProgramVariable;
	cout << endl;
}


void InvalidResponseGiven()
{
	cout << "Invalid response given. Program will now terminate." << endl;
	cout << endl;
	EndOfProgram();
}


bool IsValidText(string TextToValidate)
{
	/* All texts must be:
	- 16 hex characters long (8 hex bytes)
	- In hexadecimal
	*/


	if (TextToValidate.length() != 16)
	{
		return false;
	}

	for (int charIndex = 0; charIndex < 16; charIndex++)
	{
		if (TextToValidate[charIndex] < 48) // less than 0
		{
			return false;
		}


		if ((TextToValidate[charIndex] > 57) && (TextToValidate[charIndex] < 65)) // more than 9 and less than A
		{
			return false;
		}


		if (TextToValidate[charIndex] > 70) // more than F
		{
			return false;
		}
	}


	return true;
}


bool IsValidKey(string KeyToValidate)
{
	/* All texts must be:
	- 14 hex characters long (7 hex bytes)
	- In hexadecimal
	*/


	if (KeyToValidate.length() != 14)
	{
		return false;
	}


	for (int charIndex = 0; charIndex < 14; charIndex++)
	{
		if (KeyToValidate[charIndex] < 48) // less than 0
		{
			return false;
		}


		if ((KeyToValidate[charIndex] > 57) && (KeyToValidate[charIndex] < 65)) // more than 9 and less than A
		{
			return false;
		}


		if (KeyToValidate[charIndex] > 70) // more than F
		{
			return false;
		}
	}


	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	string CipherText = "";
	string PlainText = "";
	string RC4_Key = "";
	uns8 uns8_Key[7] = { 0, 0, 0, 0, 0, 0, 0 };
	string KeyboardInput = "";
	bool isEncrypt; // true means encrypt, false means decrypt


	cout << "JRV_RC4 by James Vaughan" << endl;
	cout << endl;
	cout << "Encrypt or decrypt?" << endl;
	cout << "Type \"encrypt\" or \"decrypt\" below: " << endl;
	cout << endl;
	cout << ">> ";
	cin >> KeyboardInput;
	cout << endl;


	if ((KeyboardInput != "encrypt") && (KeyboardInput != "decrypt"))
	{
		InvalidResponseGiven();
		return 0;
	}


	if (KeyboardInput == "encrypt")
	{
		isEncrypt = true;
	}


	if (KeyboardInput == "decrypt")
	{
		isEncrypt = false;
	}


	if (isEncrypt) // Encryption UI
	{
		cout << "The plaintext must be exactly 8 hex bytes, no spaces, all uppercase. " << endl;
		cout << "Please enter the plaintext to be encrypted below: " << endl;
		cout << endl;
		cout << "   ................" << endl;
		cout << ">> ";
		cin >> PlainText;
		cout << endl;


		if (!IsValidText(PlainText))
		{
			InvalidResponseGiven();
			return 0;
		}


		cout << "The key must be exactly 7 hex bytes, no spaces, all uppercase. " << endl;
		cout << "Please enter the RC4 key below: " << endl;
		cout << endl;
		cout << "   .............." << endl;
		cout << ">> ";
		cin >> RC4_Key;
		cout << endl;


		if (!IsValidKey(RC4_Key))
		{
			InvalidResponseGiven();
			return 0;
		}


		cout << "The generated ciphertext is shown below: " << endl;
		cout << endl;


		// convert RC4_Key into uns8_Key
		// RC4_Key is 14 characters long. Each 2 characters will become 1 uns8 character
		std::string TwoHex = "";
		for (int RC4_Key_Index = 0; RC4_Key_Index < 7; RC4_Key_Index++) // index 0-6
		{
			TwoHex = "";
			TwoHex.append(RC4_Key, (RC4_Key_Index * 2), 2);
			uns8_Key[RC4_Key_Index] = strtoul(TwoHex.substr(0, 2).c_str(), NULL, 16);
		}


		cout << RC4_Encrypt(PlainText, uns8_Key) << endl;
		cout << endl;

	}


	if (!isEncrypt) // Decryption UI
	{
		cout << "The ciphertext must be exactly 8 hex bytes, no spaces, all uppercase. " << endl;
		cout << "Please enter the ciphertext to be decrypted below: " << endl;
		cout << endl;
		cout << "   ................" << endl;
		cout << ">> ";
		cin >> CipherText;
		cout << endl;


		if (!IsValidText(CipherText))
		{
			InvalidResponseGiven();
			return 0;
		}


		cout << "The key must be exactly 7 hex bytes, no spaces, all uppercase. " << endl;
		cout << "Please enter the RC4 key below: " << endl;
		cout << endl;
		cout << "   .............." << endl;
		cout << ">> ";
		cin >> RC4_Key;
		cout << endl;


		if (!IsValidKey(RC4_Key))
		{
			InvalidResponseGiven();
			return 0;
		}


		cout << "The generated plaintext is shown below: " << endl;
		cout << endl;


		// convert RC4_Key into uns8_Key
		// RC4_Key is 14 characters long. Each 2 characters will become 1 uns8 character
		std::string TwoHex = "";
		for (int RC4_Key_Index = 0; RC4_Key_Index < 7; RC4_Key_Index++)
		{
			TwoHex = "";
			TwoHex.append(RC4_Key, (RC4_Key_Index * 2), 2);
			uns8_Key[RC4_Key_Index] = strtoul(TwoHex.substr(0, 2).c_str(), NULL, 16);
		}


		cout << RC4_Decrypt(CipherText, uns8_Key) << endl;
		cout << endl;

	}


	EndOfProgram();
	return 0;
}

