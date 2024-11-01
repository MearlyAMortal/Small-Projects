#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

const char lettersLowerArray[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
const char lettersUpperArray[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
const char numbersArray[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const char specialArray[] = {'@', '#', '$', '%', '&', '*'};

const char* charArrays[] = {lettersLowerArray, lettersUpperArray, numbersArray, specialArray};
const int arraySizes[] = {sizeof(lettersLowerArray), sizeof(lettersUpperArray), sizeof(numbersArray), sizeof(specialArray)};
const int totalCharArrays = sizeof(charArrays) / sizeof(charArrays[0]);

const int ARRAYSIZE = 204;


bool CheckInput(int argc, char* argv[]){
  if ( argc == 2){ return true; }
  std::cout << "ERROR: Input Invalid" << std::endl;
  exit(1);
}


void PrintList(char charList[], int size){
  for ( int i = 0; i < size; i++ ){
    std::cout << charList[i];
  }
  std::cout << std::endl;
}


char* FillPassword(char allCharsRandom[], char password[], int size){
  if (size > ARRAYSIZE) {
        return nullptr;
  }

  for (int i = 0; i < size; i++) {
    int randomIndex = std::rand() % ARRAYSIZE; 
    password[i] = allCharsRandom[randomIndex];
  }

  password[size] = '\0';
  return password;
}


char* FillAllCharsRandom(char allCharsRandom[]){
  for ( int i = 0; i < ARRAYSIZE; i++ ){
    int randomArrayIndex = std::rand() % totalCharArrays;
    int randomIndex = std::rand() % arraySizes[randomArrayIndex];
    allCharsRandom[i] = charArrays[randomArrayIndex][randomIndex];
  }
  return allCharsRandom;

}





int main(int argc, char* argv[]){
  std::srand(static_cast<unsigned int>(std::time(0)));

  CheckInput(argc, argv);

  // Random seed.
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  // Create and fill list allCharsRandom[] of ARRAYSIZE with random chars.
  char allCharsRandom[ARRAYSIZE];
  FillAllCharsRandom(allCharsRandom);

  // Create and fill password with length specified by user.
  const int password_length = std::stoi(argv[1]);
  char password[password_length];
  FillPassword(allCharsRandom, password, password_length);

  PrintList(password, password_length);
  //std::cout << "Password: " << PrintList(password, password_length);

  // DEBUG ONLY
  //PrintList(allCharsRandom, ARRAYSIZE);



  return 0;
}
