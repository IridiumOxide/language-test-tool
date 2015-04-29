#include "parser.h"
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

enum errorType{
	characterNotFound,
	wrongOpeningTag,
	wrongClosingTag,
	noSpaceAfterTag,
	wrongTestFormat,
	unknownCommandTag,
	somethingWentReallyWrong,
	previousTestCaseNotClosed,
	noTestCaseOpen,
};

void displayErrorMessage(errorType error, qint32 lineNumber, QChar x = 0){
	QMessageBox msgBox;
	QString errorText;
	errorText = "LINE " + QString::number(lineNumber) +":\n";
	if(error == characterNotFound){
		errorText += "Expected character '";
		errorText += x;
		errorText += "'\nNever found it.";
	}else if(error == wrongOpeningTag){
		errorText += "Wrong opening tag";
	}else if(error == wrongClosingTag){
		errorText += "Wrong closing or testCase tag";
	}else if(error == noSpaceAfterTag){
		errorText += "No space after tag";
	}else if(error == wrongTestFormat){
		errorText += "Wrong test format";
	}else if(error == unknownCommandTag){
		errorText += "Unknown command tag";
	}else if(error == somethingWentReallyWrong){
		errorText += "Something went really wrong.\nSorry.";
	}else if(error == previousTestCaseNotClosed){
		errorText += "Previous test case not closed";
	}else if(error == noTestCaseOpen){
		errorText += "No test case open";
	}
	msgBox.setText(errorText);
	msgBox.setWindowTitle("ERROR");
	msgBox.exec();
}

// po wykonaniu readUntilCharacter, source[sourceIndex] jest znakiem, do którego wczytywaliśmy.
bool readUntilCharacter(QChar character, QString &destination, QString &source, qint32 &sourceIndex, qint32 lineNumber){
	while(source[sourceIndex] != character){
		if(source[sourceIndex] == '&')
			sourceIndex++;
		destination += source[sourceIndex];
		sourceIndex++;
		if(sourceIndex >= source.length()){
			if(character == ' ')	// koniec linii traktujemy w tym wypadku jako spację
				return true;
			displayErrorMessage(characterNotFound, lineNumber, character);
			return false;
		}
	}
	return true;
}

// po wykonaniu checkFormat, source[sourceIndex] jest pierwszym znakiem po sprawdzanym wzorcu.
// zmienna closing ustawiona na true oznacza, że po odczytanym wzorcu musi znajdować się spacja
// lub koniec linii.
bool checkFormat(QString &source, QString correct, qint32 &sourceIndex, qint32 lineNumber, bool closing = false){
	if(sourceIndex + correct.length() - 1 >= source.length()){
		if(closing == false){
			displayErrorMessage(wrongOpeningTag, lineNumber);
		}else{
			displayErrorMessage(wrongClosingTag, lineNumber);
		}
		return false;
	}
	for(int i=0; i<correct.length(); i++){
		if(source[sourceIndex] != correct[i]){
			if(closing == false){
				displayErrorMessage(wrongOpeningTag, lineNumber);
			}else{
				displayErrorMessage(wrongClosingTag, lineNumber);
			}
			return false;
		}
		sourceIndex++;
	}
	if(closing && sourceIndex < source.length() && source[sourceIndex] != ' '){
		displayErrorMessage(noSpaceAfterTag, lineNumber);
		return false;
	}
	return true;
}

int loadTestData(QVector<TestCase> &testCases){
	bool testCaseOpen = false;
	qint32 lineNumber = 0;
	qint32 casesCount = 0;
	qint32 totalMaxPoints = 0;
	qint32 gapCount = 0;
	QString filename = QFileDialog::getOpenFileName(nullptr, "Choose test file", ".", "Text Files (*.txt)");
	QFile file(filename);
	QString imageDirectory = QFileInfo(file).path() + "/";
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "error opening file: " << file.error();
		return -1;
	}
	QTextStream instream(&file);
	QString line = "";
	while(!instream.atEnd()){
		line = instream.readLine().trimmed();
		lineNumber++;
		/*********************************************
		 * jeśli w testach występuje sekwencja &X,
		 * gdzie X to dowolny znak, to X dodajemy
		 * do wynikowego słowa, tzn. &< da nam <
		 * w wynikowym słowie, &" da " itd.
		 *********************************************/
		for(int i=0; i<line.length(); i++){
			if(line[i]=='<'){
				i++;
				if(line[i] == 'i'){				// wczytujemy lukę z obrazkiem
					QString imagePath = "";
					QString word = "";
					if(!checkFormat(line, "=\"", ++i, lineNumber)){
						return -1;
					}
					if(i >= line.length()){
						displayErrorMessage(wrongTestFormat, lineNumber);
						return -1;
					}
					if(!readUntilCharacter('"', imagePath, line, i, lineNumber)){
						return -1;
					}
					if(!(checkFormat(line, "\">", i, lineNumber))){
						return -1;
					}
					if(i >= line.length()){
						displayErrorMessage(wrongTestFormat, lineNumber);
						return -1;
					}
					if(!readUntilCharacter('<', word, line, i, lineNumber)){
						return -1;
					}
					testCases[casesCount].addWord(word, true);
					testCases[casesCount].changeImagePath(imageDirectory + imagePath, gapCount);
					gapCount++;
					if(!(checkFormat(line, "</i>", i, lineNumber, true))){
						return -1;
					}
				}else if(line[i] == 'g'){			// wczytujemy lukę bez obrazka
					QString word = "";
					if(!(checkFormat(line, ">", ++i, lineNumber))){
						return -1;
					}
					if(i >= line.length()){
						displayErrorMessage(wrongTestFormat, lineNumber);
						return -1;
					}
					if(!readUntilCharacter('<', word, line, i, lineNumber)){
						return -1;
					}
					testCases[casesCount].addWord(word, true);
					gapCount++;
					if(!(checkFormat(line, "</g>", i, lineNumber, true))){
						return -1;
					}
				}else if(line[i] == 'e'){			// wczytujemy dodatkowe słowa
					QString word = "";
					if(!(checkFormat(line, ">", ++i, lineNumber))){
						return -1;
					}
					if(i >= line.length()){
						displayErrorMessage(wrongTestFormat, lineNumber);
						return -1;
					}
					if(!readUntilCharacter('<', word, line, i, lineNumber)){
						return -1;
					}
					testCases[casesCount].addChoice(word);
					if(!(checkFormat(line, "</e>", i, lineNumber, true))){
						return -1;
					}
				}else if(line[i] == 't'){			// tworzymy nowy, pusty TestCase
					if(!(checkFormat(line, ">", ++i, lineNumber, true))){
						return -1;
					}
					if(testCaseOpen == true){
						displayErrorMessage(previousTestCaseNotClosed, lineNumber);
						return -1;
					}
					testCaseOpen = true;
					testCases.push_back(TestCase());
				}else if(line[i] == '/'){			// zatwierdzamy zakończenie parsowania TestCase'a
					if(!(checkFormat(line, "t>", ++i, lineNumber, true))){
						return -1;
					}
					if(testCaseOpen == false){
						displayErrorMessage(noTestCaseOpen, lineNumber);
						return -1;
					}
					testCaseOpen = false;
					testCases[casesCount].sortChoices();
					totalMaxPoints += testCases[casesCount].getMaxPoints();
					casesCount++;
					gapCount = 0;
				}else{
					displayErrorMessage(unknownCommandTag, lineNumber);
				}
			}else if(line[i] != ' '){
				if(testCaseOpen == false){
					displayErrorMessage(noTestCaseOpen, lineNumber);
					return -1;
				}
				QString word = "";
				if(!readUntilCharacter(' ', word, line, i, lineNumber)){
					displayErrorMessage(somethingWentReallyWrong, lineNumber);
					return -1;
				}
				testCases[casesCount].addWord(word, false);
			}
		}
	}
	if(testCaseOpen == true){
		displayErrorMessage(previousTestCaseNotClosed, lineNumber);
		return -1;
	}
	return totalMaxPoints;
}
