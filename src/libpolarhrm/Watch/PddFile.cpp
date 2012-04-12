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

#include "PddFile.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <list>


PddFile::PddFile(void){

	//Row 1:
	//std::string SessionDate; //yyyymmdd
	Nbr_of_exercises=0;
	Resting_HR=0; //bpm
	Orthostatic_test_HR=0; //bpm
	Weight=0;
	Sleeping_hours=0;

	//Row 2: 
	Sleeping_pattern=0; // 0 = excellent ... 4 = Insomnia
	// rest is filled with 0 

	//Row 3: 
	DayInfoData=0; 
	//- Reserved - 
	Polar_HRmaxp=0;
	Overtraining_test_result=0;
	User_defined_item1=0;
	User_defined_item2=0;

	//Row 4: 
	User_defined_item3=0;
	//- Reserved - 
	Polar_OwnIndex=0;
	Weather=0;
	Temperature=0;
	//- Reserved - 

	//Row 5: 
	//- Reserved - 
	//- Reserved - 
	Nbr_of_exercise_plans=0;
	//- Reserved - 
	//- Reserved - 
	//- Reserved - 



}



/* Save the data stored in the session structure into a pdd file */
void PddFile::savePdd(void) {

    FILE *fd; 


	fd = fopen(filename.c_str(), "w");
	printf("path %s\n\n",filename.c_str());
	if (0 == fd) {
		printf("error opening file for write\n");
	}

	/* [DayInfo] */
	fprintf(fd,"[DayInfo]\r\n");
	/* row 0 */
	fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
	        this->FileVersion,
	        this->Nbr_Of_Info_Rows,
	        this->Nbr_Of_Num_Rows,
	        this->Nbr_Of_Num_Columns,
	        this->Nbr_Of_Text_Rows,
	        this->Max_Char_Per_Text_Row);
	/* row 1 */
	fprintf(fd,"%s\t%d\t%d\t%d\t%d\t%d\r\n",
	        this->SessionDate.c_str(),
	        this->Nbr_of_exercises,
	        this->Resting_HR,
	        this->Orthostatic_test_HR,
	        this->Weight,
	        this->Sleeping_hours);
	/* row 2 */
	fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
	        this->Sleeping_pattern,
	        0,
	        0,
	        0,
	        0,
	        0);
	/* row 3 */
	fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
	        this->DayInfoData,
	        0, //- Reserved -
	        this->Polar_HRmaxp,
	        this->Overtraining_test_result,
	        this->User_defined_item1,
	        this->User_defined_item2);
	/* row 4 */
	fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
	        this->User_defined_item3,
	        0,  //- Reserved - 
	        this->Polar_OwnIndex,
	        this->Weather,
	        this->Temperature,
	        0), //- Reserved - 
	/* row 5 */
	fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
	        0, //- Reserved - 
	        0, //- Reserved - 
	        this->Nbr_of_exercise_plans,
	        0, //- Reserved - 
	        0, //- Reserved - 
	        0);//- Reserved - 
	//Number rows 6 – 7 are reserved, filled with zeros
	fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 6
	fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 7

	/* text rows */
	/* row 0 */
	fprintf(fd,"%s\r\n",
	        this->Day_note.c_str());

	/* add empty line */
	fprintf(fd,"\r\n"); 


	std::list<ExercisePddFile>::iterator current;
	int elementcounter=1;

	for (current=exeList.begin(); current != exeList.end(); current++ ) {

		/* [ExerciseInfoX] */
		fprintf(fd,"[ExerciseInfo%d]\r\n",
			    elementcounter);
		// Row 0
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->FileVersion,
		        current->Nbr_Of_Info_Rows,
		        current->Nbr_Of_Num_Rows,
		        current->Nbr_Of_Num_Columns,
		        current->Nbr_Of_Text_Rows,
		        current->Max_Char_Per_Text_Row);
		// Row 1
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        0,
		        current->No_report,
		        current->Not_edited_manually,
		        current->Distance_from_product,
		        current->Start_time,
		        current->Total_time);
		// Row 2
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Sport,
		        current->Distance_OLD,
		        current->Feeling,
		        current->Recovery,
		        0,//- Reserved -
		        current->Energy_consumption);
		// Row 3
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Distance,
		        0,//- Reserved -
		        0,//- Reserved -
		        0,//- Reserved -
		        current->Odometer,
		        current->Ascent);
		// Row 4
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Total_exertion,
		        current->Power_avg_with_zero_values,
		        current->Vert_speed_up_max,
		        current->Vert_speed_down_max,
		        0,//- Reserved - 
		        current->Vert_speed_up_avg);
		// Row 5
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Zone_0_time,
		        current->Zone_1_time,
		        current->Zone_2_time,
		        current->Zone_3_time,
		        current->Zone_4_time,
		        current->Zone_5_time);
		// Row 6
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Zone_6_time,
		        current->Zone_7_time,
		        current->Zone_8_time,
		        current->Zone_9_time,
		        current->Sport_specific_unit,
		        0);//- Reserved - 
		// Row 7
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Zone_0_exertion,
		        current->Zone_1_exertion,
		        current->Zone_2_exertion,
		        current->Zone_3_exertion,
		        current->Zone_4_exertion,
		        current->Zone_5_exertion);
		// Row 8
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Zone_6_exertion,
		        current->Zone_7_exertion,
		        current->Zone_8_exertion,
		        current->Zone_9_exertion,
		        current->Recording_rate,
		        current->Original_ascent);
		// Row 9
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->HR_Average,
		        current->HR_Max,
		        current->Speed_Average,
		        current->Speed_Max,
		        current->Cadence_Average,
		        current->Cadence_Max);
		// Row 10
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Altitude_Average,
		        current->Altitude_Max,
		        current->Power_Average,
		        current->Power_Max,
		        current->Pedalling_Index_Average,
		        current->Pedalling_Index_Max);
		//Row 11
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        0,//- Reserved - 
		        0,//- Reserved - 
		        0,//- Reserved - 
		        0,//- Reserved - 
		        current->Slope_count,
		        current->Descent);
		//Row 12
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Average_calory_rate,
		        current->Vert_speed_down_avg,
		        current->Beat_sum,
		        current->L_R_Balance_Average,
		        current->L_R_Balance_Max,
		        current->Original_energy_cons);
		//Row 13
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Power_zone_0_time,
		        current->Power_zone_1_time,
		        current->Power_zone_2_time,
		        current->Power_zone_3_time,
		        current->Power_zone_4_time,
		        current->Power_zone_5_time);
		//Row 14
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Power_zone_6_time,
		        current->Power_zone_7_time,
		        current->Power_zone_8_time,
		        current->Power_zone_9_time,
		        0,//- Reserved - 
		        0);//- Reserved - 
		//Row 15
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Ascent_hour_V_A_M,
		        current->Exercise_ranking,
		        current->MemFull,
		        current->Running_Index,
		        0,//- Reserved - 
		        current->Incline_max);
		// Row 16
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Stride_length_average,
		        current->Decline_max,
		        current->Cycling_efficiency,
		        current->Footpod_calibr_factor,
		        current->Wheel_size,
		        0);//- Reserved - 
		// Row 17
		fprintf(fd,"%d\t%d\t%d\t%d\t%d\t%d\r\n",
		        current->Exercise_type,
		        0,//- Reserved - 
		        0,//- Reserved - 
		        0,//- Reserved - 
		        0,//- Reserved - 
		        0);//- Reserved - 
		// Number rows 18 – 24 are reserved, filled with zeros
		fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 18
		fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 19
		fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 20
		fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 21
		fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 22
		fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 23
		fprintf(fd,"0\t0\t0\t0\t0\t0\r\n"); // row 24
		// Textrows
		fprintf(fd,"%s\r\n",
		        current->exercise_name.c_str());
		fprintf(fd,"%s\r\n",
		        current->exercise_note_text.c_str());
		fprintf(fd,"%s\r\n",
		        current->attached_hrm_file.c_str()); // (if in same folder, no folder info with file name)
		fprintf(fd,"%s\r\n",
		        current->hyperlink.c_str());
		fprintf(fd,"%s\r\n",
		        current->hyperlink_info_text.c_str());
		fprintf(fd,"%s\r\n",
		        current->attached_location_file.c_str());
		fprintf(fd,"%s\r\n",
		        current->attached_RR_file.c_str());
		fprintf(fd,"%s\r\n",
		        current->previous_multisport_file.c_str());
		fprintf(fd,"%s\r\n",
		        current->next_multisport_file.c_str());


		elementcounter++;
	}




	fclose(fd);
}


void PddFile::setPath(std::string path) {
	this->filename = path;
}


void PddFile::addExercise(ExercisePddFile exe){

	exeList.push_back(exe);
	Nbr_of_exercises++;

}
