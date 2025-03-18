#pragma once

class Interval
{
public:
	Interval();
	Interval(double min, double max);
	Interval(const Interval& interval1, const Interval& interval2);
	Interval(const Interval& range);

	bool contains_exclusive(double a);
	bool contains_inclusive(double a);

	bool overlaps(const Interval& range);

	double size() const;
	void expand(double delta);

	double min;
	double max;
};