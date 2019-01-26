#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "playfair.h"

using namespace std;

void prepare(const char* input, char* output){

  unsigned i = 0;
  string temp = "";

  // ignore non-alphanumeric characters
  while (i < strlen(input)){

    if (isalnum(input[i])){

      temp += string(1, input[i]);
    }
    
    i++;
  }
  
  // convert to uppercase
  transform(temp.begin(), temp.end(), temp.begin(), ::toupper);

  // append X if not even
  if (temp.length() % 2 != 0){
    temp += "X";
  }

  strcpy(output, temp.c_str());
}

void grid(const char* codeword, char square[6][6]){

  unsigned row = 0;
  unsigned col = 0;
  unsigned i = 0;
  bool repeat = false;
  
  while (row < 6){

    while (col < 6){

      // This part is only executed once the codeword has been completed
      // It adds in the rest of the unused alphanumeric characters
      if (i == strlen(codeword)){

	for (unsigned j = 'A'; j < ('Z' + 1); j++){
	  for (unsigned k = 0; k < strlen(codeword); k++){
	    if (codeword[k] == (char) j){
	      break;
	    }
	    if (k == strlen(codeword) -1){
	      square[row][col] = (char) j;
	      if (j == 'Z'){
		
		if (col == 5){
		  col = 0;
		  row++;
		} else {
		  col++;
		}
		goto numbers;
		
	      } else if (col == 5){
		col = 0;
		row++;
	      } else {
		col++;
	      }
	    }
	  }
	}

      numbers:
	
	for (unsigned j = '0'; j < ('9' + 1); j++){
	  for (unsigned k = 0; k < strlen(codeword); k++){
	    if (codeword[k] == (char) j){
	      break;
	    }
	    if (k == strlen(codeword) -1){
	      square[row][col] = (char) j;
	      if (j == '9'){
		return;
	      } else if (col == 5){
		col = 0;
		row++;
	      } else {
		col++;
	      }
	    }
	  }
	}
	
	// Add in characters of codeword checking for duplicates
      } else {

	for (unsigned j = 0; j < i; j++){
	  if (codeword[i] == codeword[j]){
	    repeat = true;
	    break;
	  }
	  j++;
	}

	if (repeat == false){
	  square[row][col] = codeword[i];
	  col++;
	}
	
	i++;	
      }
      
      repeat = false;
    }
    
    col = 0;
    row++;
  }
}
      
void bigram(char square[6][6], char inchar1, char inchar2, char& outchar1, char& outchar2){

  unsigned row = 0;
  unsigned col = 0;
  unsigned inchar1_row;
  unsigned inchar1_col;
  unsigned inchar2_row;
  unsigned inchar2_col;
  
  // loop through the square until a match is found
  while (row < 6){

    while (col < 6){

      if (square[row][col] == inchar1){
	inchar1_row = row;
	inchar1_col = col;
      }
      
      if (square[row][col] == inchar2){
	inchar2_row = row;
	inchar2_col = col;
      }
      
      col++;
    }

    col = 0;
    row++;
  }
  
  // assign the match according to the spec
  outchar1 = square[inchar1_row][inchar2_col];
  outchar2 = square[inchar2_row][inchar1_col];

}

void encode(char square[6][6], const char* prepared, char* encoded){

  // recursive solution using pointer arithmetic
  
  unsigned i = 0;
  char char1 = ' ';
  char char2 = ' ';
  string temp = prepared;
  static int count = 0;

  if (temp.length() % 2 == 0){
    
    bigram(square, temp[i], temp[i+1], char1, char2);

    *(encoded + count) = char1;
    *(encoded + count + 1) = char2;
  }

  if (strlen(prepared) == 2){
    return;
  }

  count = count + 2;
  temp = temp.substr(2, temp.length() - 2);
  encode(square, temp.c_str(), encoded);
}

void decode(char square[6][6], const char* encoded, char* decoded){

  // brute force approach taken
  
  char char1 = ' ';
  char char2 = ' ';
  char char3;
  char char4;
  unsigned x = 0;
  string temp = encoded;
  static int count = 0;
  
  for (int r = 0; r < 6; r++){
    
    for (int c = 0; c < 6; c++){

      char1 = square[r][c];
      
      for (int i = 0; i < 6; i++){
	
	for (int j = 0; j < 6; j++){

	  char2 = square[i][j];

	  //cout << char1 << char2 << endl;
	  
	  bigram(square, char1, char2, char3, char4);
	  
	  //cout << "test" << endl;
	  if (char3 == encoded[x] &&
	      char4 == encoded[x+1]){

	    temp = temp.substr(2, temp.length() - 2);
	    *(decoded + count) = char1;
	    *(decoded + count + 1) = char2;
	    count = count + 2;

	    if (strlen(encoded) == 2){
	      return;
	    }

	    decode(square, temp.c_str(), decoded);
	  }
	}
      }
    }
  }
}
