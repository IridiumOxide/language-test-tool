#ifndef TESTCASE_H
#define TESTCASE_H

#include<QVector>
#include<QString>

class TestCase
{
public:
	TestCase();
	void evaluate();
	void reset();
	void sortChoices();
	void addChoice(QString choice);
	void addWord(QString word, bool gap);
	void changeImagePath(QString path, qint32 index);
	void pickChoice(QString choice, qint32 index, bool fromChoiceBox);
	qint32 getCurrentPoints();
	qint32 getMaxPoints();
	qint32 wordsCount();
	qint32 choicesCount();
	bool gap(qint32 index);
	bool correct(qint32 index);
	bool evaluated();
	QString getWordAt(qint32 index);
	QString getImagePath(qint32 index);
	QString getChoiceAt(qint32 index);

	bool choiceTaken(qint32 index);
	QString getPickedChoice(qint32 index);

private:
	qint32 currentPoints;
	qint32 maxPoints;


	QVector<bool> isaGap;
	QVector<bool> isCorrect;
	bool isEvaluated;

	QVector<QString> words;
	QVector<QString> gapWords;
	QVector<QString> imagePaths; // jeśli nie podano obrazka, puste słowo symbolizuje domyślny wygląd luki
	QVector<QString> choices; // posortowane alfabetycznie

	// Zapamiętywanie dotychczasowych wyborów, żeby dało się wracać do poprzednich zdań
	QVector<bool> isChoiceTaken;
	QVector<QString> pickedChoices;
};

#endif // TESTCASE_H
