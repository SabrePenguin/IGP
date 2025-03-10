#pragma once
#include <QFrame>
#include <QLabel>
#include "ColorSlider.h"

class ColorPicker : public QFrame {
	Q_OBJECT
public:
	explicit ColorPicker( QWidget* parent = nullptr ) ;

private slots:
	void updateColor() ;
private:
	QLabel* display ;
	ColorSlider* red ;
	ColorSlider* green ;
	ColorSlider* blue ;
};
