#include "ColorPicker.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSlider>
#include <QColor>
#include <QPalette>

ColorPicker::ColorPicker( QWidget* parent )
{
	QBoxLayout* pickerLayout = new QVBoxLayout( this ) ;
	pickerLayout->setContentsMargins( 0, 0, 0, 0 ) ;


	display = new QLabel( this ) ;
	display->setMinimumSize( 100, 100 ) ;
	display->setMaximumWidth( 300 ) ;
	display->setAutoFillBackground( true ) ;
	pickerLayout->addWidget( display ) ;

	QHBoxLayout* sliderLayout = new QHBoxLayout() ;

	red = new ColorSlider( this ) ;
	green = new ColorSlider( this ) ;
	blue = new ColorSlider( this ) ;

	sliderLayout->addWidget( red ) ;
	sliderLayout->addWidget( green ) ;
	sliderLayout->addWidget( blue ) ;

	pickerLayout->addLayout( sliderLayout ) ;

	connect( red, &ColorSlider::valueChanged, this, &ColorPicker::updateColor );
	connect( blue , &ColorSlider::valueChanged, this, &ColorPicker::updateColor );
	connect( green, &ColorSlider::valueChanged, this, &ColorPicker::updateColor );

	updateColor() ;
}


void ColorPicker::updateColor() {
	QColor color( 
		red->value(), 
		green->value(), 
		blue->value() 
	) ;
	QPalette palette = display->palette() ;
	palette.setColor( QPalette::Window, color ) ;
	display->setPalette( palette ) ;
}