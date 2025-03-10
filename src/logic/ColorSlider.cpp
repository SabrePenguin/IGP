#include "ColorSlider.h"
#include <QLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

ColorSlider::ColorSlider( QWidget* parent ) : QWidget( parent ) {
	QVBoxLayout* layout = new QVBoxLayout( this ) ;

	lineEdit = new QLineEdit( this ) ;
	slider = new QSlider( Qt::Vertical, this ) ;
	slider->setRange( 0, 255 ) ;
	lineEdit->setPlaceholderText( "0" );
	QRegularExpression regex( "^([0-9]{1,3}|0x[0-9A-Fa-f]{1,2})$" ) ;

	lineEdit->setValidator( new QRegularExpressionValidator( regex, this ) ) ;

	layout->addWidget( lineEdit ) ;
	layout->addWidget( slider ) ;

	connect( slider, &QSlider::valueChanged, this, &ColorSlider::onSliderChanged ) ;
	connect( lineEdit, &QLineEdit::editingFinished, this, &ColorSlider::onTextChanged ) ;
}

int ColorSlider::value() const
{
	return slider->value();
}

void ColorSlider::setValue( int val ) {
	if ( val < 0 || val > 255 ) 
		return ;
	slider->setValue( val ) ;
	lineEdit->setText( QString::number( val ) ) ;
}

void ColorSlider::onTextChanged() {
	QString text = lineEdit->text().trimmed() ;
	int value = 0 ;
	if( text.startsWith( "0x", Qt::CaseInsensitive ) ) {
		bool ok ;
		value = text.toInt( &ok, 16 ) ;
		if( !ok ) return ;
	}
	else {
		bool ok ;
		value = text.toInt( &ok, 10 ) ;
		if( !ok || value < 0 || value > 255 ) return ;
	}
	slider->setValue( value ) ;
	emit valueChanged( value ) ;
}

void ColorSlider::onSliderChanged( int value ) {
	lineEdit->setText( QString::number( value ) ) ;
	emit valueChanged( value ) ;
}