#pragma once
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>
#include "ColorSlider.h"

class ColorPicker : public QFrame {
	Q_OBJECT
public:
	explicit ColorPicker( QWidget* parent = nullptr ) ;

protected:
	void mousePressEvent( QMouseEvent* event ) override ;
private slots:
	void updateColorFromSlider() ;
	void updateColorFromHex() ;
	void activateEyedropper() ;
private:
	QLabel* display ;
	QLineEdit* hexcode ;
	ColorSlider* red ;
	ColorSlider* green ;
	ColorSlider* blue ;
	QPushButton* eyedropper ; 
	bool pickingColor ;
};
