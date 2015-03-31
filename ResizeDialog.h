#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialogButtonBox>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class ResizeDialog : public QDialog
{
    Q_OBJECT

public:
    ResizeDialog(int setX=1, int setY=1, int minX=1, int minY=1, int maxX=200, int maxY=200, QString title="Resize Window");
	int getWidth();
	int getHeight();

public slots:
	void enforceARWidth();
	void enforceARHeight();
	void updateAR();

private:
	double currentAR;
	QLabel *widthLabel;
	QLabel *heightLabel;
	QCheckBox *aspectRatioCheckBox;
	QSpinBox *widthBox;
	QSpinBox *heightBox;
	QDialogButtonBox *buttonBox;
};

#endif
