#pragma once
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "ColorSlider.h"

class ColorPicker : public QFrame {
	Q_OBJECT
public:
	explicit ColorPicker( QWidget* parent = nullptr ) ;

private slots:
	void updateColorFromSlider() ;
	void updateColorFromHex() ;
private:
	QLabel* display ;
	QLineEdit* hexcode ;
	ColorSlider* red ;
	ColorSlider* green ;
	ColorSlider* blue ;
	QPushButton* eyedropper ; 
	bool pickingColor ;
};
