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
int getLength(string text);
string canonize(string text, int textLength);
int getWordsNumber(string canonizedText);
void fillArray(string textWordsArray[], string canonizedText);
int getCoincidencesNumber(string textWordsArray[], string fragmentWordsArray[], int textWordsNumber, int fragmentWordsNumber);

int main()
{
	string text = "I'm going to the shop to buy an apples";
	string fragment = "I'm going to the shop to buy a bananas";

	cout << "Result: " << antiPlagiarism(text, fragment);

	return 0;
}

double antiPlagiarism(string text, string fragment)
{
	const int TEXT_LENGTH = getLength(text);
	const int FRAGMENT_LENGTH = getLength(fragment);

	string canonizedText = canonize(text, TEXT_LENGTH);
	string canonizedFragment = canonize(fragment, FRAGMENT_LENGTH);

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
	char separator[] = " ~`!@#$%^&*()-_=+,./{[]}}?";

	for (int i = 0; separator[i] != '\0'; i++) {
		if (c == separator[i]) {
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

string canonize(string text, int textLength)
{
	string canonizedText = "";
	int index = 0;
	
	for (int i = 0; text[i] != '\0'; i++) {
		if (!isSeparator(text[i])) {
			canonizedText += text[i];
			index++;

			if (isSeparator(text[i + 1]) or text[i + 1] == '\0') {
				canonizedText += ' ';
				index++;
			}
		}
	}
	
	return canonizedText;
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
			}
		}
	}

	return coincidences;
}
