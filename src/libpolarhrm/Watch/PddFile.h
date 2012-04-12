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

#include <iostream>
#include <list>
#include <string>
#include "libpolarhrm/Watch/ExercisePddFile.h"

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


	PddFile(void);
	void setPath(std::string);

	void addExercise(ExercisePddFile);

	void savePdd(void);

private:

	std::string filename;
	std::list<ExercisePddFile> exeList;

};

#endif // _PDDFILE_H_
