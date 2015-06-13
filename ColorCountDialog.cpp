#include <QtGui>

#include "ColorCountDialog.h"

ColorCountDialog::ColorCountDialog(QVector<QRgb> *colors, QVector<int> *counts, QString title)
{
	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

	QScrollArea *scrollArea = new QScrollArea;
	scrollArea->setAlignment(Qt::AlignCenter);

	QGroupBox *bigFrame = new QGroupBox;
	QVBoxLayout *wLayout = new QVBoxLayout;
	for (int i=0; i<colors->count(); i++)
	{
		if (qAlpha(colors->value(i))>127)
		{
			QGroupBox *boxFrame = new QGroupBox;
			QHBoxLayout *hLayout = new QHBoxLayout;
			QPixmap box(20,20);
			box.fill(colors->value(i));
			QLabel *pixmapFrame = new QLabel(QString(""));
			pixmapFrame->setPixmap(box);
			hLayout->addWidget(pixmapFrame);
			QLabel *textFrame = new QLabel(QString(""));
			textFrame->setText(QString(": "+QString::number(counts->value(i))));
			hLayout->addWidget(textFrame);
			boxFrame->setLayout(hLayout);
			wLayout->addWidget(boxFrame);
		}
	}
	bigFrame->setLayout(wLayout);

	scrollArea->setWidget(bigFrame);
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(scrollArea, 0, 0);
	mainLayout->addWidget(buttonBox, 1, 0);
	setLayout(mainLayout);

	setWindowTitle(title);
}


