#ifndef _SPORTZONE_H
#define _SPORTZONE_H


class SportZone {
  private:
	unsigned int zone_number;

	unsigned int low_percent;
	unsigned int high_percent;

	double on_zone_seconds;

  public:

	void setZoneNumber(unsigned int);
	unsigned int getZoneNumber(void);

	void setLowPercent(unsigned int);
	unsigned int getLowPercent(void);

	void setHighPercent(unsigned int);
	unsigned int getHighPercent(void);

	void setOnZoneSeconds(double);
	double getOnZoneSeconds(void);

	static int number_of_sportzones;
};
#endif
