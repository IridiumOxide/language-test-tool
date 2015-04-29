#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>
#include <QWidget>
#include <QVector>
#include "testcase.h"
#include "flowlayout.h"
#include "gapbutton.h"
#include "parser.h"

class QLabel;

class Window : public QWidget
{
	Q_OBJECT

public:
	Window();
	void fillGap(GapButton *gapButton);
	void buttoniseTestCase();
	void unbuttoniseTestCase();
	void rebuttoniseTestCase();
	void buttoniseControlsWidget();
	void buttoniseChoicesWidget();
	void buttoniseSentenceWidget();
	void clearLayout(QLayout *layout);
	void testStart();
	void nextTestCase();
	void previousTestCase();
	void resetTestCase();
	void evaluateTestCase();
	void finish();

private:
	QPushButton *currentlySelectedButton;
	GapButton *currentlySelectedGap;
	QVector<TestCase> testCases;
	qint32 currentTestCase;
	qint32 totalTestCases;
	qint32 totalCurrentPoints;
	qint32 totalMaxPoints;

	QGridLayout *mainWindowLayout;

	QGridLayout *controlsLayout;
	QWidget *controls;

	FlowLayout *choicesLayout;
	QWidget *choicesViewPort;
	QScrollArea *choices;

	FlowLayout *sentenceLayout;
	QWidget *sentenceViewPort;
	QScrollArea *sentence;
};

#endif
