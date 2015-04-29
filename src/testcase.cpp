#include "testcase.h"

#include<algorithm>

TestCase::TestCase()
{
	currentPoints = 0;
	maxPoints = 0;
	isEvaluated = false;
}

void TestCase::evaluate(){
	currentPoints = 0;
	for(int i = 0; i < gapWords.size(); i++){
		if(gapWords[i] == pickedChoices[i]){
			isCorrect[i] = true;
			currentPoints++;
		}
	}
	isEvaluated = true;
}

void TestCase::reset(){
	currentPoints = 0;
	isEvaluated = false;
	for(int i = 0; i < pickedChoices.size(); i++){
		pickedChoices[i] = "";
		isCorrect[i] = false;
	}
	for(int i = 0; i < choices.size(); i++){
		isChoiceTaken[i] = false;
	}
}

bool caseInsensitiveComp(const QString &x, const QString &y){
	int c = QString::compare(x, y, Qt::CaseInsensitive);
	if(c == 0){
		return x > y;
	}else if(c > 0){
		return false;
	}else{
		return true;
	}
}

void TestCase::sortChoices(){
	std::sort(choices.begin(), choices.end(), caseInsensitiveComp);
}

void TestCase::addChoice(QString choice){
	choices.push_back(choice);
	isChoiceTaken.push_back(false);
}

void TestCase::addWord(QString word, bool gap){
	words.push_back(word);
	isaGap.push_back(gap);
	if(gap){
		maxPoints++;
		pickedChoices.push_back("");
		gapWords.push_back(word);
		isCorrect.push_back(false);
		imagePaths.push_back("");
		addChoice(word);
	}
}

void TestCase::changeImagePath(QString path, qint32 index){
	imagePaths[index] = path;
}

void TestCase::pickChoice(QString choice, qint32 index, bool fromChoiceBox){
	pickedChoices[index] = choice;
	if(fromChoiceBox){
		for(int i=0; i<choices.size(); i++){
			if(choices[i] == choice && isChoiceTaken[i] != true){
				isChoiceTaken[i] = true;
				break;
			}
		}
	}
}

qint32 TestCase::getCurrentPoints(){
	return currentPoints;
}

qint32 TestCase::getMaxPoints(){
	return maxPoints;
}

qint32 TestCase::wordsCount(){
	return words.size();
}

qint32 TestCase::choicesCount(){
	return choices.size();
}

bool TestCase::gap(qint32 index){
	return isaGap[index];
}

bool TestCase::correct(qint32 index){
	return isCorrect[index];
}

bool TestCase::evaluated(){
	return isEvaluated;
}

QString TestCase::getWordAt(qint32 index){
	return words[index];
}

QString TestCase::getChoiceAt(qint32 index){
	return choices[index];
}

QString TestCase::getImagePath(qint32 index){
	return imagePaths[index];
}

bool TestCase::choiceTaken(qint32 index){
	return isChoiceTaken[index];
}

QString TestCase::getPickedChoice(qint32 index){
	return pickedChoices[index];
}
