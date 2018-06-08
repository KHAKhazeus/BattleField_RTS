#include "MouseRect.h"

void MouseRect::update(float delta) {
	clear();
	drawRect(start, end, Color4F(0, 1, 0, 1));
}

void MouseRect::reset() {
	setVisible(false);
	if (isScheduled(schedule_selector(MouseRect::update)))
		unschedule(schedule_selector(MouseRect::update));
	start = end = Vec2::ZERO;
}