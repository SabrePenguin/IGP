#include "ColorSlider.h"
#include <QLayout>

ColorSlider::ColorSlider( QWidget* parent ) : QWidget( parent ) {
	QVBoxLayout* layout = new QVBoxLayout( this ) ;

	lineEdit = new QLineEdit( this ) ;
	slider = new QSlider( Qt::Vertical, this ) ;
	slider->setRange( 0, 255 ) ;
}