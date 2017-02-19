#include<iostream>
#include<string>
#include<iomanip>
using namespace std;

int main()
{
	string id;
	double revenue;
	string country;
	double fee;
	cout<<"Identification: ";
	getline(cin,id);
	cout<<"Expected revenue (in millions): ";
	cin>>revenue;
	cin.ignore(10000, '\n');
	cout<<"Country: ";
	getline(cin,country);
	cout<<"---"<<endl;
	
	double specialTax=0.203;
	if (country=="UAE" || country=="Turkey")
		specialTax=0.217;
	
	if(revenue<20)
		fee=0.181*revenue;
	else if(revenue<50)
		fee=0.181*20+specialTax*(revenue-20);
	else
		fee=0.181*20+specialTax*(30)+0.23*(revenue-50);
	
	if(id=="")
		cout<<"You must enter a property identification.";
	else if(revenue<0)
		cout<<"The expected revenue must be nonnegative.";
	else if(country=="")
		cout<<"You must enter a country.";
	else
		cout<<"The license fee for "<<id<<" is $"<<fixed<<setprecision(3)<<fee<<" million.";
	cout<<endl;
}