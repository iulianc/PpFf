/*
 * decrypt.cpp
 *
 * Copyright (c) 2014, Danilo Treffiletti <urban82@gmail.com>
 * All rights reserved.
 *
 *     This file is part of Aes256.
 *
 *     Aes256 is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as
 *     published by the Free Software Foundation, either version 2.1
 *     of the License, or (at your option) any later version.
 *
 *     Aes256 is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU Lesser General Public License for more details.
 *
 *     You should have received a copy of the GNU Lesser General Public
 *     License along with Aes256.
 *     If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

#include <Pipe.hpp>
#include "aes256.hpp"


using namespace PpFf;

#define BUFFER_SIZE 1024*1024

#ifdef __APPLE__
#  define fseeko64 fseeko
#endif


ByteArray encrypt(ByteArray content, ByteArray &key)
{
	ByteArray enc;
   Aes256 aes(key);

	const unsigned char* plain = &content[0];
	aes.encrypt(key, plain, content.size(), enc);
	return enc;
}

ByteArray decrypt(ByteArray content, ByteArray &key)
{
	ByteArray dec;
   Aes256 aes(key);

	const unsigned char* encrypted_content = &content[0];
	aes.decrypt(key, encrypted_content, content.size(), dec);
	return dec;
}

void read_content_file(const char *path, ByteArray &content)
{
   FILE * pFile;
  	int c;
  	pFile=fopen (path,"rb");
  	if (pFile==NULL) {fprintf(stderr, "Cannot read file '%s'\n", path);}

	 while ((c = getc(pFile)) != EOF){
		content.push_back(c);
	 }
}



std::string* map_encrypt(std::string* data) {
	std::string *result = new std::string;
	std::string key_str = "64529BD6F6C682AE74B9AAC9CEB86";
	ByteArray key, encrypted_data, input_data;

	for (char c: key_str) {
		key.push_back(c);
	}

	for (char c: *data) {
		input_data.push_back(c);
	}

	encrypted_data = encrypt(input_data, key);

	for (char c: encrypted_data) {
		*result += c;
	}
	

//***String to char array***
//const char *array = tmp.c_str();
//char *array = &tmp[0];


	return result;
}


std::string* map_decrypt(std::string* data) {
	std::string *result = new std::string;
	std::string key_str = "64529BD6F6C682AE74B9AAC9CEB86";
	ByteArray key, decrypted_data, input_data;

	for (char c: key_str) {
		key.push_back(c);
	}

	for (char c: *data) {
		input_data.push_back(c);
	}

	decrypted_data = decrypt(input_data, key);

	for (char c: decrypted_data) {
		*result += c;
	}

	return result;
}



int main(int argc, char **argv)
{
   ByteArray key, enc_dec_data, content;
   FILE *output;
	std::string mode = "encrypt";				//encrypt/decrypt
	std::string pr_type = "cpp-native";		//cpp-native/PpFf

	const char *input_file_path = NULL;
	const char *output_file_path = NULL;
	const char *key_file_path = NULL;
	int no_times, no_enc_dec;

   //srand(time(0));

   if (argc != 8) {
       fprintf(stderr, "Missing arguments\n");
       fprintf(stderr, "Usage: %s <key> <input file> <encrypted or decrypted file> <mode encrypt/decript> <number encrypt/decript> <number times> <program type>\n", argv[0]);
       return 1;
   }


	key_file_path = argv[1];

	input_file_path = argv[2];
	output_file_path = argv[3];
	 
	mode = argv[4];
	no_enc_dec = atoi(argv[5]);
	no_times = atoi(argv[6]);

	if(no_enc_dec < 1) no_enc_dec = 1;
	if(no_times < 1) no_times = 1;

	pr_type = argv[7];


	read_content_file(key_file_path, key);
	read_content_file(input_file_path, content);
	 

	if(pr_type == "cpp-native") {

   	output = fopen(output_file_path, "wb");
   	if (output == 0) {
   		fprintf(stderr, "Cannot write file '%s'\n", output_file_path);
      	return 1;
   	}
	
		if(mode == "encrypt"){
			for(int i = 0; i < no_enc_dec; i++){
				if(i > 0){
					content = enc_dec_data;
				}

				enc_dec_data = encrypt(content, key);
			}
		} else if( mode == "decrypt" ) {
			for(int i = 0; i < no_enc_dec; i++){
				if(i > 0){
					content = enc_dec_data;
				}

				enc_dec_data = decrypt(content, key);
			}
		}

		fwrite(enc_dec_data.data(), enc_dec_data.size(), 1, output);
   	fclose(output);

	} else if(pr_type == "PpFf") {
		typedef std::vector<std::string> Result;
		Result result;

		if( mode == "encrypt" ) {
		 	result = Pipe()
						.linesFromFile(input_file_path)
						.map<std::string, std::string>(map_encrypt)
						.collect<std::string, std::vector>();

		} else if( mode == "decrypt" ) {
		 	result = Pipe()
						.linesFromFile(input_file_path)
						.map<std::string, std::string>(map_decrypt)
						.collect<std::string, std::vector>();
		}

		if( result.size() > 0 ) {
   		std::ofstream myFile;
   		myFile.open(output_file_path);
   		for(unsigned int i = 0; i < result.size(); i++)
	   		myFile << result[i];
   		myFile.close( );
		}
	}


   return 0;
}
