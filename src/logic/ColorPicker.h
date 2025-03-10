#pragma once
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "ColorSlider.h"
#include <QEvent>
#include <QObject>

class ColorPicker : public QWidget {
	Q_OBJECT
public:
	explicit ColorPicker( QWidget* parent = nullptr ) ;

protected:
	bool eventFilter( QObject* obj, QEvent* event ) override ;

private slots:
	void updateColorFromSlider() ;
	void updateColorFromHex() ;
	void activateEyedropper() ;
	void togglePicker() ;

private:
	QLabel* display ;
	QLineEdit* hexcode ;
	ColorSlider* red ;
	ColorSlider* green ;
	ColorSlider* blue ;
	QPushButton* eyedropper ; 
	bool pickingColor ;
	QWidget* colorPickerWidget ;
	QPushButton* toggleButton ;
};
