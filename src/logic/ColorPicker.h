#pragma once
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "ColorSlider.h"
#include <QEvent>
#include <QObject>
#include <QColor>
#include <QTabWidget>


class ColorTab : public QTabWidget {
	Q_OBJECT
public:
	explicit ColorTab( QWidget* parent = nullptr, const QColor& start = Qt::black ) ;
	QLabel* getLabel();
	void overrideColor( const QColor& color ) ;
signals:
	void colorChanged( QColor color ) ;
private slots:
	void updateColorFromSlider() ;
	void updateColorFromHex() ;
	void setColor( const QColor& new_color, bool emitter = true );
private:
	ColorSlider* red;
	ColorSlider* blue;
	ColorSlider* green;
	QLabel* current_color;
	QLineEdit* hexcode;
};


class ColorPicker : public QWidget {
	Q_OBJECT
public:
	explicit ColorPicker( QWidget* parent = nullptr ) ;
	void overrideSelectedColor( QColor color ) ;

signals:
	void colorChanged( QColor color ) ;
	void swappedButton() ;

protected:
	bool eventFilter( QObject* obj, QEvent* event ) override ;

private slots:
	void activateEyedropper() ;
	void togglePicker() ;

private:
	ColorTab* left_tab ;
	ColorTab* right_tab ;
	// Eyedropper
	QPushButton* eyedropper ; 
	bool pickingColor ;
	QWidget* colorPickerWidget ;
	QTabWidget* tab;
	// Collapse
	QPushButton* toggleButton ;
};
