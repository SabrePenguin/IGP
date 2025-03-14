#pragma once
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "ColorSlider.h"
#include <QEvent>
#include <QObject>
#include <QColor>

class ColorButton : public QPushButton {
	Q_OBJECT
public:
	explicit ColorButton( QWidget* parent = nullptr, bool selected = false, const QColor& start = Qt::black ) ;
	void setButtonColor( const QColor& color ) ;
	void setLabelColor( const QColor& color ) ;
private:
	QLabel* innerColor ;
	QColor currentColor ;
	QColor borderColor ;
	bool selected ;
};

class ColorPicker : public QWidget {
	Q_OBJECT
public:
	explicit ColorPicker( QWidget* parent = nullptr ) ;

public slots:
	void overrideSelectedColor( QColor color ) ;

signals:
	void colorChanged( QColor color ) ;

protected:
	bool eventFilter( QObject* obj, QEvent* event ) override ;

private slots:
	void updateColorFromSlider() ;
	void updateColorFromHex() ;
	void activateEyedropper() ;
	void togglePicker() ;

private:
	void setSliders( const QColor& color ) ;
	// Sliders
	QLabel* leftColorDisplay ;
	QLabel* rightColorDisplay ;
	ColorButton* leftButton ;
	QLineEdit* hexcode ;
	ColorSlider* red ;
	ColorSlider* green ;
	ColorSlider* blue ;
	// Eyedropper
	QPushButton* eyedropper ; 
	bool pickingColor ;
	QWidget* colorPickerWidget ;
	// Collapse
	QPushButton* toggleButton ;
};
