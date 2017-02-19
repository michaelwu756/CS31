#include<iostream>
#include<ctype.h>
#include<string>
#include<cassert>
using namespace std;

bool hasCorrectSyntax(string pollData);
int countVotes(string pollData, char party, int& voteCount);
bool isValidUppercaseStateCode(string stateCode);
bool predictsZeroVotes(string pollData);
bool isValidNumber(string num);
string strToUpper(string str);

int main()
{
		/*assert(hasCorrectSyntax("TX38RCA55D"));
	    assert(!hasCorrectSyntax("MX38RCA55d"));
	    int votes;
	    votes = -999;    // so we can detect whether countVotes sets votes
	    assert(countVotes("TX38RCA55DMs6rnY29dUT06L", 'd', votes) == 0  &&  votes == 84);
	    votes = -999;    // so we can detect whether countVotes sets votes
	    assert(countVotes("TX38RCA55D", '%', votes) == 3  &&  votes == -999);
		assert(countVotes("TX38RCA 5D", 'd', votes) == 1  &&  votes == -999);
		assert(countVotes("TX38RCA00D", 'd', votes) == 2  &&  votes == -999);
		(hasCorrectSyntax("")==true);//empty string
		assert(hasCorrectSyntax("AL1DAK1DAZ1DAR1DCA1DCO1DCT1DDE1DDC1DFL1DGA1DHI1DID1DIL1DIN1DIA1DKS1DKY1DLA1DME1DMD1DMA1DMI1DMN1DMS1DMO1DMT1DNE1DNV1DNH1DNJ1DNM1DNY1DNC1DND1DOH1DOK1DOR1DPA1DRI1DSC1DSD1DTN1DTX1DUT1DVT1DVA1DWA1DWV1DWI1DWY1D")==true);//every state code test	
		assert(hasCorrectSyntax(" ")==false);//non alphanumeric character
		assert(hasCorrectSyntax("NJ39")==false);//missing party
		assert(hasCorrectSyntax("39D")==false);//missing state
		assert(hasCorrectSyntax("XY33B")==false);//invalid state code
		assert(hasCorrectSyntax("NJD")==false);//missing electoral vote number
		assert(hasCorrectSyntax("NJ999D")==false);//number must have 1 or 2 digits
		assert(hasCorrectSyntax("NJ1D")==true);//1 digit
		assert(hasCorrectSyntax("NJ11D")==true);//2 digits
		assert(hasCorrectSyntax("nj1d")==true);//lowercase

		assert(countVotes("TX38RCA 5D", 'd', votes)==1);//incorrect syntax
		assert(countVotes("TX38RCA00D", 'd', votes)==2);//state with zero votes
		assert(countVotes("TX38RCA01D", '1', votes)==3);//invalid party
		assert(countVotes("TX38RCA01D", 'd', votes)==0 && votes==1);//checks votes correctly
	    cout << "All tests succeeded" << endl;*/
}
bool hasCorrectSyntax(string pollData)
{
	size_t k = 0;
	while(k<pollData.size())
	{
		int mode = 0;//uses mode to parse in each each poll data string
		string stateCode = "";
		string num = "";
		string party = "";
		while (k<pollData.size() && mode < 2)
		{
			switch(mode)
			{
				case 0:
					stateCode+=pollData[k];//first mode reads in the first two letters
					if (stateCode.size()==2)
						mode++;
					break;
				case 1:
					if(isdigit(pollData[k]))//then it reads in digits until it comes across a non letter
						num+=pollData[k];
					else
					{
						party+=pollData[k];//then it stores the next character as the party
						mode++;
					}
					break;
			}
			k++;
		}
		if(!isValidUppercaseStateCode(strToUpper(stateCode))||!isValidNumber(num)||!isalpha(party[0]))//if data not valid
			return false;
	}
	return true;//returns true only if each poll data string is valid
}
int countVotes(string pollData, char party, int& voteCount)
{
	pollData = strToUpper(pollData);//uppercases everything
	party = toupper(party);
	if(!hasCorrectSyntax(pollData))//checks syntax
		return 1;
	if(predictsZeroVotes(pollData))//checks if any state vote numbers are zero
		return 2;
	if(!isalpha(party))//checks that party is a letter
		return 3;
	voteCount = 0;
	string num ="";
	for(size_t i=0;i<pollData.size();i++)
	{
		if(isdigit(pollData[i]))//gets strings of digits
			num+=pollData[i];
		else
		{
			if(num!="" && pollData[i]==party)//if the party letter matches, add to the vote count
				voteCount+=stoi(num);
			num="";//resets string of digits
		}
	}
	return 0;
}
bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
	"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
        "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
	    stateCode.find('.') == string::npos  &&  // no '.' in stateCode
	    codes.find(stateCode) != string::npos);  // match found
}
bool predictsZeroVotes(string pollData)
{
	string num ="";
	for(size_t i=0;i<pollData.size();i++)
	{
		if(isdigit(pollData[i]))//gets strings of digits
			num+=pollData[i];
		else//once a string of digits ends, checks if that string is all zeros
		{
			if(num!="" && stoi(num)==0)
				return true;
			num="";
		}
	}
	return false;
}
bool isValidNumber(string num)
{
	if(num.size()==0)
		return false;
	for(size_t i=0;i<num.size();i++)//checks each character to make sure it is a digit
		if(!isdigit(num[i]))
			return false;
	if(num.size()<=2)//must have 1 or 2 digits
		return true;
	return false;
}
string strToUpper(string str)//converts a string to upper case
{
	string returnStr = "";
	for(size_t i=0;i<str.size();i++)
		returnStr+=toupper(str[i]);	
	return returnStr;
}