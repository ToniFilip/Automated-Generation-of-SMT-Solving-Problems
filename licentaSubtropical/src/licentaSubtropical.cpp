//============================================================================
// Name        : licentaSubtropical.cpp
// Author      : toni
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <setjmp.h>

using namespace std;

static const double alpha   = 1.0e-8;
int equal(double x, double y) { return fabs(x-y) < alpha; }
double values[8] = {-4,-3,-2,-1,1,2,3,4};
double values1[3] = { -1,0,1};
double values2[4] = {1,2,3,4};
int linearRows  = 0;
double linearTable[7][7] = {};
double positives[2] = {0,0};
double frames[10] = {};
int pos = 0;

void fillTableau(double table[][7], int rows, int col){
	int random = 0;
	for(int i = 0; i < rows+1; i++){
		table[i][0] = 0;
		random = (rand() % 3);
		table[i][1] = values1[random];
	}

	for(int i = 0; i < rows; i++){
		for(int j = 2; j < col+1; j++){
			random = (rand() % 4);
			table[i][j] = values2[random];
		}
	}

	for(int j = 2; j < col+1; j++){
		random = (rand() % 8);
		table[rows][j] = values[random];
	}
}

void viewTableau1(double table[][7], int rows, int col){
	  cout<<"  \\begin{table}[H]"<<endl;
	  cout<<"\\begin{tabular}  { |p{2cm}|p{2cm}||p{2cm}|p{2cm}|p{2cm}| }"<<endl;
	  cout<<" \\hline"<<endl;
	  cout<<" bound& & $p_x$& $p_y$ & $c$\\\\"<<endl;
	  cout<<" \\hline"<<endl;

	  for(int i=1; i<rows+1; i++) {
	    if(i == 1) cout<<" $0$ & $<$&";
	    if(i > 1) cout<<" $0$& $>$&";
	    for(int j=1; j<col+1; j++) {

	      cout<<" $";
	      if((int)table[i][j] == table[i][j])
	        printf("%6d", (int)table[i][j]);
	      else
	        printf("%6.2lf", table[i][j]);
	      cout<<"$";
	      if(j < col) cout<<"&";
	      }
	    cout<<"\\\\";
	    printf("\n");
	  }
	 cout<<" \\hline"<<endl;
	 cout<<"\\end{tabular}"<<endl;
	 cout<<"\\end{table}"<<endl;
}

double evalOne(double table[][7], int col){
	double value = 0;
	for(int j = 2; j < col+1; j++){
		value += table[3][j];
	}
	cout<<"\\begin{center}"<<endl;
	cout<<"$f(1,1) = "<<value<<endl;
	if(value > 0){
		for(int j = 2; j < col+1; j++){
			table[3][j] *= -1;
		}
	}
	cout<<".$";
	cout<<endl;
	cout<<"\\end{center}";
	cout<<endl;
	if(table[1][1] != 1){
		if(value > 0) cout<<"A solution is already $1$."<<endl;
	}
	if(table[1][1] == 1){
		if(value > 0) cout<<"Because the value of $f$ in point 1 is greater than zero, $-f$ will be considered."<<endl;
		cout<<endl;
	}
	return value;
}

void viewPolynom1(double table[][7], int rows, int col, bool greater){

	cout<<"The polynom and its bound are: "<<endl;
	cout<<"\\begin{center}"<<endl;
	cout<<"$"<<0;
	if(table[1][1] == -1) cout<<" < ";
	if(table[1][1] == 0) cout<<" \\leqslant ";
	if(table[1][1] == 1) cout<<" = ";
	for(int j = 2; j < col+1; j++){
			if(table[3][j] < 0){
				cout<<" - ";
				if(table[3][j] != 0 && table[3][j] != 1) cout<<-table[3][j];
			}
			if(table[3][j] == 0) continue;
			if(table[3][j] > 0){
				if( j > 2){
				cout<<" + ";
				if(table[3][j] != 1) cout<<table[3][j];
				cout<<" ";}
				else{
					if(table[3][j] != 1)
					cout<<table[3][j]<<" ";
				}
			}
			if(table[2][j] != 0){
				cout<<"x";
				if(table[2][j] != 1)
				cout<<"^"<<table[2][j];
			}
			if(table[1][j] != 0){
				cout<<"y";
				if(table[1][j] != 1)
				cout<<"^"<<table[1][j];
			}
	}
	cout<<"$";
	cout<<"\\end{center}"<<endl;
}

void viewPolynom(double table[][7], int rows, int col, bool greater){
	int temp = 0;
	int max = -100;
	cout<<"\\noindent"<<endl;
	if(greater == false){
	cout<<'\n';
	cout<<"The set of negative frames is: "<<endl;
	cout<<"\\begin{center}"<<endl;
	cout<<"$frames^{-}(f) = \\{";
	for(int j = 2; j < col+1; j++){
		if(table[3][j] < 0 && table[3][j] != 0){
			cout<<"("<<table[2][j]<<","<<table[1][j]<<")"<<" ";
			linearRows++;
				frames[temp] = table[2][j];
				temp++;
				frames[temp] = table[1][j];
				temp++;
		}
	}
	cout<<"\\}$";
	cout<<"\\end{center}"<<endl;
	cout<<'\n';
	cout<<"The set of positive frames is: "<<endl;
	cout<<"\\begin{center}"<<endl;
	cout<<"$frames^{+}(f) = \\{";
	for(int j = 2; j < col+1; j++){
		if(table[3][j] > 0 && table[3][j] != 0){
			cout<<"("<<table[2][j]<<","<<table[1][j]<<")"<<" ";
			if(max < table[2][j] + table[1][j]){
				positives[0] = table[2][j];
				positives[1] = table[1][j];
				max = table[2][j] + table[1][j];
				pos=temp;
			}

			linearRows++;
				frames[temp] = table[2][j];
				temp++;
				frames[temp] = table[1][j];
				temp++;

		}
	}
	}
	if(greater == true){
	cout<<'\n';
	cout<<"The set of positive frames is: "<<endl;
	cout<<"\\begin{center}"<<endl;
	cout<<"$frames^{+}(f) = \\{";
	for(int j = 2; j < col+1; j++){
		if(table[3][j] > 0 && table[3][j] != 0){
			cout<<"("<<table[2][j]<<","<<table[1][j]<<")"<<" ";

			if(max < table[2][j] + table[1][j]){
					positives[0] = table[2][j];
					positives[1] = table[1][j];
					max = table[2][j] + table[1][j];
					pos=temp;
				}
			linearRows++;
				frames[temp] = table[2][j];
				temp++;
				frames[temp] = table[1][j];
				temp++;
		}
	}

	cout<<"\\}$";
	cout<<"\\end{center}";
	cout<<'\n';
	cout<<"The set of negative frames is: "<<endl;
	cout<<"\\begin{center}"<<endl;
	cout<<"$frames^{-}(f) = \\{";
	for(int j = 2; j < col+1; j++){
		if(table[3][j] < 0 && table[3][j] != 0){
			cout<<"("<<table[2][j]<<","<<table[1][j]<<")"<<" ";

			linearRows++;
				frames[temp] = table[2][j];
				temp++;
				frames[temp] = table[1][j];
				temp++;

		}
	}
	}
	cout<<"\\}$";
	cout<<"\\end{center}";
	cout<<endl;
	cout<<'\n';
}

void linearProblem(double table[][7], int rows, bool greater){
	int temp = 0;
	int temp1 = 0;
	cout<<"The positive frame, for which a separating hyperplane is searched, is "<<"$("<<positives[0]<<","<<positives[1]<<"):$"<<endl;
	cout<<endl;
	for(int j = 1; j < 5; j++){
		linearTable[0][j] = 0;
		linearTable[1][j] = positives[temp];
		temp++;
	}
	for(int i = 1; i <= rows; i++){
		linearTable[i][0] = 0;
	}
	for(int i = 2; i <= rows; i++){
		for(int j = 1; j < 3; j++){
			if(pos == temp1) temp1 += 2;
			linearTable[i][j] = frames[temp1];
			temp1++;
		}

	}
	for(int i = 1; i <= rows; i++){
		linearTable[i][3] = 1;
	}
}

void triangular(double table[][7], int rows, int col){
	bool swapNeed = false;
	if(positives[0] == -10 && positives[1] == -10){
		cout <<"No positive frames"<<endl;
	}
	else{
	for(int i = 1; i < rows+1; i++){
		for(int j = i+1; j <= col+1; j++){
			double temp = table[j][i]/table[i][i];
			if(table[i][i] == 0) temp = 0;
			for(int k = 1; k < rows+1; k++){
				table[j][k] -= temp*table[i][k];
			}
		}
	}
	int counter = 0;
	int temp1 = 0;
	double temp = 0;
	for(int i = 1; i < rows+1; i++){
		for(int j = 1; j < col+1; j++){
			if(table[i][j] == 0 && i != rows) counter++;
		}
		if(counter == 3){
			temp1 = i;
			swapNeed = true;
		}
		counter = 0;
	}
	if(table[2][2] == 0 && table[3][2] != 0){
		for(int j = 1; j < col+1; j++){
			temp = table[2][j];
			table[2][j] = table[3][j];
			table[3][j] = temp;
		}
	}
	if(swapNeed == true){
		for(int j = 1; j < col+1; j++){
			temp = table[temp1+1][j];
			table[temp1+1][j] = table[temp1][j];
			table[temp1][j] = temp;
		}
	}

		for(int i = 4; i < rows+1; i++){
			for(int j = 1; j < col+1; j++){
				table[i][j] = 0;
			}
		}

	viewTableau1(table,rows,col);
	if(table[3][3] == 0){
		cout<<"The frame is not separable, because of the third row."<<endl;

	}else{
	cout<<"\\begin{equation}";
	cout<<table[3][3]<<"*c < 0 ";
	if(table[3][3] < 0) cout<<" \\Rightarrow c > 0";
	if(table[3][3] > 0) cout<<" \\Rightarrow c < 0";
	if(table[3][3] == 0) cout<<" \\Rightarrow c = 0";
	cout<<'\n';
	cout<<"\\end{equation}";

	if(table[2][2] != 0){
		cout<<"\\begin{equation}";
	cout<<table[2][2]<<"*p_y ";
	if(table[2][3] > 0) cout<<"+ "<<table[2][3]<<"*c"<<" < 0";
	if(table[2][3] < 0) cout<<"- "<<-table[2][3]<<"*c"<<" < 0";
	if(table[2][3] == 0) cout<< " < 0";
	if(table[2][2] < 0 && table[2][3] != 0) cout<<" \\Rightarrow p_y > "<<-table[2][3]/table[2][2]<<"*c";
	if(table[2][2] > 0 && table[2][3] != 0) cout<<" \\Rightarrow p_y < "<<-table[2][3]/table[2][2]<<"*c";
	if(table[2][3] == 0){
		if(table[2][2] < 0) cout<<" \\Rightarrow p_y > 0";
		if(table[2][2] > 0) cout<<" \\Rightarrow p_y < 0";
	}
	cout<<"\\end{equation}";
	cout<<'\n';
	}
	cout<<"\\begin{equation}";
	cout<<table[1][1]<<"*p_x + "<<table[1][2]<<"*p_y + "<<table[1][3]<<"*c > 0";
	cout<<" \\Rightarrow p_x > "<<-table[1][2]/table[1][1]<<"*p_y - "<<table[1][3]/table[1][1]<<"*c";
	cout<<"\\end{equation}";
	cout<<'\n';
	cout<<'\n';
	cout<<"The three inequalities constitute the solution set. Picking a value for $c$, followed by $p_y$ and then by $p_x$, will represent a hyperplane.";

} }
}
int main() {

	srand(time(NULL));
	bool greater = false;
	//int pivotrow, pivotcol;
	double table[7][7] = {};
	int rows = 3;
	int col = 2;
	cout << "Number of frames(at least 3): ";
	cin>>col;
	col++;
	cout<<"The txt file with the Latex code is 'latexOutput.txt' and is located in the folder of the project."<<endl;
	freopen("latexOutput.txt","w",stdout);
	cout<<"\\documentclass[a4paper,10pt, twoside, openright, notitlepage]{report}"<<endl;
	cout<<"\\usepackage{float}"<<endl;
	cout<<"\\usepackage{amsmath,latexsym,amsthm,amssymb,amsfonts,amscd,stmaryrd,textcomp}"<<endl;
	cout<<"\\usepackage[left=4.5cm, right=3.5cm]{geometry}"<<endl;
	cout<<"\\usepackage[absolute]{textpos}"<<endl;
	cout<<"\\usepackage{blindtext}"<<endl;
	cout<<"\\usepackage{helvet}"<<endl;
	cout<<"\\usepackage{color}"<<endl;
	cout<<"\\usepackage{amsthm, amssymb}"<<endl;
	cout<<"\\usepackage{multirow}"<<endl;
	cout<<"\\usepackage{graphicx}"<<endl;
	cout<<"\\usepackage{pst-intersect}"<<endl;
	cout<<"\\usepackage{tkz-euclide}"<<endl;
	cout<<"\\usepackage{pgfplots}"<<endl;
	cout<<"\\begin{document}"<<endl;
	cout<<"For the following polynomial $f$ over the real domain and constraint, find the solution set for a hyperplane separating a positive frame $p$ from the rest of the $frame(f)$:"<<endl;
	fillTableau(table,rows,col);
	viewPolynom1(table,rows,col,greater);
	double value = evalOne(table,col);
	if(value > 0){   greater = true;}
	if(value < 0) { greater = false;}
	if((table[1][1] == -1 || table[1][1] == 0) && value > 0){
		cout<<"\\end{document}"<<endl;
		return 0;
	}
	if((table[1][1] == 0 || table[1][1] == 1) && value == 0){
		cout<<"\\end{document}"<<endl;
		return 0;
	}
	viewPolynom(table,rows,col,greater);
	if(positives[0] == 0 && positives[1] == 0){ cout<<"No positive frame. Subtropical Satisfiability would return 'Unknown'."; }
	else{
	linearProblem(table,linearRows,greater);
	viewTableau1(linearTable,linearRows,3);
	triangular(linearTable,linearRows,3); }
	cout<<"\\end{document}"<<endl;
	return 0;
}
