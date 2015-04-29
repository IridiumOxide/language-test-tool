#include<QVector>

#include "window.h"
Window::Window()
{
	controlsLayout = new QGridLayout();
	controls = new QWidget();

	choicesLayout = new FlowLayout;
	choicesViewPort = new QWidget();
	choices = new QScrollArea();

	sentenceLayout = new FlowLayout;
	sentenceViewPort = new QWidget();
	sentence = new QScrollArea();

	mainWindowLayout = new QGridLayout();

	controls->setLayout(controlsLayout);

	choicesViewPort->setLayout(choicesLayout);
	choices->setWidget(choicesViewPort);
	choices->setWidgetResizable(true);

	sentenceViewPort->setLayout(sentenceLayout);
	sentence->setWidget(sentenceViewPort);
	sentence->setWidgetResizable(true);

	mainWindowLayout->addWidget(sentence, 0, 0, 1, 2);
	mainWindowLayout->addWidget(choices, 1, 0);
	mainWindowLayout->addWidget(controls, 1, 1);

	setLayout(mainWindowLayout);

	setWindowTitle("Language Test");
	currentlySelectedButton = nullptr;
	currentlySelectedGap = nullptr;

	totalMaxPoints = -1;
	totalCurrentPoints = 0;
	currentTestCase = 0;

	QPushButton *startButton = new QPushButton("LOAD TEST DATA\nAND START");
	connect(startButton, &QPushButton::clicked, this, &Window::testStart);
	controlsLayout->addWidget(startButton, 0, 0);

	QPushButton *exitButton = new QPushButton("QUIT");
	connect(exitButton, &QPushButton::clicked, this, &Window::close);
	controlsLayout->addWidget(exitButton, 1, 0);

}

void Window::fillGap(GapButton *gapButton){
	if(gapButton->answered()){
		if(currentlySelectedButton != nullptr){
			currentlySelectedButton->setStyleSheet("background-color : white; color : black");
			currentlySelectedButton = nullptr;
			gapButton->highlight();
			currentlySelectedGap = gapButton;
		}else if(currentlySelectedGap == nullptr){
			gapButton->highlight();
			currentlySelectedGap = gapButton;
		}else if(currentlySelectedGap == gapButton){
			gapButton->unhighlight();
			currentlySelectedGap = nullptr;
		}else{
			currentlySelectedGap->unhighlight();
			gapButton->highlight();
			currentlySelectedGap = gapButton;
		}
	}else{
		if(currentlySelectedButton != nullptr && currentlySelectedGap == nullptr){
			qint32 index = gapButton->getID();
			QString phrase = currentlySelectedButton->text();
			gapButton->getFilled(phrase);
			testCases[currentTestCase].pickChoice(phrase, index, true);
			choicesLayout->removeWidget(currentlySelectedButton);
			delete currentlySelectedButton;
			currentlySelectedButton = nullptr;
		}else if(currentlySelectedButton == nullptr && currentlySelectedGap != nullptr){
			qint32 oldIndex = currentlySelectedGap->getID();
			qint32 newIndex = gapButton->getID();
			QString transferredPhrase = testCases[currentTestCase].getPickedChoice(oldIndex);
			gapButton->getFilled(transferredPhrase);
			testCases[currentTestCase].pickChoice(transferredPhrase, newIndex, false);
			testCases[currentTestCase].pickChoice("", oldIndex, false);
			currentlySelectedGap->restore();
			currentlySelectedGap = nullptr;
		}
	}
}

void Window::buttoniseTestCase(){
	buttoniseControlsWidget();
	buttoniseChoicesWidget();
	buttoniseSentenceWidget();
}

void Window::unbuttoniseTestCase(){
	clearLayout(controlsLayout);
	clearLayout(choicesLayout);
	clearLayout(sentenceLayout);
}

void Window::rebuttoniseTestCase(){
	unbuttoniseTestCase();
	buttoniseTestCase();
}

void Window::buttoniseControlsWidget(){
	controlsLayout->addWidget(new QLabel(tr("CASE %1 / %2").arg(currentTestCase + 1).arg(totalTestCases)), 0, 0, 4, 1);
	controlsLayout->addWidget(new QLabel(tr("THIS CASE: %1 / %2").arg(testCases[currentTestCase].getCurrentPoints()).arg(testCases[currentTestCase].getMaxPoints())), 4, 0, 4, 1);
	controlsLayout->addWidget(new QLabel(tr("TOTAL: %1 / %2").arg(totalCurrentPoints).arg(totalMaxPoints)), 8, 0, 4, 1);

	QPushButton *resetButton = new QPushButton(tr("RESET CASE"));
	connect(resetButton, &QPushButton::clicked, this, &Window::resetTestCase);
	controlsLayout->addWidget(resetButton, 3, 1, 3, 1);

	QPushButton *evalButton = new QPushButton(tr("EVALUATE CASE"));
	connect(evalButton, &QPushButton::clicked, this, &Window::evaluateTestCase);
	controlsLayout->addWidget(evalButton, 0, 1, 3, 1);
	if(testCases[currentTestCase].evaluated()){
		evalButton->setDisabled(true);
	}

	if(currentTestCase != totalTestCases - 1){
		QPushButton *nextButton = new QPushButton(tr("NEXT CASE"));
		connect(nextButton, &QPushButton::clicked, this, &Window::nextTestCase);
		controlsLayout->addWidget(nextButton, 6, 1, 3, 1);
	}else{
		QPushButton *endButton = new QPushButton(tr("FINISH"));
		connect(endButton, &QPushButton::clicked, this, &Window::finish);
		controlsLayout->addWidget(endButton, 6, 1, 3, 1);

	}

	QPushButton *previousButton = new QPushButton(tr("PREVIOUS CASE"));
	connect(previousButton, &QPushButton::clicked, this, &Window::previousTestCase);
	controlsLayout->addWidget(previousButton, 9, 1, 3, 1);
	if(currentTestCase == 0)
		previousButton->setDisabled(true);
}

void Window::buttoniseChoicesWidget(){
	for(int i=0; i<testCases[currentTestCase].choicesCount(); i++){
		if(testCases[currentTestCase].choiceTaken(i) == false){
			QPushButton *button = new QPushButton(testCases[currentTestCase].getChoiceAt(i));
			button->setStyleSheet("background-color: white; color : black ");
			connect(button, &QPushButton::clicked, [this, button] () {
				if(currentlySelectedGap != nullptr){
					currentlySelectedGap->unhighlight();
					currentlySelectedGap = nullptr;
				}
				if(currentlySelectedButton == button){
					button->setStyleSheet("background-color : white; color : black");
					currentlySelectedButton = nullptr;
				}else{
					if(currentlySelectedButton != nullptr){
						currentlySelectedButton->setStyleSheet("background-color : white; color : black");
					}
					currentlySelectedButton = button;
					button->setStyleSheet("background-color : black; color : white");
				}
			} );
			choicesLayout->addWidget(button);
			if(testCases[currentTestCase].evaluated()){
				button->setDisabled(true);
				button->setStyleSheet("color: grey");
			}
		}
	}

}

void Window::buttoniseSentenceWidget(){
	qint32 whichGap = 0;
	for(int i=0; i<testCases[currentTestCase].wordsCount(); i++){
		if(testCases[currentTestCase].gap(i)){
			GapButton *gapButton;
			if(testCases[currentTestCase].getImagePath(whichGap) == ""){
				gapButton = new GapButton(whichGap, this);
			}else{
				gapButton = new GapButton(whichGap, testCases[currentTestCase].getImagePath(whichGap), this);
			}
			QString alreadyChosen = testCases[currentTestCase].getPickedChoice(whichGap);
			if(alreadyChosen != ""){
				gapButton->getFilled(alreadyChosen);
			}
			sentenceLayout->addWidget(gapButton);
			connect(gapButton, &GapButton::clicked, [this, gapButton](){ fillGap(gapButton); });
			if(testCases[currentTestCase].evaluated()){
				gapButton->setDisabled(true);
				gapButton->show();
				if(gapButton->answered()){
					if(testCases[currentTestCase].correct(whichGap)){
						gapButton->setStyleSheet("background-color: green; color: black");
					}else{
						gapButton->setStyleSheet("background-color: red; color: black");
					}
				}else{
					gapButton->setIcon(QIcon());
					gapButton->setStyleSheet("background-color: red; color: black");
					gapButton->setText("-");
				}
			}
			whichGap++;
		}else{
			sentenceLayout->addWidget(new QLabel(testCases[currentTestCase].getWordAt(i), this));
		}
	}
}

void Window::clearLayout(QLayout *layout){
	QLayoutItem *item;
	while((item = layout->takeAt(0))) {
		if (item->layout()) {
			clearLayout(item->layout());
			delete item->layout();
		}
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
}

void Window::testStart(){
	testCases.clear();
	totalMaxPoints = loadTestData(testCases);
	if(totalMaxPoints >= 0){		// -1 oznaczałoby, że plik nie został wczytany
		clearLayout(controlsLayout);
		totalTestCases = testCases.size();
		buttoniseTestCase();
	}
}

void Window::nextTestCase(){
	currentlySelectedButton = nullptr;
	currentlySelectedGap = nullptr;
	currentTestCase++;
	rebuttoniseTestCase();
}

void Window::previousTestCase(){
	currentlySelectedButton = nullptr;
	currentlySelectedGap = nullptr;
	currentTestCase--;
	rebuttoniseTestCase();
}

void Window::resetTestCase(){
	currentlySelectedButton = nullptr;
	currentlySelectedGap = nullptr;
	totalCurrentPoints -= testCases[currentTestCase].getCurrentPoints();
	testCases[currentTestCase].reset();
	rebuttoniseTestCase();
}

void Window::evaluateTestCase(){
	currentlySelectedButton = nullptr;
	currentlySelectedGap = nullptr;
	testCases[currentTestCase].evaluate();
	totalCurrentPoints += testCases[currentTestCase].getCurrentPoints();
	rebuttoniseTestCase();
}

void Window::finish(){
	unbuttoniseTestCase();

	delete controlsLayout;
	delete controls;

	delete choicesLayout;
	delete choicesViewPort;
	delete choices;

	delete sentenceLayout;
	delete sentenceViewPort;
	delete sentence;

	delete mainWindowLayout;

	QGridLayout *endingButtonLayout = new QGridLayout(this);
	QPushButton *endingButton = new QPushButton("THE END", this);
	endingButton->setStyleSheet("font-size: 72px");
	connect(endingButton, &QPushButton::clicked, this, &Window::close);
	QLabel *finalScoreLabel = new QLabel(tr("FINAL SCORE: %1 / %2").arg(totalCurrentPoints).arg(totalMaxPoints), this);
	endingButtonLayout->addWidget(endingButton, 0, 0, 1, 2);
	endingButtonLayout->addWidget(finalScoreLabel, 1, 0, 1, 2);
	setLayout(endingButtonLayout);
}
