#include "interval.h"

Interval::Interval() : min(0.0), max(0.0) {}

Interval::Interval(double min, double max) : min(min), max(max) {}

Interval::Interval(const Interval& interval1, const Interval& interval2) {
	if (interval1.min < interval2.min) {
		min = interval1.min;
	} else {
		min = interval2.min;
	}

	if (interval1.max > interval2.max) {
		max = interval1.max;
	} else {
		max = interval2.max;
	}
}

Interval::Interval(const Interval& range) {
	min = range.min;
	max = range.max;
}

bool Interval::contains_exclusive(double a)
{
	return (min < a && a < max);
}

bool Interval::contains_inclusive(double a)
{
	return (min <= a && a <= max);
}

bool Interval::overlaps(const Interval& range)
{
	if (min > range.max || range.min > max) {
		return false;
	};
	return true;
}

double Interval::size() const
{
	return max - min;
}

void Interval::expand(double delta)
{
	min -= delta * 0.5;
	max += delta * 0.5;
}
