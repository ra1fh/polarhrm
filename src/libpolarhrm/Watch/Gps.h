/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * polarhrm
 * Copyright (C) Thomas Foyth 2011 <thomas.foyth@gmail.com>
 * 
polarhrm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * polarhrm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GPS_H_
#define _GPS_H_

class Gps {

  private:
	double latitude;
	double longitude;

  public:
	Gps(void);
	Gps(double, double);

	void setLatitude(double);
	double getLatitude(void);

	void setLongitude(double);
	double getLongitude(void);

  protected:


};

#endif // _GPS_H_
