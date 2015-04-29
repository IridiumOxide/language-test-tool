#ifndef GAPBUTTON_H
#define GAPBUTTON_H

#include<QtWidgets>

class GapButton : public QPushButton
{
public:
	GapButton(qint32 id, QWidget *parent);
	GapButton(qint32 id, QString imagePath, QWidget *parent);
	void getFilled(QString);
	void restore();
	void highlight();
	void unhighlight();

	qint32 getID();
	bool answered();

private:
	bool withImage;
	qint32 ID;
	bool isAnswered;
	QIcon icon;
};

#endif // GAPBUTTON_H
