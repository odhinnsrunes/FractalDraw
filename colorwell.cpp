#include "colorwell.h"

ColorWell::ColorWell(QWidget *parent) :
	QFrame(parent)
{
	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Sunken);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}
