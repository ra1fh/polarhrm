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

#ifndef _PDDFILE_H_
#define _PDDFILE_H_

//#include <iostream>
#include <string>

class PddFile
{
public:

	/* [DayInfo] */
	//Row 0 
	static const int FileVersion = 100;
	static const int Nbr_Of_Info_Rows = 1;
	static const int Nbr_Of_Num_Rows = 7;
	static const int Nbr_Of_Num_Columns = 6;
	static const int Nbr_Of_Text_Rows = 1;
	static const int Max_Char_Per_Text_Row = 512;

	//Row 1:
	std::string SessionDate; //yyyymmdd
	int Nbr_of_exercises;
	int Resting_HR; //bpm
	int Orthostatic_test_HR; //bpm
	int Weight;
	int Sleeping_hours;

	//Row 2: 
	int Sleeping_pattern; // 0 = excellent ... 4 = Insomnia
	// rest is filled with 0 

	//Row 3: 
	int DayInfoData; 
	//- Reserved - 
	int Polar_HRmaxp;
	int Overtraining_test_result;
	int User_defined_item1;
	int User_defined_item2;

	//Row 4: 
	int User_defined_item3;
	//- Reserved - 
	int Polar_OwnIndex;
	int Weather;
	int Temperature;
	//- Reserved - 

	//Row 5: 
	//- Reserved - 
	//- Reserved - 
	int Nbr_of_exercise_plans;
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 

	//Number rows 6 – 7 are reserved, filled with zeros

	//Text rows
	//Row 0
	std::string Day_note;



	/* [ExerciseInfo1] */

	// Row 0
	int FileVersion_ExerciseInfo;
	int Nbr_Of_Info_Rows_ExerciseInfo;
	int Nbr_Of_Num_Rows_ExerciseInfo;
	int Nbr_Of_Num_Columns_ExerciseInfo;
	int Nbr_Of_Text_Rows_ExerciseInfo;
	int Max_Char_Per_Text_Row_ExerciseInfo;

	// Row 1
	//	- Reserved - 
	int No_report;
	int Not_edited_manually;
	int Distance_from_product;
	int Start_time;
	int Total_time;

	// Row 2
	int Sport;
	int Distance_OLD;
	int Feeling;
	int Recovery;
	//- Reserved -
	int Energy_consumption;

	// Row 3
	int Distance;
	//- Reserved -
	//- Reserved -
	//- Reserved -
	int Odometer;
	int Ascent;

	// Row 4
	int Total_exertion;
	int Power_avg_with_zero_values;
	int Vert_speed_up_max;
	int Vert_speed_down_max;
	//- Reserved - 
	int Vert_speed_up_avg;

	// Row 5
	int Zone_0_time;
	int Zone_1_time;
	int Zone_2_time;
	int Zone_3_time;
	int Zone_4_time;
	int Zone_5_time;

	// Row 6
	int Zone_6_time;
	int Zone_7_time;
	int Zone_8_time;
	int Zone_9_time;
	int Sport_specific_unit;
	//- Reserved - 

	// Row 7
	int Zone_0_exertion;
	int Zone_1_exertion;
	int Zone_2_exertion;
	int Zone_3_exertion;
	int Zone_4_exertion;
	int Zone_5_exertion;

	// Row 8
	int Zone_6_exertion;
	int Zone_7_exertion;
	int Zone_8_exertion;
	int Zone_9_exertion;
	int Recording_rate;
	int Original_ascent;

	// Row 9
	int HR_Average;
	int HR_Max;
	int Speed_Average;
	int Speed_Max;
	int Cadence_Average;
	int Cadence_Max;

	// Row 10
	int Altitude_Average;
	int Altitude_Max;
	int Power_Average;
	int Power_Max;
	int Pedalling_Index_Average;
	int Pedalling_Index_Max;

	//Row 11
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	int Slope_count;
	int Descent;

	//Row 12
	int Average_calory_rate;
	int Vert_speed_down_avg;
	int Beat_sum;
	int L_R_Balance_Average;
	int L_R_Balance_Max;
	int Original_energy_cons;

	//Row 13
	int Power_zone_0_time;
	int Power_zone_1_time;
	int Power_zone_2_time;
	int Power_zone_3_time;
	int Power_zone_4_time;
	int Power_zone_5_time;

	//Row 14
	int Power_zone_6_time;
	int Power_zone_7_time;
	int Power_zone_8_time;
	int Power_zone_9_time;
	//- Reserved - 
	//- Reserved - 

	//Row 15
	int Ascent_hour_V_A_M;
	int Exercise_ranking;
	int MemFull;
	int Running_Index;
	//- Reserved - 
	int Incline_max;

	// Row 16
	int Stride_length_average;
	int Decline_max;
	int Cycling_efficiency;
	int Footpod_calibr_factor;
	int Wheel_size;
	//- Reserved - 

	// Row 17
	int Exercise_type;
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 

	// Number rows 18 – 24 are reserved, filled with zeros

	// Textrows
	std::string exercise_name;
	std::string exercise_note_text;
	std::string attached_hrm_file; // (if in same folder, no folder info with file name)
	std::string hyperlink;
	std::string hyperlink_info_text;
	std::string attached_location_file;
	std::string attached_RR_file;
	std::string previous_multisport_file;
	std::string next_multisport_file;


	void setPath(std::string);
	void save(void);

private:

	std::string filename;

};

#endif // _PDDFILE_H_
