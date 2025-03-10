#include "ColorPicker.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSlider>
#include <QColor>
#include <QPalette>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

ColorPicker::ColorPicker( QWidget* parent )
{
	pickingColor = false ;
	QBoxLayout* pickerLayout = new QVBoxLayout( this ) ;
	pickerLayout->setContentsMargins( 0, 0, 0, 0 ) ;


	display = new QLabel( this ) ;
	display->setMinimumSize( 100, 100 ) ;
	display->setMaximumWidth( 300 ) ;
	display->setAutoFillBackground( true ) ;
	pickerLayout->addWidget( display ) ;


	// Generate the RGB
	QHBoxLayout* sliderLayout = new QHBoxLayout() ;

	red = new ColorSlider( this ) ;
	green = new ColorSlider( this ) ;
	blue = new ColorSlider( this ) ;

	sliderLayout->addWidget( red ) ;
	sliderLayout->addWidget( green ) ;
	sliderLayout->addWidget( blue ) ;

	pickerLayout->addLayout( sliderLayout ) ;

	connect( red, &ColorSlider::valueChanged, this, &ColorPicker::updateColorFromSlider );
	connect( blue , &ColorSlider::valueChanged, this, &ColorPicker::updateColorFromSlider );
	connect( green, &ColorSlider::valueChanged, this, &ColorPicker::updateColorFromSlider );

	// Set up the defaults for the hex input
	hexcode = new QLineEdit( this ) ;
	hexcode->setAlignment( Qt::AlignCenter ) ;
	hexcode->setPlaceholderText( "#RRGGBB" ) ;
	hexcode->setMaxLength( 7 );
	QRegularExpression hexregex( "^#([A-Fa-f0-8]{6})$" ) ;
	hexcode->setValidator( new QRegularExpressionValidator( hexregex, this ) ) ;

	pickerLayout->addWidget( hexcode ) ;

	connect( hexcode, &QLineEdit::editingFinished, this, &ColorPicker::updateColorFromHex ) ;

	// Create the eyedropper
	eyedropper = new QPushButton( "Pick Color", this ) ;
	pickerLayout->addWidget( eyedropper ) ;

	connect( eyedropper, &QPushButton::clicked, this, &ColorPicker::activateEyedropper ) ;

	updateColorFromSlider() ;
}


void ColorPicker::mousePressEvent( QMouseEvent* event )
{
	if( pickingColor ) {
		pickingColor = false ;
		setCursor( Qt::ArrowCursor ) ;

	}
}


void ColorPicker::updateColorFromSlider() {
	QColor color( 
		red->value(), 
		green->value(), 
		blue->value() 
	) ;
	QPalette palette = display->palette() ;
	palette.setColor( QPalette::Window, color ) ;
	display->setPalette( palette ) ;

	hexcode->setText( color.name( QColor::HexRgb ) ) ;
}

void ColorPicker::updateColorFromHex() {
	QString hex = hexcode->text().trimmed() ;
	QColor color( hex ) ;
	if( hex.startsWith("#") && color.isValid()) {
		red->setValue( color.red() ) ;
		green->setValue( color.green() ) ;
		blue->setValue( color.blue() ) ;
		updateColorFromSlider() ;
	}
}

void ColorPicker::activateEyedropper() {
	pickingColor = true ;
	setCursor( Qt::CrossCursor ) ;
}