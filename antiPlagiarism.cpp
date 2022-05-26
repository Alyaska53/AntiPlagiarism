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
bool isSeparator(char c);
int length(string text);
int canonization(string text, char textArray[]);
void arrayCreation(char text[], string arrayOfWords[]);
int shingleRepetition(string textWordsArray[], string fragmentWordsArray[], int textWordsCount, int fragmentWordsCount);
char separator[]=" ,.!?";

int main()
{
	string text = "Hello  . My name    is Liza. I   am   23 years old. I like ??? programming! This is my first c++ project.";
	string fragment = "Hello  . My name    is . I   am   23 years . I like ??? shopping! This is my first java project.";
	
	int textLength = length(text);
	int fragmentLength = length(fragment);
	
	char canonizedText[textLength];
	char canonizedFragment[fragmentLength];
	
	int textWordsCount = canonization(text, canonizedText);
	int fragmentWordsCount = canonization(fragment, canonizedFragment);
	
	string textWordsArray[textWordsCount];
	string fragmentWordsArray[fragmentWordsCount];
	
	arrayCreation(canonizedText, textWordsArray);
	arrayCreation(canonizedFragment, fragmentWordsArray);
	
	int matches = shingleRepetition(textWordsArray, fragmentWordsArray, textWordsCount, fragmentWordsCount);
	
	cout << matches;
	
	return 0;
}

bool isSeparator(char c)
{
	
    for(int i = 0; separator[i] != '\0'; i++) {
    	if(c == separator[i]) {
    		return true;
    	}
    }
    
	return false;
}

int getLength(string text)
{
	int counter = 0;

	for (int i = 0; text[i] != '\0'; i++) {
		counter++;
	}

	return counter;
}

int canonization(string text, char textArray[])
{
	int index = 0;
	int wordsCounter = 0;
	
	for (int i = 0; text[i] != '\0'; i++) {
    	
        if (!isSeparator(text[i])) {
	        textArray[index] = text[i];
	        index++;
	    
	        if (isSeparator(text[i + 1]) or text[i + 1] == '\0') {
	            textArray[index] = ' ';
	            index++;
	            wordsCounter++;
        	}	
        }
    }
    
	for(int i = index; text[i] != '\0'; i++) {
		textArray[i] = '\0';
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
			}
		}
	}

	return coincidences;
}



