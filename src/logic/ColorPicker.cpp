#include "ColorPicker.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSlider>
#include <QColor>
#include <QPalette>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QApplication>
#include <QMouseEvent>
#include <QScreen>

ColorPicker::ColorPicker( QWidget* parent )
{
	QHBoxLayout* mainLayout = new QHBoxLayout( this ) ;
	pickingColor = false ;

	toggleButton = new QPushButton( "<<", this ) ;
	toggleButton->setFixedWidth( 20 ) ;
	toggleButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding ) ;
	mainLayout->addWidget( toggleButton ) ;

	

	colorPickerWidget = new QWidget( this ) ;
	QBoxLayout* pickerLayout = new QVBoxLayout( colorPickerWidget ) ;
	pickerLayout->setContentsMargins( 0, 0, 0, 0 ) ;
	colorPickerWidget->setVisible( false ) ;
	mainLayout->addWidget( colorPickerWidget ) ;


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

	connect( toggleButton, &QPushButton::clicked, this, &ColorPicker::togglePicker ) ;
	connect( eyedropper, &QPushButton::clicked, this, &ColorPicker::activateEyedropper ) ;
	qApp->installEventFilter( this ) ;

	updateColorFromSlider() ;
}


bool ColorPicker::eventFilter( QObject* obj, QEvent* event )
{
	if( pickingColor && event->type() == QEvent::MouseButtonPress ) {
		QMouseEvent* mouseEvent = static_cast< QMouseEvent * >( event ) ;
		pickingColor = false ;
		QApplication::setOverrideCursor( Qt::ArrowCursor ) ;
		QScreen* screen = QApplication::primaryScreen() ;
		if( !screen ) return false ;
		QPixmap image = screen->grabWindow( 0, mouseEvent->globalX(), mouseEvent->globalY(), 1, 1 );

		QColor pickedColor = image.toImage().pixelColor( 0, 0 ) ;
		if( pickedColor.isValid() ) {
			red->setValue( pickedColor.red() ) ;
			green->setValue( pickedColor.green() ) ;
			blue->setValue( pickedColor.blue() ) ;

			updateColorFromSlider() ;
		}
		return true ;
	}

	return QWidget::eventFilter( obj, event ) ;
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
	QApplication::setOverrideCursor( Qt::CrossCursor ) ;
}

void ColorPicker::togglePicker()
{
	bool visible = colorPickerWidget->isVisible() ;
	colorPickerWidget->setVisible( !visible ) ;
	toggleButton->setText( visible ? "<<" : ">>" ) ;
}
