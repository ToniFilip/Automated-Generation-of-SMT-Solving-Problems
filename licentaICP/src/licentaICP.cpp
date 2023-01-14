//============================================================================
// Name        : licentaICP.cpp
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
#include <gmp.h>
#include <gmpxx.h>
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
int equal(double x, double y) { return fabs(x-y) < alpha; }
double values[7] = {-2,-1,0,1,2,3,4};
double values1[5] = {1,2,3,4,5};
#define INF  std::numeric_limits<double>::infinity();


struct interval{
	double lower;
	double upper;
};

bool secondone = false;
interval second;
ostream& operator<<(ostream& os, interval& a){
	if(secondone == false){
		if(a.lower == std::numeric_limits<double>::infinity() ||  a.lower == -std::numeric_limits<double>::infinity())
			os<<"(";
		if(a.lower != std::numeric_limits<double>::infinity() &&  a.lower != -std::numeric_limits<double>::infinity())
			os<<"[";
		os<<a.lower<<";"<<a.upper;
		if(a.upper == std::numeric_limits<double>::infinity() ||  a.upper == -std::numeric_limits<double>::infinity())
			os<<")";
		if(a.upper != std::numeric_limits<double>::infinity() &&  a.upper != -std::numeric_limits<double>::infinity())
			os<<"]";

	}else
	{   if(second.lower > a.lower && second.upper > a.upper){
		if(a.lower == std::numeric_limits<double>::infinity() ||  a.lower == -std::numeric_limits<double>::infinity())
			os<<"(";
		if(a.lower != std::numeric_limits<double>::infinity() &&  a.lower != -std::numeric_limits<double>::infinity())
			os<<"[";
		os<<a.lower<<";"<<a.upper;
		if(a.upper == std::numeric_limits<double>::infinity() ||  a.upper == -std::numeric_limits<double>::infinity())
			os<<")";
		if(a.upper != std::numeric_limits<double>::infinity() &&  a.upper != -std::numeric_limits<double>::infinity())
			os<<"]";
		cout<<" \\cup ";
		if(second.lower == std::numeric_limits<double>::infinity() ||  second.lower == -std::numeric_limits<double>::infinity())
			os<<"(";
		if(second.lower != std::numeric_limits<double>::infinity() &&  second.lower != -std::numeric_limits<double>::infinity())
			os<<"[";
		os<<second.lower<<";"<<second.upper;
		if(second.upper == std::numeric_limits<double>::infinity() ||  second.upper == -std::numeric_limits<double>::infinity())
			os<<")";
		if(second.upper != std::numeric_limits<double>::infinity() &&  second.upper != -std::numeric_limits<double>::infinity())
			os<<"]";
	}
	   if(a.lower > second.lower && a.upper > second.upper){
			if(second.lower == std::numeric_limits<double>::infinity() ||  second.lower == -std::numeric_limits<double>::infinity())
				os<<"(";
			if(second.lower != std::numeric_limits<double>::infinity() &&  second.lower != -std::numeric_limits<double>::infinity())
				os<<"[";
			os<<second.lower<<";"<<second.upper;
			if(second.upper == std::numeric_limits<double>::infinity() ||  second.upper == -std::numeric_limits<double>::infinity())
				os<<")";
			if(second.upper != std::numeric_limits<double>::infinity() &&  second.upper != -std::numeric_limits<double>::infinity())
				os<<"]";
			cout<<" \\cup ";
			if(a.lower == std::numeric_limits<double>::infinity() ||  a.lower == -std::numeric_limits<double>::infinity())
				os<<"(";
			if(a.lower != std::numeric_limits<double>::infinity() &&  a.lower != -std::numeric_limits<double>::infinity())
				os<<"[";
			os<<a.lower<<";"<<a.upper;
			if(a.upper == std::numeric_limits<double>::infinity() ||  a.upper == -std::numeric_limits<double>::infinity())
				os<<")";
			if(a.upper != std::numeric_limits<double>::infinity() &&  a.upper != -std::numeric_limits<double>::infinity())
				os<<"]";

	   }
	}
	return os;
}

interval operator+(interval a, interval b){
	interval c;
	c.lower = a.lower + b.lower;
	c.upper = a.upper + b.upper;
	return c;
}

interval operator-(interval a, interval b){
	interval c;
	c.lower = a.lower - b.upper;
	c.upper = a.upper - b.lower;
	return c;
}

interval operator*(interval a, interval b){
	interval c;
	float min = 1000;
	float max = -1000;
	float temp = 0;
	temp = a.lower * b.lower;
	if(min > temp) min = temp;
	if(max < temp) max = temp;
	temp = a.lower * b.upper;
	if(min > temp) min = temp;
	if(max < temp) max = temp;
	temp = a.upper * b.lower;
	if(min > temp) min = temp;
	if(max < temp) max = temp;
	temp = a.upper * b.upper;
	if(min > temp) min = temp;
	if(max < temp) max = temp;
	c.lower = min;
	c.upper = max;

	if( a.lower == b.lower && a.upper == b.upper && a.lower < 0){
		c.lower = 0;
	}
	return c;
}

interval operator/(interval a, interval b){
	interval c,d;
	d.lower = 1/b.upper;
	d.upper = 1/b.lower;
	c = a*d;

	if(b.lower <= 0 && 0 <= b.upper){
		if(a.lower <= 0 && 0 <= a.upper){
			c.lower = -INF;
			c.upper = INF;
		}
		if(a.upper < 0){
			if(b.lower < b.upper && b.upper == 0){
				c.lower = a.upper/b.lower;
				c.upper = INF;
			}
			if(b.lower < 0 && 0 < b.upper){
				c.lower = -INF;
				c.upper = a.upper/b.upper;
				second.lower =a.upper/b.lower;
				second.upper = +INF;
				secondone = true;
			}
			if(0 == b.lower && b.lower < b.upper){
				c.lower = -INF;
				c.upper = a.upper/b.upper;
			}

		}
		if(0 < a.lower){
			if(b.lower < b.upper && b.upper == 0){
				c.lower = -INF;
				c.upper = a.lower/b.lower;
			}
			if(b.lower < 0 && 0 < b.upper){
				c.lower = -INF;
				c.upper = a.lower/b.lower;
				second.lower = a.lower/b.upper;
				second.upper = INF;
				secondone = true;
			}
			if(0 == b.lower && b.lower < b.upper){
				c.lower = a.lower/b.upper;
				c.upper = INF;
			}
		}
	}

	return c;
}

void fillTableau(double table[][7], int rows, int col){
	int random = 0;
	for(int j = 1; j < col+1; j++){
		table[0][j] = 0;
	}
	for(int i = 1; i < rows+1; i++){
		random = (rand() % 5);
		table[i][0] = values1[random];
	}

	for(int i = 1 ; i < rows+1; i++){
		for(int j = 1; j < col+1; j++){
			random = (rand() % 7);
			table[i][j] = values[random];
		}
	}
}

double* vietaFormulas(double table[][7], int row, int n)
{
    double *coeff = new double[n + 1];
    double roots[5] = {};
    int temp = 0;
    for(int j = 0; j < n; j++){
    	roots[temp] = table[row][j+1];
    	temp++;
    }
    coeff[n] = 1;

    for (int i = 1; i <= n; i++) {
        for (int j = n - i - 1; j < n; j++) {
            coeff[j] = coeff[j] + (-1) *
                roots[i - 1] * coeff[j + 1];
        }
    }

   cout << "The given polynomial is: "<<endl;
    cout<< "$f = ";
    for (int i = n; i >= 0; i--) {
    	if(i == n) cout<<"x^"<<n;
    	else{
    		if(coeff[i] > 1){
    			if(i > 1)
    			cout<<" + "<<coeff[i]<<"x^"<<i;
    			if(i == 1)
    				cout<<" + "<<coeff[i]<<"x";
    			if(i == 0)
    				cout<<" + "<<coeff[i];
    		}
    		if(coeff[i] == 1)cout<<" + x^"<<i;
    		if(coeff[i] == 0) continue;
    		if(coeff[i] == -1)cout<<" - x^"<<i;
    		if(coeff[i] < -1){
    			if(i > 1)
    			cout<<" - "<<-coeff[i]<<"x^"<<i;
    			if(i == 1)
    				cout<<" - "<<-coeff[i]<<"x";
    			if(i == 0)
    				cout<<" - "<<-coeff[i];
    		}
    	}
    }
    cout<<"$, with ";
    cout<<'\n';
    return coeff;
}

interval evalDerivate(double table[0][7], int row, int col, interval b){
	interval a,temp1,temp2,temp3;
	interval c = b;
	cout<<"The set of derivatives in $f$ is: ";
	cout<<'\n';
	cout<<"\\begin{center}";
	cout<<'\n';
	cout<<"$f'(A) = ";
	for(int j = col; j >= 2; j--){
		if(table[row][j] < 0){
			temp1.lower = -table[row][j];
			temp1.upper = -table[row][j];
			cout<<temp1;
		}else{
			temp1.lower = table[row][j];
			temp1.upper = table[row][j];
			cout<<temp1;

		}
		temp2.lower = j-1;
		temp2.upper = j-1;
		cout<<"\\cdot "<<temp2;
		temp3 = c;
		for(int i = 1; i < j-2; i++){
			c = c * temp3;
		}

		temp1 = temp1 * temp2;

		if(j > 2) {
			temp1 = temp1 * c;
			cout<<"\\cdot"<<c;

		}
		if(table[row][j] < 0){
			a = a - temp1;

		}
		else{
			a = a + temp1;
		}
		if(table[row][j-1] < 0 && j > 2){
			cout<< " - ";
		}
		if(table[row][j-1] >= 0 && j > 2){
			cout<<" + ";
		}
		c = b;
	}
	cout<<" = "<<a;
	cout<<" $.";
	cout<<'\n';
	cout<<"\\end{center}";
	return a;
}

double evalFunction(double table[0][7], int row, int col, double s){
	double a = 0;
	cout<<"Evaluating the polynomial in point "<<s<<":"<<endl;
	cout<<'\n';
	cout<<"\\begin{center}";cout<<'\n';
	cout<<"$f("<<s<<") = ";
	double temp = 0;
	double temp1 = 0;
	double c = s;
	for(int j = 1; j < col+1; j++){
		temp = table[row][j];
		if(temp > 0)
		cout<<temp;
		if(temp < 0)
		cout<<-temp;
		temp1 = pow(c, j-1);
		c = temp1;
		if(j > 1) {
			temp = temp * c;
			cout<<"\\cdot "<<temp1;
		}
		if(temp > 0)
		a = a + temp;
		if(j != col)
		{
			c = s;
			if(table[row][j+1] > 0 && temp != 0)
			cout<<" + ";
			if(table[row][j+1] < 0 && temp != 0)
			cout<<" - ";
		}
	}
	cout<<" = "<<a<<".$"<<endl;
	cout<<'\n';
	cout<<"\\end{center}";
	return a;
}
double evalFunction1(double table[0][7], int row, int col, double s){
	double a;
	double temp = 0;
	double temp1 = 0;
	double c = s;
	for(int j = 1; j < col+1; j++){
		temp = table[row][j];
		temp1 = pow(c, j-1);
		c = temp1;
		if(j > 1) {
			temp = temp * c;
		}
		a = a + temp;

		c = s;
	}
	cout<<"$ f(s) = "<<a<<"$."<<endl;
	return a;

	}

interval possibleZeros(double table[0][7], int row, int col, double point,interval a){
	interval temp2,temp3, zeros, temp4;
	double temp1 = 0;
	interval b;
	b.lower = point;
	b.upper = point;
	temp1 = evalFunction1(table,row,col,point);
	temp2 = evalDerivate(table,row,col,a);

	temp3.lower = temp1;
	temp3.upper = temp1;
	temp4 = temp3/temp2;
	cout<<"The set of possible zeros in A: ";
	cout<<'\n';
	cout<<"\\begin{center}";
	cout<<'\n';
	cout<<"$ s-\\frac{f(s)}{f'(A)} = ";
	cout<<"["<<point<<";"<<point<<"]";
	cout<<" - ";
	cout<<"\\frac{"<<"["<<temp3.lower<<";"<<temp3.upper<<"]"<<"}{"<<"["<<temp2.lower<<";"<<temp2.upper<<"]"<<"} = ";
	zeros = b - temp4;
	if(secondone == true){
		second = b - second;
	}
	cout<<zeros<<"$.";
	cout<<'\n';
	cout<<"\\end{center}";

	return zeros;
}
void icp(double table[0][7], int rows, int col, double point,interval a){
	interval b,temp,temp1;
	bool changed = 0;
	bool upperchanged = 0;
	for(int i = 1; i < rows+1; i++){
		temp = a;

		b = possibleZeros(table, i, col, point,a);
		secondone = false;
		cout<<"The new contracted interval $A'$  will be: ";
		cout<<'\n';
		cout<<"\\begin{center}";
		cout<<'\n';
		cout<<"$";
		cout<<"A' = ";
		cout<<a<<" \\cap ";
		if(second.lower != 0 || second.upper != 0){ secondone = true;
		cout<<"("<<b<<")"<<endl;}
		if(secondone == false) cout<<b<<endl;
		if(secondone == true){
			if(second.lower > a.lower && second.lower < a.upper) temp.lower = second.lower;
			if(second.upper < a.upper && second.upper >= a.lower) temp.upper = second.upper;
		}

		if(b.lower > a.lower && b.lower < a.upper){
			temp1.lower = b.lower;
			changed = 1;
		}
		if(b.upper < a.upper && b.upper >= a.lower){
			temp1.upper = b.upper;
			changed = 1;
			upperchanged = 1;
		}
		if(upperchanged == 0) temp1.upper = a.upper;

		secondone = false;
		if(temp.lower == a.lower && temp.upper == a.upper){
			if(changed == 1){
				cout<<" = ";
				cout<<temp1;

			}else{
				cout<<" = \\emptyset.$\\\\";
				cout<<"Because the result is the empty set, the starting interval did not change ";
				cout<<"$\\Rightarrow ";
				cout<<"A' = ";
				cout<<a; cout<<".";}
		}
		else{
			cout<<" = ";
			cout<<temp;
			if(changed == 1){
				cout<<" \\cup ";
				cout<<temp1;
			}
		}
		changed = 0;
		cout<<"$";
		cout<<'\n';
		cout<<"\\end{center}";
	}
}


int main() {

	srand(time(NULL));
	double table[7][7] = {};
	double coeff[7][7] = {};
	int rows = 1;
	int col = 0;
	int max = -1000;
	int min = 1000;
	double s = 2;
	interval a;

	cout << "Highest power of the polynomial: ";
	cin>>col;
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
	cout<<"For the following polynomial perform one step of interval constraint propagation using Newton's Method. "<<endl;

	do{
		max = -1000;
		fillTableau(table,rows,col);

		for(int j = 1; j < col+1; j++){
			if(max < table[1][j])
				max = table[1][j];
		}

	}while(max <= 2);
	min = 0;
	a.lower = min;
	a.upper = max;
	s = 2;
    cout<<'\n';
    cout<<"The given starting interval will be $"<<a<<"$ and sample point $s = "<<s<<" $. ";
    for(int k = 1; k < rows+1; k++){
    	double* table2 = vietaFormulas(table,k,col);
        for (int i = col; i >= 0; i--) {
            coeff[k][i+1] = table2[i];
        }
    }
    col++;
    icp(coeff,rows,col,s,a);
    cout<<"\\end{document}"<<endl;
	return 0;
}

