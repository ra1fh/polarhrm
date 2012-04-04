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

/* Save the data stored in the session structure into a pdd file */
void PddFile::save(void) {

    FILE *fd; 


	fd = fopen(filename.c_str(), "w");
	printf("path %s\n\n",filename.c_str());
	if (0 == fd) {
		printf("error opening file for write\n");
	}

	/* [DayInfo] */
	fprintf(fd,"[DayInfo]\r\n");
	/* row 0 */
	fprintf(fd,"%d\r%d\r%d\r%d\r%d\r%d\r\n",
	        this->FileVersion,
	        this->Nbr_Of_Info_Rows,
	        this->Nbr_Of_Num_Rows,
	        this->Nbr_Of_Num_Columns,
	        this->Nbr_Of_Text_Rows,
	        this->Max_Char_Per_Text_Row);
	/* row 1 */
	fprintf(fd,"%s\r%d\r%d\r%d\r%d\r%d\r\n",
	        this->SessionDate.c_str(),
	        this->Nbr_of_exercises,
	        this->Resting_HR,
	        this->Orthostatic_test_HR,
	        this->Weight,
	        this->Sleeping_hours);
	/* row 2 */
	fprintf(fd,"%d\r%d\r%d\r%d\r%d\r%d\r\n",
	        this->Sleeping_pattern,
	        0,
	        0,
	        0,
	        0,
	        0);
	/* row 3 */
	fprintf(fd,"%d\r%d\r%d\r%d\r%d\r%d\r\n",
	        this->DayInfoData,
	        0, //- Reserved -
	        this->Polar_HRmaxp,
	        this->Overtraining_test_result,
	        this->User_defined_item1,
	        this->User_defined_item2);
	/* row 4 */
	fprintf(fd,"%d\r%d\r%d\r%d\r%d\r%d\r\n",
	        this->User_defined_item3,
	        0,  //- Reserved - 
	        this->Polar_OwnIndex,
	        this->Weather,
	        this->Temperature,
	        0), //- Reserved - 
	/* row 5 */
	fprintf(fd,"%d\r%d\r%d\r%d\r%d\r%d\r\n",
	        0, //- Reserved - 
	        0, //- Reserved - 
	        this->Nbr_of_exercise_plans,
	        0, //- Reserved - 
	        0, //- Reserved - 
	        0);//- Reserved - 
	//Number rows 6 – 7 are reserved, filled with zeros
	fprintf(fd,"0\r0\r0\r0\r0\r0\r\n"); // row 6
	fprintf(fd,"0\r0\r0\r0\r0\r0\r\n"); // row 7

	/* text rows */
	/* row 0 */
	fprintf(fd,"%s\r\n",
	        this->Day_note.c_str());

	/* add empty line */
	fprintf(fd,"\r\n"); 



	/* [ExerciseInfoX] */
	fprintf(fd,"[ExerciseInfo%s]\r\n",
	        this->Day_note.c_str());





	fclose(fd);
}


void PddFile::setPath(std::string path) {
	this->filename = path;
}
