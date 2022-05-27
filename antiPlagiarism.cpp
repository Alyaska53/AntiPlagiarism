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
int getCoincidencesNumber(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber);
double getPercentage(int coincidences, int fragmentWordsNumber);

int main()
{
	string text = "            ....London capital  Great ............ The Britain, the the the its political, economic                cultural centre. It's  largest cities in the world. Its population is more than million people. London is situated on the river Thames. The city is very old and beautiful. It was founded more than two thousand years ago. Traditionally London is divided into several parts:  City, the West End, the East End and Westminster. The City is the oldest part of London, its financial and business centre. The heart  City  Stock Exchange. Westminster is the most important part  capital. It's administrative centre. The Houses of Parliament, the seat of the British Government, are there. It's a very beautiful building with two towers and a very big clock called Big Ben. Big Ben is really the bell which strikes every quarter of an hour. Opposite the Houses of Parliament is Westminster Abbey. It's a very beautiful church built over 900 years ago. The tombs of many great statesmen, scientists and writers are there.       ";
	string fragment = "         Minsk  capital  Belarus, its political, economic and cultural centre. It's one  largest cities in the world. Its population is more than million people. London is situated on the river Thames. The city is very old and beautiful. It was founded more than two thousand years ago. Traditionally London is divided into several parts: the City, the West End,  East End and Westminster.  City   oldest part of London, its financial and business centre. The heart of the City is the Stock Exchange. Westminster is the most important part of the capital. It's the administrative centre. Houses of Parliament, the seat of the British Government, are there. It's a very beautiful building with two towers and a very big clock called Big Ben. Hello World!           ";
 	
 	//string text = "   ABCD ABCDE  ...         ABCDEF   ";
 	//string fragment = "   ABCD ABCDE... ABCDEF   ";
 	
	cout << "Result: " << antiPlagiarism(text, fragment);

	return 0;
}

double antiPlagiarism(string text, string fragment)
{
	if (text == "" or fragment == "") {
		return 0.0;
	}
	
	const int TEXT_LENGTH = getLength(text);
	const int FRAGMENT_LENGTH = getLength(fragment);
	
	char canonizedText[TEXT_LENGTH];
	char canonizedFragment[FRAGMENT_LENGTH];

	canonize(text, canonizedText);
	canonize(fragment, canonizedFragment);

	int textWordsNumber = getWordsNumber(canonizedText);
	int fragmentWordsNumber = getWordsNumber(canonizedFragment);
	
	string textWordsArray[textWordsNumber];
	string fragmentWordsArray[fragmentWordsNumber];
	
	fillArray(textWordsArray, canonizedText);
	fillArray(fragmentWordsArray, canonizedFragment);

	int coincidences = getCoincidencesNumber(textWordsArray, fragmentWordsArray, textWordsNumber, fragmentWordsNumber);
	
	return getPercentage(coincidences, fragmentWordsNumber); //TODO: round to two decimal places
}

bool isSeparator(char c)
{
	char separator[] = " ~`!@#$%^&*-_=+,./({[<>]})?\n";

	for (int i = 0; separator[i] != '\0'; i++) {
		if (c == separator[i]) {
			return true;
		}
	}

	return false;
}

bool isNumber(char c)
{
	char number[] = "0123456789";
	
    for (int i = 0; number[i] != '\0'; i++) {
    	if (c == number[i]) {
    		return true;
    	}
    }
    
	return false;
}

bool isSmallWord(string word)
{
	int wordLength = getLength(word);
	
	if (wordLength < 3) {
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
	for (int i = 0; canonizedText[i] != '\0'; i++) {
		if ((canonizedText[i] >= 65) and (canonizedText[i] <= 90)) {
			canonizedText[i] = canonizedText[i] + 32;
		}
	}
}

int getLength(string text)
{
	int counter = 0;

	for (int i = 0; text[i] != '\0'; i++) {
		counter++;
	}

	return counter;
}

void cutSeparators(string text, char canonizedText[])
{
	int index = 0;
	
	for (int i = 0; text[i] != '\0'; i++) {
		if (!isSeparator(text[i]) and !isNumber(text[i])) {
				canonizedText[index] = text[i];
			    index++;
			
			if (isSeparator(text[i + 1]) or isNumber(text[i + 1]) or text[i + 1] == '\0') {
				canonizedText[index] = ' ';
				index++;
			}
		}
	}
	
	canonizedText[index] = '\0';
}

void cutWrongWords(char canonizedText[])
{
	int index = 0;
	string word = "";
	
	for (int i = 0; canonizedText[i] != '\0'; i++) {
		if (canonizedText[i] != ' ') {
			word += canonizedText[i];
			
			if (canonizedText[i + 1] == ' ' or canonizedText[i + 1] == '\0') {
				if (!isSmallWord(word) and !isWrongWord(word)) {
					int wordLength = getLength(word);
				
					for (int i = 0; i < wordLength; i++) {
						canonizedText[index] = word[i];
						index++;
					}
					
					canonizedText[index] = ' ';
					index++;
				}
				
				word = "";
			}
		}
	}
	
	canonizedText[index] = '\0';
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

	for (int i = 0; canonizedText[i] != '\0'; i++) {
		if (canonizedText[i] == ' ' or canonizedText[i + 1] == '\0') {
			wordsCounter++;
		}
	}
	
	return wordsCounter;
}

void fillArray(string textWordsArray[], char canonizedText[])
{
	string word = "";
	int index = 0;
	
	for (int i = 0; canonizedText[i] != '\0'; i++) {
		if (canonizedText[i] != ' ') {
			word += canonizedText[i];
		} else {
			textWordsArray[index] = word;
			index++;
			word = "";
		}
	}
}

int getCoincidencesNumber(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber)
{
	int coincidences = 0;
	
	// TODO: check for the words number in the array

	for (int i = 0; i < (fragmentWordsNumber - 2); i++) {

		for (int j = 0; j < (textWordsNumber - 2); j++) {
			if ((fragmentWordsArray[i] == textWordsArray[j]) and
				(fragmentWordsArray[i + 1] == textWordsArray[j + 1]) and
				(fragmentWordsArray[i + 2] == textWordsArray[j + 2])) {
					
				coincidences++;
				
				break;
			}
		}
	}

	return coincidences;
}

double getPercentage(int coincidences, int fragmentWordsNumber)
{
	double percentage = round(coincidences * 10000.0 / (fragmentWordsNumber - 2)) / 100;
	return percentage;
}
