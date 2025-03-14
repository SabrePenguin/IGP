#include "ColorPicker.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSlider>
#include <QPoint>
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
	colorPickerWidget->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding ) ;
	mainLayout->addWidget( colorPickerWidget ) ;

	QBoxLayout* labelLayout = new QHBoxLayout() ;
	leftColorDisplay = new QLabel( this ) ;
	leftColorDisplay->setMinimumSize( 100, 50 ) ;
	leftColorDisplay->setMaximumWidth( 300 ) ;
	leftColorDisplay->setAutoFillBackground( true ) ;
	rightColorDisplay = new QLabel( this ) ;
	rightColorDisplay->setMinimumSize( 100, 50 ) ;
	rightColorDisplay->setMaximumWidth( 300 ) ;
	rightColorDisplay->setAutoFillBackground( true ) ;

	leftButton = new ColorButton( this, true ) ;

	labelLayout->addWidget( leftColorDisplay ) ;
	labelLayout->addWidget( rightColorDisplay ) ;
	labelLayout->addWidget( leftButton ) ;
	pickerLayout->addLayout( labelLayout ) ;


	// Generate the RGB
	QHBoxLayout* sliderLayout = new QHBoxLayout() ;

	red = new ColorSlider( this ) ;
	red->setValue( 255 ) ;
	green = new ColorSlider( this ) ;
	green->setValue( 255 ) ;
	blue = new ColorSlider( this ) ;
	blue->setValue( 255 ) ;

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
	QRegularExpression hexregex( "^#?([A-Fa-f0-8]{6})$" ) ;
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

void ColorPicker::overrideSelectedColor( QColor color ) {
	qDebug() << "Selected color: " << color.name() ;
	setSliders( color ) ;
	QPalette palette = leftColorDisplay->palette() ;
	palette.setColor( QPalette::Window, color ) ;
	leftColorDisplay->setPalette( palette ) ;

	hexcode->setText( color.name( QColor::HexRgb ) ) ;
}



bool ColorPicker::eventFilter( QObject* obj, QEvent* event )
{
	if( pickingColor && event->type() == QEvent::MouseButtonPress ) {
		QMouseEvent* mouseEvent = static_cast< QMouseEvent * >( event ) ;
		pickingColor = false ;
		QApplication::setOverrideCursor( Qt::ArrowCursor ) ;
		QPoint position = mouseEvent->globalPos() ;
		QScreen* screen = QApplication::screenAt( position ) ;
		if( !screen ) return false ;

		QPixmap image = screen->grabWindow( 0, position.x() - screen->geometry().x(), position.y() - screen->geometry().y(), 1, 1);

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
	QPalette palette = leftColorDisplay->palette() ;
	palette.setColor( QPalette::Window, color ) ;
	leftColorDisplay->setPalette( palette ) ;
	leftButton->setLabelColor( color ) ;

	hexcode->setText( color.name( QColor::HexRgb ) ) ;

	emit colorChanged( color ) ;
}

void ColorPicker::updateColorFromHex() {
	QString hex = hexcode->text().trimmed() ;
	if( !hex.startsWith( "#" ) )
		hex.prepend( "#" ) ;
	QColor color( hex ) ;
	if( color.isValid()) {
		setSliders( color ) ;
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

void ColorPicker::setSliders( const QColor& color )
{
	if( red && green && blue ) {
		red->setValue( color.red() ) ;
		green->setValue( color.green() ) ;
		blue->setValue( color.blue() ) ;
	}
}

ColorButton::ColorButton( QWidget* parent, bool selected, const QColor& defaultColor ) : QPushButton(parent), currentColor(defaultColor), selected( selected )
{
	innerColor = new QLabel( this ) ;
	innerColor->setBackgroundRole( QPalette::Window ) ;
	innerColor->setAlignment( Qt::AlignCenter ) ;
	innerColor->setAutoFillBackground( true ) ;
	setMinimumSize( 50, 50 ) ;
	setMaximumWidth( 300 ) ;
	

	QBoxLayout* layout = new QVBoxLayout( this ) ;
	layout->setContentsMargins( 3, 3, 3, 3 ) ;
	layout->addWidget( innerColor ) ;

	setLayout( layout ) ;
	setAutoFillBackground( true ) ;
	setButtonColor( Qt::black ) ;
	setLabelColor( defaultColor ) ;
}

void ColorButton::setButtonColor( const QColor& color )
{
	if( !selected ) return ;

	borderColor = color ;
	QPalette pal = palette() ;
	pal.setColor( QPalette::Button, color ) ;
	setPalette( pal ) ;
	update() ;
}

void ColorButton::setLabelColor( const QColor &color )
{
	qDebug() << "Label Color: " << color.name() ;
	currentColor = color ;
	QPalette palette = innerColor->palette() ;
	palette.setColor( QPalette::Window, color ) ;
	innerColor->setPalette( palette ) ;
	update() ;
}
