#pragma once

#include <QWidget>
#include <QSlider>
#include <QLineEdit>

class ColorSlider : public QWidget {
	Q_OBJECT

public:
	explicit ColorSlider( QWidget* parent = nullptr ) ;

	int value() const ;
	void setValue( int val ) ;

signals:
	void valueChanged( int newValue ) ;

private slots:
	void onSliderChanged( int value ) ;
	void onTextChanged() ;

private:
	QSlider* slider ;
	QLineEdit* lineEdit ;
};