#include "colorwell.h"

ColorWell::ColorWell(QWidget *parent, QString newCaption):
	QFrame(parent)
{
	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Sunken);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_caption = newCaption;
}
