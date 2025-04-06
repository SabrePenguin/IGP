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
#include <QSignalBlocker>
#include <QTabBar>


ColorPicker::ColorPicker( QWidget* parent )
{
	QHBoxLayout* mainLayout = new QHBoxLayout( this ) ;
	pickingColor = false ;

	toggleButton = new QPushButton( "<<", this ) ;
	toggleButton->setFixedWidth( 20 ) ;
	toggleButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding ) ;
	mainLayout->addWidget( toggleButton ) ;

	/////////////////////////////
	colorPickerWidget = new QWidget( this ) ;
	QBoxLayout* pickerLayout = new QVBoxLayout( colorPickerWidget ) ;

	tab = new QTabWidget( this );
	//tab->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
	mainLayout->addWidget( tab );

	left_tab = new ColorTab();
	tab->addTab( left_tab, "" );
	tab->tabBar()->setTabButton( 0, QTabBar::LeftSide, left_tab->getLabel() );

	right_tab = new ColorTab(nullptr, Qt::white);
	tab->addTab( right_tab, "" );
	tab->tabBar()->setTabButton( 1, QTabBar::LeftSide, right_tab->getLabel() );

	
	connect( tab->tabBar(), &QTabBar::currentChanged, this, &ColorPicker::swappedButton ) ;  //Update the active tab for eyedropper
	connect( right_tab, &ColorTab::colorChanged, this, &ColorPicker::colorChanged) ;
	connect( left_tab, &ColorTab::colorChanged, this, &ColorPicker::colorChanged) ;

	pickerLayout->addWidget( tab );

	/////////////////////////////

	
	pickerLayout->setContentsMargins( 0, 0, 0, 0 ) ;
	colorPickerWidget->setVisible( false ) ;
	colorPickerWidget->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding ) ;
	mainLayout->addWidget( colorPickerWidget ) ;

	QBoxLayout* labelLayout = new QHBoxLayout() ;

	// Create the eyedropper
	eyedropper = new QPushButton( "Pick Color", this ) ;
	pickerLayout->addWidget( eyedropper ) ;

	connect( toggleButton, &QPushButton::clicked, this, &ColorPicker::togglePicker ) ;
	connect( eyedropper, &QPushButton::clicked, this, &ColorPicker::activateEyedropper ) ;
	qApp->installEventFilter( this ) ;
}


void ColorPicker::overrideSelectedColor( QColor color ) {
	//setColor( color ) ;
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
			int selected_tab = tab->currentIndex() ;
			ColorTab* active ;
			if( selected_tab == 0 ) {
				active = left_tab ;
			}
			else if( selected_tab == 1 ) {
				active = right_tab ;
			}
			else {
				return false ;
			}
			active->overrideColor( pickedColor ) ;
		}
		return true ;
	}

	return QWidget::eventFilter( obj, event ) ;
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


ColorTab::ColorTab( QWidget* parent, const QColor& defaultColor ) : QTabWidget(parent)
{
	QBoxLayout* mainLayout = new QVBoxLayout( this );
	QHBoxLayout* sliderLayout = new QHBoxLayout() ;

	red = new ColorSlider( this ) ;
	green = new ColorSlider( this ) ;
	blue = new ColorSlider( this ) ;

	sliderLayout->addWidget( red ) ;
	sliderLayout->addWidget( green ) ;
	sliderLayout->addWidget( blue ) ;

	connect( red, &ColorSlider::valueChanged, this, &ColorTab::updateColorFromSlider );
	connect( green, &ColorSlider::valueChanged, this, &ColorTab::updateColorFromSlider );
	connect( blue, &ColorSlider::valueChanged, this, &ColorTab::updateColorFromSlider );

	mainLayout->addLayout( sliderLayout );
	current_color = new QLabel( this ) ;
	current_color->setBackgroundRole( QPalette::Window ) ;
	current_color->setAlignment( Qt::AlignCenter ) ;
	current_color->setAutoFillBackground( true ) ;
	current_color->setMinimumSize( 45, 45 );
	

	hexcode = new QLineEdit( this ) ;
	hexcode->setAlignment( Qt::AlignCenter ) ;
	hexcode->setPlaceholderText( "#RRGGBB" ) ;
	hexcode->setMaxLength( 7 );
	QRegularExpression hexregex( "^#?([A-Fa-f0-8]{6})$" ) ;
	hexcode->setValidator( new QRegularExpressionValidator( hexregex, this ) ) ;

	mainLayout->addWidget( hexcode );

	setColor( defaultColor );
}

QLabel* ColorTab::getLabel()
{
	return current_color;
}

void ColorTab::overrideColor( const QColor& color )
{
	setColor( color ) ;
}


void ColorTab::updateColorFromSlider()
{
	QColor color(
		red->value(),
		green->value(),
		blue->value()
	) ;
	setColor( color ) ;
}

void ColorTab::updateColorFromHex()
{
	QString hex = hexcode->text().trimmed() ;
	if( !hex.startsWith( "#" ) )
		hex.prepend( "#" ) ;
	QColor color( hex ) ;
	if( color.isValid() ) {
		setColor( color ) ;
	}
}

void ColorTab::setColor( const QColor& new_color, bool emitter )
{
	if( emitter ) {
		red->setValue( new_color.red() ) ;
		green->setValue( new_color.green() ) ;
		blue->setValue( new_color.blue() ) ;
	}
	else {
		const QSignalBlocker blockr( red ) ;
		const QSignalBlocker blockg( green ) ;
		const QSignalBlocker blockb( blue ) ;
		red->setValue( new_color.red() ) ;
		green->setValue( new_color.green() ) ;
		blue->setValue( new_color.blue() ) ;
	}
	
	QPalette palette = current_color->palette() ;
	palette.setColor( QPalette::Window, new_color ) ;
	current_color->setPalette( palette ) ;
	update() ;
	red->setValue( new_color.red() ) ;
	green->setValue( new_color.green() ) ;
	blue->setValue( new_color.blue() ) ;
	hexcode->setText( new_color.name( QColor::HexRgb ) ) ;

	if( emitter )
	{
		emit colorChanged( new_color ) ;
	}
}
