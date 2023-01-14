//============================================================================
// Name        : licenta.cpp
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
#include <iomanip>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <complex>
#include <cmath>

using namespace std;
using std::ostringstream;
using std::ifstream;
using std::complex;
using std::cout;
using std::endl;
using std::ios;
static const double alpha   = 1.0e-8;
int equal(double x, double y) {
	return fabs(x-y) < alpha; }
double values[9] = {-2,-1,0,1,2,3,4,5,6};
double values1[5] = {1,2,3,4,5};
int oldCol = 0;
int basisLocation[3] = {};
int nonBasisLocation[3] = {};
double secondTab[7][7] = {};

void addSlacks(double table[][7], int rows, int col);
int rowPivot(double table[][7], int rows, int col);
int colPivot(double table[][7], int rows, int col, int pivotrow);
void pivot(double table[][7], int rows, int col, int pivotrow, int pivotcol);
void simplex1(double table[][7], int rows, int col);
bool basisOn = false;
bool sat = false;

bool integer(double k)
{
    return k == (double)(int)k;
}

void fillTableau(double table[][7], int rows, int col){
	int random = 0;
	int temporal = 0;
	double temp,temp1;
	bool good = false;

	for(int j = 1; j < col+1; j++){
		table[0][j] = 0;
	}
	for(int i = 1; i < rows+1; i++){
		random = (rand() % 5);
		table[i][0] = values1[random];
	}
	while(good == false){
		start:
		for(int i = 1; i < col+1; i++){
			nonBasisLocation[i-1] = i;
		}
		for(int j = 1; j < col+rows+1; j++){
			table[0][j] = 0;
		}
		for(int i = 1; i < rows+1; i++){
			random = (rand() % 5);
			table[i][0] = values1[random];
		}
		for(int i = col+1; i < col+rows+1; i++){
			basisLocation[temporal] = i;
			temporal++;
		}
		temporal = 0;
		for(int i = 1 ; i < rows+1; i++){
			for(int j = 1; j < col+1; j++){
				random = (rand() % 9);
				table[i][j] = values[random];
			}
		}
		table[2][1] = -1;
		for(int i = 0; i < rows+1; i++){
			for(int j = 0; j < col+1; j++){
				secondTab[i][j] = table[i][j];
			}
		}
		addSlacks(table,rows,col);
		col += rows;
		temp = table[2][3]*table[1][1] - table[2][1]*table[1][3];
		temp1 = table[2][2]*table[1][1] - table[1][2]*table[2][1];

		simplex1(table,rows,col);
		col = col - rows;
		for(int i = 1; i < rows+1; i++){
			for(int j = 1; j < col+1; j++){
				if(fmod(table[i][j],1.0) != 0 || table[0][2] == 0 || (rows == 3 && table[0][3] == 0) || fmod(table[1][3],table[1][1]) != 0 || fmod(table[1][2],table[1][1]) != 0 || fmod(temp,table[1][1]) != 0 ||
						fmod(temp1,table[1][1]) != 0 || fmod(temp,temp1) != 0 || fmod(table[1][1],temp1) != 0 || fmod(table[1][2],table[1][1]*temp1) != 0){
					goto start;
				}
			}
		}
		good = true;
	}
}

void viewLatexTableau(double table[][7], int rows, int col){

	  int i, j,ct;
	  cout<<"\\begin{table}[H]"<<endl;
	  cout<<"\\begin{tabular}{ |p{2cm}||";
	  for(ct = 0; ct < col+1; ct++){
		  cout<<"p{2cm}|";
	  }
	  cout<<" }"<<endl;
	  cout<<"\\hline"<<endl;
	  cout<<"Constraint& ";
	  for(j=1;j<col+1; j++) {
		  if(j == nonBasisLocation[0]){ cout<<"$x_1$ & ";
		  }

		  if(j == nonBasisLocation[1]){ cout<<"$x_2$ & ";
		  }
		  if(j == nonBasisLocation[2]){ cout<<"$x_3$ & ";
		  }
		  if(j != nonBasisLocation[0] && j != nonBasisLocation[1] && j != nonBasisLocation[2]){
			  if(j == basisLocation[0]){ cout<<"$s_1$ & ";
			  }
			  if(j == basisLocation[1]){ cout<<"$s_2$ & ";

			  }
			  if(j == basisLocation[2]){ cout<<"$s_3$ & ";}
		  }
	  }
	  cout<<"bound";
	  cout<<"\\\\"<<endl;
	  cout<<"\\hline"<<endl;
	  for(i=0;i<rows+1; i++) {
		if(i == 0) cout<<"Assignment ";
		if(i > 0) printf("$f_%d$ ", i);
	    for(j=1;j<col+1; j++) {
	      if ((int)table[i][j] == table[i][j])
	        { cout<<" & ";
	    	  printf(" %6d", (int)table[i][j]);

	        }
	      else{
	    	  cout<<" & ";
	        printf(" %6.2lf", table[i][j]);
	      }
	      }

	      if ((int)table[i][0] == table[i][0]){
	    	  cout<<" & ";
	        printf(" %6d", (int)table[i][0]);

	      }
	      else {
	    	  cout<<" & ";
	        printf(" %6.2lf", table[i][0]);

	      }
	      cout<<"\\\\"<<endl;
	    printf("\n");
	  }
	  printf("\n");
	  cout<<"\\hline"<<endl;
	  cout<<"\\end{tabular}"<<endl;
	  cout<<"\\end{table}"<<endl;
}

void addSlacks(double table[][7], int rows, int col){
	int i, j, temp;
	temp = 0;
	for(i=1; i<rows+1; i++) {
	  for(j=1; j<rows+1; j++)
	    table[i][j + col] = (i==j);
	}
	for(int i = col+1; i < col+rows+1; i++){
		basisLocation[temp] = i;
		temp++;
	}
}

int rowPivot1(double table[][7], int rows, int col){
	int temp = col - rows;
	for(int j = col-rows+1; j < col+1; j++){
		if(j == basisLocation[0] || j == basisLocation[1] || j == basisLocation[2]){
			if(table[0][j] < table[j-temp][0]){
				return j-temp;
			}
		}
	}
	return 0;
}

int colPivot1(double table[][7], int rows, int col, int pivotrow){
	int coeff = 0;
	bool found = 0;
	double val = 0;
	for(int j = 1; j < col-rows+1; j++){
		if(j != basisLocation[0] && j != basisLocation[1] && j != basisLocation[2]){
			if(table[pivotrow][j] != 0){
				val = table[pivotrow][j];
				found = 1;
				coeff = j;
				break;
			}
		}
	}
	if(found == 0){
		for(int j = 1; j < col-rows+1; j++){
			if(j == basisLocation[0]){
				if(table[pivotrow][j] > 0 && table[0][j] < table[1][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
				if(table[pivotrow][j] < 0 && table[0][j] > table[1][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
			}
			if(j == basisLocation[1]){
				if(table[pivotrow][j] > 0 && table[0][j] < table[2][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
				if(table[pivotrow][j] < 0 && table[0][j] > table[2][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
			}
			if(j == basisLocation[2]){
				if(table[pivotrow][j] > 0 && table[0][j] < table[3][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
				if(table[pivotrow][j] < 0 && table[0][j] > table[3][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
			}
		}

	}
	return coeff;
}

void pivot1(double table[][7], int rows, int col, int pivotrow, int pivotcol) {
  double pivot;
  double temp = 0;

  pivot = table[pivotrow][pivotcol];
  for( int j = 1; j<col+1; j++)
    table[pivotrow][j] /= pivot;

  for(int i = 1; i<rows+1; i++) {
    double temp = table[i][pivotcol];
    if(i==pivotrow) continue;
    for(int j=1; j<col+1; j++) {
      table[i][j] -= temp * table[pivotrow][j];
    }
  }

  for(int i = 1; i < rows+1; i++){
	  temp = table[i][pivotcol];
	  table[i][pivotcol] = table[i][pivotrow+col-rows];
	  table[i][pivotrow+col-rows] = temp;
  }
  for(int i = 1; i < rows+1; i++){
	  for(int j = 1; j <= col-rows; j++){
		if(i == pivotrow && j != pivotcol)
			{
			table[i][j] *= -1;

			}
		if(i != pivotrow && j == pivotcol){
			table[i][j] *= -1;

		}
	  }
  }
  table[0][pivotcol] = table[pivotrow][0];
  basisLocation[pivotrow-1] = pivotcol;
  if(basisOn == true) basisLocation[pivotcol-1] = pivotrow;

  if(rows == 2 && col == 4){
	  table[0][3] = table[0][1]*table[1][1] + table[0][2]*table[1][2];
	  table[0][4] = table[0][1]*table[2][1] + table[0][2]*table[2][2];
  }
  if(rows == 3 && col == 6){
	  table[0][4] = table[0][1]*table[1][1] + table[0][2]*table[1][2] + table[0][3]*table[1][3];
	  table[0][5] = table[0][1]*table[2][1] + table[0][2]*table[2][2] + table[0][3]*table[2][3];
	  table[0][6] = table[0][1]*table[3][1] + table[0][2]*table[3][2] + table[0][3]*table[3][3];
  }

  if(rows == 2 && col == 5){
	  table[0][4] = table[0][1]*table[1][1] + table[0][2]*table[1][2] + table[0][3]*table[1][3];
	  table[0][5] = table[0][1]*table[2][1] + table[0][2]*table[2][2] + table[0][3]*table[2][3];
  }

  if(rows == 3 && col == 5){
	  table[0][3] = table[0][1]*table[1][1] + table[0][2]*table[1][2];
	  table[0][4] = table[0][1]*table[2][1] + table[0][2]*table[2][2];
	  table[0][5] = table[0][1]*table[3][1] + table[0][2]*table[3][2];
  }
}


int rowPivot(double table[][7], int rows, int col){
	int temp = col - rows;
	for(int j = col-rows+1; j < col+1; j++){
		if(j == basisLocation[0] || j == basisLocation[1] || j == basisLocation[2]){
			if(table[0][j] < table[j-temp][0]){
				cout<<"Slack variable with number "<<j-temp<<" needs to be pivoted, because $"<<table[0][j]<<" < "<<table[j-temp][0]<<"$."<<endl;
				return j-temp;
			}
		}
	}
	cout<<"No slack variable is violating the bound. We are done. The current assignments constitute a solution."<<endl;
	return 0;
}

int colPivot(double table[][7], int rows, int col, int pivotrow){
	int coeff = 0;
	bool found = 0;
	double val = 0;
	for(int j = 1; j < col-rows+1; j++){
		if(j != basisLocation[0] && j != basisLocation[1] && j != basisLocation[2]){
			if(table[pivotrow][j] != 0){
				val = table[pivotrow][j];
				found = 1;
				coeff = j;
				break;
			}
		}
	}
	if(found == 0){
		for(int j = 1; j < col-rows+1; j++){
			if(j == basisLocation[0]){
				if(table[pivotrow][j] > 0 && table[0][j] < table[1][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
				if(table[pivotrow][j] < 0 && table[0][j] > table[1][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
			}
			if(j == basisLocation[1]){
				if(table[pivotrow][j] > 0 && table[0][j] < table[2][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
				if(table[pivotrow][j] < 0 && table[0][j] > table[2][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
			}
			if(j == basisLocation[2]){
				if(table[pivotrow][j] > 0 && table[0][j] < table[3][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
				if(table[pivotrow][j] < 0 && table[0][j] > table[3][0]){
					val = table[pivotrow][j];
					found = 1;
					coeff = j;
					basisOn = true;
					break;
				}
			}
		}

	}
	if(pivotrow != 0){
	if(coeff != 0) cout<<"Variable $x_"<<coeff<<"$ is suitable for pivoting with coefficient $"<<val<<".$"<<endl;
	if(coeff == 0) cout <<"No variable suitable for pivoting was found."<<endl; }
	return coeff;
}

void pivot(double table[][7], int rows, int col, int pivotrow, int pivotcol) {
  double pivot;
  double temp = 0;

  pivot = table[pivotrow][pivotcol];
  for(int j = 1; j < col+1; j++)
    table[pivotrow][j] = table[pivotrow][j]/pivot;

  for(int i = 1; i < rows+1; i++) {
    double temp = table[i][pivotcol];
    if(i == pivotrow) continue;
    for(int j = 1; j < col+1; j++) {
      table[i][j] =  table[i][j] - temp * table[pivotrow][j];
    }
  }

  for(int i = 1; i < rows+1; i++){
	  temp = table[i][pivotcol];
	  table[i][pivotcol] = table[i][pivotrow+col-rows];
	  table[i][pivotrow+col-rows] = temp;
  }
  for(int i = 1; i < rows+1; i++){
	  for(int j = 1; j <= col-rows; j++){
		if(i == pivotrow && j != pivotcol)
			{
			table[i][j] *= -1;

			}
		if(i != pivotrow && j == pivotcol){
			table[i][j] *= -1;

		}
	  }
  }
  table[0][pivotcol] = table[pivotrow][0];
  basisLocation[pivotrow-1] = pivotcol;
  nonBasisLocation[pivotcol-1] = pivotrow+col-rows;
  if(basisOn == true) basisLocation[pivotcol-1] = pivotrow;

  if(rows == 2 && col == 4){
	  table[0][3] = table[0][1]*table[1][1] + table[0][2]*table[1][2];
	  table[0][4] = table[0][1]*table[2][1] + table[0][2]*table[2][2];
  }
  if(rows == 3 && col == 6){
	  table[0][4] = table[0][1]*table[1][1] + table[0][2]*table[1][2] + table[0][3]*table[1][3];
	  table[0][5] = table[0][1]*table[2][1] + table[0][2]*table[2][2] + table[0][3]*table[2][3];
	  table[0][6] = table[0][1]*table[3][1] + table[0][2]*table[3][2] + table[0][3]*table[3][3];
  }

  if(rows == 2 && col == 5){
	  table[0][4] = table[0][1]*table[1][1] + table[0][2]*table[1][2] + table[0][3]*table[1][3];
	  table[0][5] = table[0][1]*table[2][1] + table[0][2]*table[2][2] + table[0][3]*table[2][3];
  }

  if(rows == 3 && col == 5){
	  table[0][3] = table[0][1]*table[1][1] + table[0][2]*table[1][2];
	  table[0][4] = table[0][1]*table[2][1] + table[0][2]*table[2][2];
	  table[0][5] = table[0][1]*table[3][1] + table[0][2]*table[3][2];
  }
  cout<<"The new location of slack variable with number $"<<pivotrow<<"$ is $"<<basisLocation[pivotrow-1]<<".$"<<endl;
  cout<<'\n';
}

void simplex1(double table[][7], int rows, int col){
	int pivotrow = 0;
	int pivotcol = 0;
	bool violate = 1;
	while(violate == 1){
		pivotrow = rowPivot1(table,rows,col);
		pivotcol = colPivot1(table,rows,col,pivotrow);
		if(pivotrow == 0 || pivotcol == 0){
			violate = 0;
			break;
		}
		pivot1(table,rows,col,pivotrow,pivotcol);
	}
}

void simplex(double table[][7], int rows, int col){
	int pivotrow = 0;
	int pivotcol = 0;
	bool violate = 1;
	while(violate == 1){
		pivotrow = rowPivot(table,rows,col);
		pivotcol = colPivot(table,rows,col,pivotrow);
		if(pivotrow == 0 || pivotcol == 0){
			violate = 0;
			break;
		}
		pivot(table,rows,col,pivotrow,pivotcol);
		viewLatexTableau(table,rows,col);
	}
}

int main() {
	srand(time(NULL));
	int rows = 2;
	int col = 2;
	cout<<"Only matrices of form 2x2, 2x3, 3x2, 3x3."<<endl;
	cout << "Number of constraints: ";
	cin>>rows;
	cout << "Number of variables: ";
	cin>>col;
	cout<<"The file with the Latex code 'latexOutput.txt' has been created and is located in the folder of the project.";

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
	cout<<"Given is the following initial tableau. Bring the tableau to a standard form and find a solution. ";
	cout<<"The ordering for choosing a pivot candidate should be ";
	if(rows == 2 && col == 2) cout<<"$x_1 > x_2 > s_1 > s_2.$";
	if(rows == 2 && col == 3) cout<<"$x_1 > x_2 > x_3 > s_1 > s_2.$";
	if(rows == 3 && col == 2) cout<<"$x_1 > x_2 > s_1 > s_2 > s_3.$";
	if(rows == 3 && col == 3) cout<<"$x_1 > x_2 > x_3 > s_1 > s_2 > s_3$."<<endl;;
	double table[7][7] = {};

	fillTableau(table,rows,col);
	viewLatexTableau(secondTab,rows,col);
	addSlacks(secondTab,rows,col);
	cout<<"Tableau after adding the slack variables: "<<endl;
	col = rows+col;
	viewLatexTableau(secondTab,rows,col);
	simplex(secondTab,rows,col);

	cout<<"\\end{document}"<<endl;
	return 0;
}
