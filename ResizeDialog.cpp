#include <QtGui>

#include "ResizeDialog.h"

ResizeDialog::ResizeDialog(int setX, int setY, int minX, int minY, int maxX, int maxY, QString title)
{
    widthLabel = new QLabel(tr("Width:"));
	heightLabel = new QLabel(tr("Height:"));
	heightLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	widthBox = new QSpinBox;
	widthBox->setMinimum(minX);
	widthBox->setMaximum(maxX);
	widthBox->setValue(setX);
	heightBox = new QSpinBox;
	heightBox->setMinimum(minY);
	heightBox->setMaximum(maxY);
	heightBox->setValue(setY);

	currentAR = setX/setY;

	aspectRatioCheckBox = new QCheckBox(tr("Keep current aspect ratio of image"));

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(widthBox, SIGNAL(valueChanged(int)), this, SLOT(enforceARWidth()));
	connect(heightBox, SIGNAL(valueChanged(int)), this, SLOT(enforceARHeight()));
	connect(aspectRatioCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateAR()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(widthLabel, 0, 0);
	mainLayout->addWidget(heightLabel, 0, 1);
	mainLayout->addWidget(widthBox, 1, 0);
	mainLayout->addWidget(heightBox, 1, 1);
	mainLayout->addWidget(aspectRatioCheckBox, 2, 0);
	mainLayout->addWidget(buttonBox, 2, 1);
	setLayout(mainLayout);

	setWindowTitle(title);
}

int ResizeDialog::getWidth()
{
	return widthBox->value();
}

int ResizeDialog::getHeight()
{
	return heightBox->value();
}

void ResizeDialog::enforceARWidth()
{
	if (aspectRatioCheckBox->checkState()==Qt::Checked)
	{
		heightBox->setValue(int(widthBox->value()/currentAR));
	}
}

void ResizeDialog::enforceARHeight()
{
	if (aspectRatioCheckBox->checkState()==Qt::Checked)
	{
		widthBox->setValue(int(heightBox->value()*currentAR));
	}
}

void ResizeDialog::updateAR()
{
	currentAR = widthBox->value()/heightBox->value();
}

