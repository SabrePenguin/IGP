#pragma once

#include <QWidget>
#include <QSlider>
#include <QLineEdit>

class ColorSlider : public QWidget {
	Q_OBJECT

public:
	explicit ColorSlider( QWidget* parent = nullptr ) ;

private:
	QSlider* slider ;
	QLineEdit* lineEdit ;
};