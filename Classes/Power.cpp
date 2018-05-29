#include "Power.h"
bool Power::init() {
	_power = 5000;
	return true;
}

bool Power::checkPower(int cost)const {
	if (_power >= cost) {
		return true;
	}
	return false;
}

void Power::spendPower(int cost) {
	_power -= cost;
}

int Power::getPower() {
	return _power;
}

void Power::increasePower(int amount) {
	_power += amount;
}