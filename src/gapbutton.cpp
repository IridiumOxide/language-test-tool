#include "gapbutton.h"

GapButton::GapButton(qint32 id, QWidget *parent) : QPushButton("?", parent){
	isAnswered = false;
	withImage = false;
	ID = id;
	setStyleSheet("background-color: lightblue; color: black");
	icon = QIcon();
}

GapButton::GapButton(qint32 id, QString imagePath, QWidget *parent) : QPushButton(parent){
	isAnswered = false;
	withImage = true;
	ID = id;
	QPixmap pixmap(imagePath);
	pixmap = pixmap.scaled(QSize(100, 50));
	icon = QIcon(pixmap);
	setIcon(icon);
	setIconSize(pixmap.rect().size());
}

void GapButton::getFilled(QString phrase){
	isAnswered = true;
	if(withImage)
		setIcon(QIcon());
	setStyleSheet("background-color: yellow; color: black");
	setText(phrase);
}

void GapButton::restore(){
	isAnswered = false;
	if(withImage){
		setStyleSheet("");
		setText("");
		setIcon(icon);
	}else{
		setStyleSheet("background-color: lightblue; color: black");
		setText("?");
	}
}

void GapButton::highlight(){
	setStyleSheet("background-color: darkblue; color: white");
}

void GapButton::unhighlight(){
	setStyleSheet("background-color: yellow; color: black");
}

bool GapButton::answered(){
	return isAnswered;
}

qint32 GapButton::getID(){
	return ID;
}
