#include <iostream>
#include <string>
#include <cmath>

using namespace std;

double antiPlagiarism(string text, string fragment);
bool isSeparator(char c);
bool isNumber(char c);
bool isSmallWord(string word);
bool isWrongWord(string word);
void cutSeparators(string text, char canonizedText[]);
void cutWrongWords(char canonizedText[]);
int getLength(string text);
void toLowerCase(char canonizedText[]) ;
void canonize(string text, char canonizedText[]);
int getWordsNumber(char canonizedText[]);
void fillArray(string textWordsArray[], char canonizedText[]);
long RSHash(string str);
long RSHash(int array[], int shingleLength);
void fillHashsArray(string textWordsArray[], long textWordsHashsArray[], int textWordsNumber);
double getMatchesPercentage(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber);
int getWordsMatch(int shingleLength, int i, int j, string textWordsArray[], string fragmentWordsArray[]);
double getPercentage(int coincidences, int fragmentWordsNumber);


const int CHARACTER_A_ASCII = 65;
const int CHARACTER_Z_ASCII = 90;
const int LOWERCASE_UPPERCASE_INTERVAL = 32;
const char TERMINAL_NULL = '\0';
const char SPACE = ' ';
const string EMPTY_STRING = "";

int main()
{
	string text = "            ....London capital            Great ............ The Britain, the the the its political, economic                cultural centre. It's  largest cities in the world. Its population is more than million people. London is situated on the river Thames. The city is very old and beautiful. It was founded more than two thousand years ago. Traditionally London is divided into several parts:  City, the West End, the East End and Westminster. The City is the oldest part of London, its financial and business centre. The heart  City  Stock Exchange. Westminster is the most important part  capital. It's administrative centre. The Houses of Parliament, the seat of the British Government, are there. It's a very beautiful building with two towers and a very big clock called Big Ben. Big Ben is really the bell which strikes every quarter of an hour. Opposite the Houses of Parliament is Westminster Abbey. It's a very beautiful church built over 900 years ago. The tombs of many great statesmen, scientists and writers are there.       ";
	string fragment = "         Minsk  capital  Belarus, its political, economic and cultural centre. It's one  largest cities in the world. Its population is more than million people. London is situated on the river Thames. The city is very old and beautiful. It was founded more than two thousand years ago. Traditionally London is divided into several parts: the City, the West End,  East End and Westminster.  City   oldest part of London, its financial and business centre. The heart of the City is the Stock Exchange. Westminster is the most important part of the capital. It's the administrative centre. Houses of Parliament, the seat of the British Government, are there. It's a very beautiful building with two towers and a very big clock called Big Ben. Hello World!           ";
 	
	cout << "Plagiarism percentage: " << antiPlagiarism(text, fragment) << "%";

	return 0;
}

double antiPlagiarism(string text, string fragment)
{
	const int TEXT_LENGTH = getLength(text);
	const int FRAGMENT_LENGTH = getLength(fragment);
	
	if (TEXT_LENGTH == 0 or FRAGMENT_LENGTH == 0) {
		return 0.0;
	}
	
	char canonizedText[TEXT_LENGTH];
	char canonizedFragment[FRAGMENT_LENGTH];

	canonize(text, canonizedText);
	canonize(fragment, canonizedFragment);

	int textWordsNumber = getWordsNumber(canonizedText);
	int fragmentWordsNumber = getWordsNumber(canonizedFragment);
	
	if (textWordsNumber == 0 or fragmentWordsNumber == 0) {
		return 0.0;
	}
	
	string textWordsArray[textWordsNumber];
	string fragmentWordsArray[fragmentWordsNumber];
	
	fillArray(textWordsArray, canonizedText);
	fillArray(fragmentWordsArray, canonizedFragment);
	
	long textWordsHashsArray[textWordsNumber];
	long fragmentWordsHashsArray[fragmentWordsNumber];
	
	fillHashsArray(textWordsArray, textWordsHashsArray, textWordsNumber);
	fillHashsArray(fragmentWordsArray, fragmentWordsHashsArray, fragmentWordsNumber);
	
	return getMatchesPercentage(textWordsArray, fragmentWordsArray, textWordsNumber, fragmentWordsNumber);
}

bool isSeparator(char c)
{
	char separator[] = " ~`!@#$%^&*-_=+,./({[<>]});:?\n";

	for (int i = 0; separator[i] != TERMINAL_NULL; i++) {
		if (c == separator[i]) {
			return true;
		}
	}

	return false;
}

bool isNumber(char c)
{
	char number[] = "0123456789";
	
	for (int i = 0; number[i] != TERMINAL_NULL; i++) {
		if (c == number[i]) {
			return true;
		}
	}
		
	return false;
}

bool isSmallWord(string word)
{
	int wordLength = getLength(word);
	
	const int MIN_WORD_LENGTH = 3;
	
	if (wordLength < MIN_WORD_LENGTH) {
		return true;
	}
	
	return false;
}

bool isWrongWord(string word)
{
	const int N = 12;
	string wrongWords[N] = {"the", "with", "under", "and", "for", "from", "between", "into", "out", "over", "that", "among"};
	
	for (int i = 0; i < N; i++) {
		if (word == wrongWords[i]) {
			return true;
		}
	}
		
	return false;
}

void toLowerCase(char canonizedText[]) 
{
	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if ((canonizedText[i] >= CHARACTER_A_ASCII) and (canonizedText[i] <= CHARACTER_Z_ASCII)) {
			canonizedText[i] = canonizedText[i] + LOWERCASE_UPPERCASE_INTERVAL;
		}
	}
}

int getLength(string text)
{
	int counter = 0;

	for (int i = 0; text[i] != TERMINAL_NULL; i++) {
		counter++;
	}

	return counter;
}

void cutSeparators(string text, char canonizedText[])
{
	int index = 0;
	
	for (int i = 0; text[i] != TERMINAL_NULL; i++) {
		if (!isSeparator(text[i]) and !isNumber(text[i])) {
				canonizedText[index] = text[i];
				index++;
			
			if (isSeparator(text[i + 1]) or isNumber(text[i + 1]) or text[i + 1] == TERMINAL_NULL) {
				canonizedText[index] = SPACE;
				index++;
			}
		}
	}
	
	canonizedText[index] = TERMINAL_NULL;
}

void cutWrongWords(char canonizedText[])
{
	int index = 0;
	string word = EMPTY_STRING;
	
	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if (canonizedText[i] != SPACE) {
			word += canonizedText[i];
			
			if (canonizedText[i + 1] == SPACE or canonizedText[i + 1] == TERMINAL_NULL) {
				if (!isSmallWord(word) and !isWrongWord(word)) {
					int wordLength = getLength(word);
				
					for (int i = 0; i < wordLength; i++) {
						canonizedText[index] = word[i];
						index++;
					}
					
					canonizedText[index] = SPACE;
					index++;
				}
				
				word = EMPTY_STRING;
			}
		}
	}
	
	canonizedText[index] = TERMINAL_NULL;
}

void canonize(string text, char canonizedText[])
{
	cutSeparators(text, canonizedText);
	toLowerCase(canonizedText);
	cutWrongWords(canonizedText);
}

int getWordsNumber(char canonizedText[])
{
	int wordsCounter = 0;

	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if (canonizedText[i] == SPACE or canonizedText[i + 1] == TERMINAL_NULL) {
			wordsCounter++;
		}
	}
	
	return wordsCounter;
}

void fillArray(string textWordsArray[], char canonizedText[])
{
	string word = EMPTY_STRING;
	int index = 0;
	
	for (int i = 0; canonizedText[i] != TERMINAL_NULL; i++) {
		if (canonizedText[i] != SPACE) {
			word += canonizedText[i];
		} else {
			textWordsArray[index] = word;
			index++;
			word = EMPTY_STRING;
		}
	}
}

long RSHash(string str)  
{  
    int b = 378551;
    int a = 63689;
    long hash = 0;
    
    for(int i = 0; i < getLength(str); i++)
    {
        hash = hash * a + (int)str[i];
        a    = a * b;
    }
    
    return hash;
}

long RSHash(int array[], int shingleLength)
{
	int b = 378551;
    int a = 63689;
    long hash = 0;
    
    for(int i = 0; i < shingleLength; i++)
    {
        hash = hash * a + array[i];
        a    = a * b;
    }
    
    return hash;
}

void fillHashsArray(string textWordsArray[], long textWordsHashsArray[], int textWordsNumber)
{
	for (int i = 0; i < textWordsNumber; i++) {
		textWordsHashsArray[i] = RSHash(textWordsArray[i]);
	}
}

double getMatchesPercentage(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber)
{
	int shinglesMatch = 0;
	int shingleLength = 3;
	
	if (textWordsNumber < 4 or fragmentWordsNumber < 4) {
		shingleLength = 1;
	}
	
	int textShinglesNumber = textWordsNumber - shingleLength + 1;
	int fragmentShinglesNumber = fragmentWordsNumber - shingleLength + 1;

	for (int i = 0; i < fragmentShinglesNumber; i++) {
		for (int j = 0; j < textShinglesNumber; j++) {
			int wordsMatch = getWordsMatch(shingleLength, i, j, textWordsArray, fragmentWordsArray);
			
			if (wordsMatch == shingleLength) {
				shinglesMatch++;
				break;
			}		
		}
	}
	
	return getPercentage(shinglesMatch, fragmentShinglesNumber);		
}

int getWordsMatch(int shingleLength, int i, int j, string textWordsArray[], string fragmentWordsArray[])
{
	int wordsMatch = 0;
			
	for (int k = 0; k < shingleLength; k++) {
		if (fragmentWordsArray[i + k] == textWordsArray[j + k]) {
			wordsMatch++;
		}
	}
	
	return wordsMatch;
}

double getPercentage(int shinglesMatch, int fragmentShinglesNumber)
{
	double percentage = round(shinglesMatch * 10000.0 / fragmentShinglesNumber) / 100;
	return percentage;
}
