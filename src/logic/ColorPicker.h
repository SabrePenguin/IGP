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

class ColorButton : public QPushButton {
	Q_OBJECT
public:
	explicit ColorButton( QWidget* parent = nullptr, bool selected = false, const QColor& start = Qt::black ) ;
	void setButtonColor( const QColor& color ) ;
	void setLabelColor( const QColor& color ) ;
	void setInactive() ;
	QLabel* getLabel();
	QColor getColor() ;
signals:
	void selectionChanged( bool selected ) ;
public slots:
	void onClick() ;
private:
	QLabel* innerColor ;
	QColor currentColor ;
	QColor borderColor ;
	bool selected ;
};


class ColorTab : public QTabWidget {
	Q_OBJECT
public:
	explicit ColorTab( QWidget* parent = nullptr, const QColor& start = Qt::black ) ;
	QLabel* getLabel();
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
	// Eyedropper
	QPushButton* eyedropper ; 
	bool pickingColor ;
	QWidget* colorPickerWidget ;
	QTabWidget* tab;
	// Collapse
	QPushButton* toggleButton ;
};
