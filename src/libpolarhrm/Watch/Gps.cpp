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

#include "Gps.h"

Gps::Gps(void){

}


Gps::Gps(double lat, double lon){
	this->latitude = lat;
	this->longitude = lon;
}


void Gps::setLatitude(double lat){
	this->latitude = lat;
}


double Gps::getLatitude(void){
	return this->latitude;
}


void Gps::setLongitude(double lon){
	this->longitude = lon;
}


double Gps::getLongitude(void){
	return this->longitude;
}
