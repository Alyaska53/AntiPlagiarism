/****************************************************************************

Вопросы:
1. Нужно ли избавляться от чисел при канонизации текста?
2. Что если в тексте повторяются одинаковые выборки?
3. Какие знаки препинания надо убирать?

Улучшения:
1. Как будем хешировать?

Алгоритм:
1. Не нужно избавляться от апострофов и пробелов
2. Убираем все остальные знаки препинания
3. Получив отредактированный текст, разбиваем его на слова.
4. Пункты 1-3 повторяем с фрагментом.
5. Считаем количество выборок в фрагменте
6. Объявляем счетчик совпадений
7. Идем по выборкам фрагмента и сравниваем с выборками текста
8. После сравнения возвращаем отношение счетчика к количеству выборок

Рассуждения:
! Возможно, выгоднее все-таки разбить массивы слов на массивы выборок.
! Если смотреть немного наперед, скорее всего, мы в дальнейшем будем
! хешировать шинглы. А это удобнее будет делать из массива выборок.
! Да и в циклах будет проще работать с целыми выборками, чем динамически
! их генерировать. Это надо обсудить...

****************************************************************************/

#include <iostream>
#include <string>

using namespace std;

double antiPlagiarism(string text, string fragment);
bool isSeparator(char c);
bool isNumber(char c);
int getLength(string text);
void toLowerCase(char canonizedText[]) ;
void canonize(string text, char canonizedText[]);
int getWordsNumber(string canonizedText);
void fillArray(string textWordsArray[], string canonizedText);
int getCoincidencesNumber(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber);

int main()
{
	string text = "        ....London is the capital of Great ............Britain, its political, economic and               cultural centre. It's one of the largest cities in the world. Its population is more than million people. London is situated on the river Thames. The city is very old and beautiful. It was founded more than two thousand years ago. Traditionally London is divided into several parts: the City, the West End, the East End and Westminster. The City is the oldest part of London, its financial and business centre. The heart of the City is the Stock Exchange. Westminster is the most important part of the capital. It's the administrative centre. The Houses of Parliament, the seat of the British Government, are there. It's a very beautiful building with two towers and a very big clock called Big Ben. Big Ben is really the bell which strikes every quarter of an hour. Opposite the Houses of Parliament is Westminster Abbey. It's a very beautiful church built over 900 years ago. The tombs of many great statesmen, scientists and writers are there.";
	string fragment = "Minsk is the capital of Belarus, its political, economic and cultural centre. It's one of the largest cities in the world. Its population is more than million people. London is situated on the river Thames. The city is very old and beautiful. It was founded more than two thousand years ago. Traditionally London is divided into several parts: the City, the West End, the East End and Westminster. The City is the oldest part of London, its financial and business centre. The heart of the City is the Stock Exchange. Westminster is the most important part of the capital. It's the administrative centre. The Houses of Parliament, the seat of the British Government, are there. It's a very beautiful building with two towers and a very big clock called Big Ben. Hello World!";

	cout << "Result: " << antiPlagiarism(text, fragment);

	return 0;
}

double antiPlagiarism(string text, string fragment)
{
	const int TEXT_LENGTH = getLength(text);
	const int FRAGMENT_LENGTH = getLength(fragment);
	
	char canonizedText[TEXT_LENGTH];
	char canonizedFragment[FRAGMENT_LENGTH];

	canonize(text, canonizedText);
	canonize(fragment, canonizedFragment);

	int textWordsNumber = getWordsNumber(canonizedText);
	int fragmentWordsNumber = getWordsNumber(canonizedFragment);
	
	string textWordsArray[textWordsNumber];
	string fragmentWordsArray[textWordsNumber];
	
	fillArray(textWordsArray, canonizedText);
	fillArray(fragmentWordsArray, canonizedFragment);

	int coincidences = getCoincidencesNumber(textWordsArray, fragmentWordsArray, textWordsNumber, fragmentWordsNumber);
	
	return coincidences * 1.0 / (fragmentWordsNumber - 2);
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

void canonize(string text, char canonizedText[])
{
	int index = 0;
	
	for (int i = 0; text[i] != '\0'; i++) {
		if (!isSeparator(text[i]) and !isNumber(text[i])) {
			canonizedText[index] = text[i];
			index++;
		    
			if (isSeparator(text[i + 1]) or isNumber(text[i + 1]) or text[i + 1] == '\0') {
				canonizedText[index] = ' ';
			}
			
				index++;
			}
		}
	}
	
	canonizedText[index] = '\0';
	
	toLowerCase(canonizedText);
}

int getWordsNumber(string canonizedText)
{
	int wordsCounter = 0;

	for (int i = 0; canonizedText[i] != '\0'; i++) {
		if (canonizedText[i] == ' ' or canonizedText[i + 1] == '\0') {
			wordsCounter++;
		}
	}
	
	return wordsCounter;
}

void fillArray(string textWordsArray[], string canonizedText)
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
