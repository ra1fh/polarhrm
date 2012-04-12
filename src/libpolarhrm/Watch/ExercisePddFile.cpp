/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * polarhrm
 * Copyright (C) Thomas Foyth 2012 <thomas.foyth@gmail.com>
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

#include "ExercisePddFile.h"

#include "wDate.h"

	ExercisePddFile::ExercisePddFile(void){

	// Row 1
	//	- Reserved - 
	No_report=0;
	Not_edited_manually=0;
	Distance_from_product=0;
	Start_time=0;
	Total_time=0;

	// Row 2
	Sport=1; 
	Distance_OLD=0;
	Feeling=0;
	Recovery=0;
	//- Reserved -
	Energy_consumption=0;

	// Row 3
	Distance=0;
	//- Reserved -
	//- Reserved -
	//- Reserved -
	Odometer=0;
	Ascent=0;

	// Row 4
	Total_exertion=0;
	Power_avg_with_zero_values=0;
	Vert_speed_up_max=0;
	Vert_speed_down_max=0;
	//- Reserved - 
	Vert_speed_up_avg=0;

	// Row 5
	Zone_0_time=0;
	Zone_1_time=0;
	Zone_2_time=0;
	Zone_3_time=0;
	Zone_4_time=0;
	Zone_5_time=0;

	// Row 6
	Zone_6_time=0;
	Zone_7_time=0;
	Zone_8_time=0;
	Zone_9_time=0;
	Sport_specific_unit=0;
	//- Reserved - 

	// Row 7
	Zone_0_exertion=0;
	Zone_1_exertion=0;
	Zone_2_exertion=0;
	Zone_3_exertion=0;
	Zone_4_exertion=0;
	Zone_5_exertion=0;

	// Row 8
	Zone_6_exertion=0;
	Zone_7_exertion=0;
	Zone_8_exertion=0;
	Zone_9_exertion=0;
	Recording_rate=0;
	Original_ascent=0;

	// Row 9
	HR_Average=0;
	HR_Max=0;
	Speed_Average=0;
	Speed_Max=0;
	Cadence_Average=0;
	Cadence_Max=0;

	// Row 10
	Altitude_Average=0;
	Altitude_Max=0;
	Power_Average=0;
	Power_Max=0;
	Pedalling_Index_Average=0;
	Pedalling_Index_Max=0;

	//Row 11
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	Slope_count=0;
	Descent=0;

	//Row 12
	Average_calory_rate=0;
	Vert_speed_down_avg=0;
	Beat_sum=0;
	L_R_Balance_Average=0;
	L_R_Balance_Max=0;
	Original_energy_cons=0;

	//Row 13
	Power_zone_0_time=0;
	Power_zone_1_time=0;
	Power_zone_2_time=0;
	Power_zone_3_time=0;
	Power_zone_4_time=0;
	Power_zone_5_time=0;

	//Row 14
	Power_zone_6_time=0;
	Power_zone_7_time=0;
	Power_zone_8_time=0;
	Power_zone_9_time=0;
	//- Reserved - 
	//- Reserved - 

	//Row 15
	Ascent_hour_V_A_M=0;
	Exercise_ranking=0;
	MemFull=0;
	Running_Index=0;
	//- Reserved - 
	Incline_max=0;

	// Row 16
	Stride_length_average=0;
	Decline_max=0;
	Cycling_efficiency=0;
	Footpod_calibr_factor=0;
	Wheel_size=0;
	//- Reserved - 

	// Row 17
	Exercise_type=0;
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 

	// Number rows 18 – 24 are reserved, filled with zeros


	exercise_name.append("Basic Use");

	}

void ExercisePddFile::setSession(Session *session){


	wDate* startdate;
	startdate = session->getStartDate();
	wTime* starttime;
	starttime = startdate->getTime();

	// Row 1
	this->Distance_from_product = session->distance;
	this->Start_time = (int)starttime->toDouble();
	this->Total_time = session->getDuration()->toDouble();

	// Row 2
	this->Energy_consumption = session->calories;

	// Row 3
	this->Distance = session->distance;
	Odometer=0;
	Ascent=0; //FIXME where can get this?

	// Row 4
	Total_exertion=0;
	Power_avg_with_zero_values=0;
	Vert_speed_up_max=0;
	Vert_speed_down_max=0;
	Vert_speed_up_avg=0;

	// Row 5
	Zone_0_time=session->sportzones[0]->getOnZoneSeconds();
	Zone_1_time=session->sportzones[1]->getOnZoneSeconds();
	Zone_2_time=session->sportzones[2]->getOnZoneSeconds();
	Zone_3_time=session->sportzones[3]->getOnZoneSeconds();
	Zone_4_time=session->sportzones[4]->getOnZoneSeconds();
	Zone_5_time=0;

	// Row 6
	Zone_6_time=0;
	Zone_7_time=0;
	Zone_8_time=0;
	Zone_9_time=0;
	Sport_specific_unit=0;
	//- Reserved - 

	// Row 7
	Zone_0_exertion=0;
	Zone_1_exertion=0;
	Zone_2_exertion=0;
	Zone_3_exertion=0;
	Zone_4_exertion=0;
	Zone_5_exertion=0;

	// Row 8
	Zone_6_exertion=0;
	Zone_7_exertion=0;
	Zone_8_exertion=0;
	Zone_9_exertion=0;
	this->Recording_rate = session->recording_interval;
	Original_ascent=0;

	// Row 9
	this->HR_Average = session->hr_avg;
	this->HR_Max = session->hr_max;
	this->Speed_Average = session->pace_avg;
	this->Speed_Max = session->pace_max;
	Cadence_Average=0;
	Cadence_Max=0;

	// Row 10
	Altitude_Average = 0;
	Altitude_Max=0;
	Power_Average=0;
	Power_Max=0;
	Pedalling_Index_Average=0;
	Pedalling_Index_Max=0;

	//Row 12
	Average_calory_rate=0;
	Vert_speed_down_avg=0;
	this->Beat_sum = session->getNumberOfSamples();
	L_R_Balance_Average=0;
	L_R_Balance_Max=0;
	Original_energy_cons=0;

	//Row 13
	Power_zone_0_time=0;
	Power_zone_1_time=0;
	Power_zone_2_time=0;
	Power_zone_3_time=0;
	Power_zone_4_time=0;
	Power_zone_5_time=0;

	//Row 14
	Power_zone_6_time=0;
	Power_zone_7_time=0;
	Power_zone_8_time=0;
	Power_zone_9_time=0;
	//- Reserved - 
	//- Reserved - 

	//Row 15
	Ascent_hour_V_A_M=0;
	Exercise_ranking=0;
	MemFull=0;
	Running_Index=0;
	//- Reserved - 
	Incline_max=0;

	// Row 16
	Stride_length_average=0;
	Decline_max=0;
	Cycling_efficiency=0;
	Footpod_calibr_factor=0;
	Wheel_size=0;
	//- Reserved - 

	// Row 17
	Exercise_type=0;




}

