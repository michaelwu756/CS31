#include<string>
#include<iostream>
#include<cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
/*
Append value to the end of each of the n elements of the array and return n. [Of course, in this and other functions, if n is negative, the paragraph above that starts "Notwithstanding" trumps this by requiring that the function return -1. Also, in the description of this function and the others, when we say "the array", we mean the n elements that the function is aware of.] For example:
string people[5] = { "hillary", "gary", "donald", "jill", "evan" };
int j = appendToAll(people, 5, "!!!");  // returns 5
    // now cand[0] is "hillary!!!", cand[1] is "gary!!!", ...,
    // and cand[4] is "evan!!!"*/
int lookup(const string a[], int n, string target);
/*
Return the position of a string in the array that is equal to target; if there is more than one such string, return the smallest position number of such a matching string. Return -1 if there is no such string. As noted above, case matters: Do not consider "TIm" to be equal to "tIM".*/
int positionOfMax(const string a[], int n);
/*
Return the position of a string in the array such that that string is >= every string in the array. If there is more than one such string, return the smallest position in the array of such a string. Return -1 if the array has no elements. For example:
string cand[6] = { "jill", "hillary", "donald", "tim", "evan", "mike" };
int k = positionOfMax(cand, 6);   // returns 3, since  tim  is latest
                                  // in alphabetic order*/								  
int rotateLeft(string a[], int n, int pos);
/*
Eliminate the item at position pos by copying all elements after it one place to the left. Put the item that was thus eliminated into the last position of the array. Return the original position of the item that was moved to the end. Here's an example:
string running[5] = { "evan", "donald", "gary", "jill", "hillary" };
int m = rotateLeft(running, 5, 1);  // returns 1
    // running now contains:  "evan", "gary", "jill", "hillary", "donald"*/
int countRuns(const string a[], int n);
/*
Return the number of sequences of one or more consecutive identical items in a.
string d[9] = {
    "tim", "ajamu", "mike", "mike", "donald", "donald", "donald", "mike", "mike"
};
int p = countRuns(d, 9);  //  returns 5
	   //  The five sequences of consecutive identical items are
	   //    "tim"
	   //    "ajamu"
	   //    "mike", "mike"
	   //    "donald", "donald", "donald"
	   //    "mike", "mike"*/
int flip(string a[], int n);
/*
Reverse the order of the elements of the array and return n. For example,
string folks[6] = { "ajamu", "mike", "", "tim", "mindy", "bill" };
int q = flip(folks, 4);  // returns 4
    // folks now contains:  "tim"  ""  "mike"  "ajamu"  "mindy"  "bill"*/
int differ(const string a1[], int n1, const string a2[], int n2);
/*
Return the position of the first corresponding elements of a1 and a2 that are not equal. n1 is the number of interesting elements in a1, and n2 is the number of interesting elements in a2. If the arrays are equal up to the point where one or both runs out, return whichever value of n1 and n2 is less than or equal to the other. For example,
string folks[6] = { "ajamu", "mike", "", "tim", "mindy", "bill" };
string group[5] = { "ajamu", "mike", "bill", "", "tim" };
int r = differ(folks, 6, group, 5);  //  returns 2
int s = differ(folks, 2, group, 1);  //  returns 1*/
int subsequence(const string a1[], int n1, const string a2[], int n2);
/*
If all n2 elements of a2 appear in a1, consecutively and in the same order, then return the position in a1 where that subsequence begins. If the subsequence appears more than once in a1, return the smallest such beginning position in the array. Return -1 if a1 does not contain a2 as a contiguous subsequence. (Consider a sequence of 0 elements to be a subsequence of any sequence, even one with no elements, starting at position 0.) For example,
string names[10] = { "evan", "hillary", "mindy", "jill", "ajamu", "gary" };
string names1[10] = { "hillary", "mindy", "jill" };
int t = subsequence(names, 6, names1, 3);  // returns 1
string names2[10] = { "evan", "jill" };
int u = subsequence(names, 5, names2, 2);  // returns -1*/
int lookupAny(const string a1[], int n1, const string a2[], int n2);
/*
Return the smallest position in a1 of an element that is equal to any of the elements in a2. Return -1 if no element of a1 is equal to any element of a2. For example,
string names[10] = { "evan", "hillary", "mindy", "jill", "ajamu", "gary" };
string set1[10] = { "bill", "ajamu", "jill", "hillary" };
int v = lookupAny(names, 6, set1, 4);  // returns 1 (a1 has "hillary" there)
string set2[10] = { "tim", "donald" };
int w = lookupAny(names, 6, set2, 2);  // returns -1 (a1 has none)*/
int separate(string a[], int n, string separator);
/*
Rearrange the elements of the array so that all the elements whose value is < separator come before all the other elements, and all the elements whose value is > separator come after all the other elements. Return the position of the first element that, after the rearrangement, is not < separator, or n if there are no such elements. For example,
string cand[6] = { "donald", "jill", "hillary", "tim", "evan", "bill" };
int x = separate(cand, 6, "gary");  //  returns 3
	// cand must now be
	//      "donald"  "evan"  "bill"  "jill"  "tim"  "hillary"
	// or   "evan"  "bill"  "donald"  "hillary"  "jill"  "tim"
	// or one of several other orderings.
	// All elements < "gary" (i.e., "evan", "bill", and "donald")
	//   come before all others
	// All elements > "gary" (i.e., "tim", "jill", and "hillary")
	//   come after all others
string cand2[4] = { "gary", "hillary", "jill", "donald" };
int y = separate(cand2, 4, "hillary");  //  returns 2
	// cand2 must now be either
	//      "gary"  "donald"  "hillary"  "jill"
	// or   "donald"  "gary"  "hillary"  "jill"
	// All elements < "hillary" (i.e., "donald" and "gary") come
        // before all others.
	// All elements > "hillary" (i.e., "jill") come after all others.*/

int appendToAll(string a[], int n, string value)
{
	if(n<0)
		return -1;
	for(int i=0;i<n;i++)
		a[i]+=value;
	return n;
}
int lookup(const string a[], int n, string target)
{
	for(int i=0;i<n;i++)
		if(a[i]==target)
			return i;
	return -1;
}
int positionOfMax(const string a[], int n)
{
	if(n<1)
		return -1;//must have at least 1 element
	int max = 0;
	for(int i=0;i<n;i++)
		if(a[i]>a[max])//if string at index i is greater than the string at the index max, replace max with i
			max=i;
	return max;
}
int rotateLeft(string a[], int n, int pos)
{
	if(pos>=n || n<0 || pos<0)
		return -1;
	string toPushBack = a[pos];//record string to be rotated
	for(int i=pos;i<n-1;i++)//make every element starting from pos except the last equal to the next element
		a[i]=a[i+1];
	a[n-1]=toPushBack;//make the last element the recorded string
	return pos;
}
int countRuns(const string a[], int n)
{
	if(n<0)
		return -1;
	int runs = 0;
	string prev;
	for(int i=0;i<n;i++)
		if (i==0 || a[i]!=prev)//will initialize prev upon first loop
		{//if the current element is the first element or is different from the prev, it is a new run
			runs++;
			prev = a[i];//set prev to the current element
		}
	return runs;
}
int flip(string a[], int n)
{
	if(n<0)
		return -1;
	int front = 0;
	int end = n-1;
	while (front<end)
	{
		string toEnd=a[front];//swaps the front and the end
		a[front]=a[end];
		a[end]=toEnd;
		front++;//moves front forward one
		end--;//moves end back one
	}//continues swapping until the front is greater than or equal to the end, meaning the whole array is swapped
	return n;
}
int differ(const string a1[], int n1, const string a2[], int n2)
{
	if(n1<0||n2<0)
		return -1;
	int n = n1;
	if(n2<n1)//sets n to be the least of n1 and n2
		n=n2;
	for(int i=0;i<n;i++)
		if(a1[i]!=a2[i])//if a1 and a2 differ, returns the index of where they differ
			return i;
	return n;//otherwise returns the smallest of n1 and n2
}
int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if(n1<0||n2<0||n2>n1)
		return -1;
	if(n2==0)//if n2 is zero, then consider it the empty array which has a subsequence at 0
		return 0;
	for(int i=0;i<n1-n2+1;i++)//i goes from 0 to n1-n2 to prevent going out of bounds
		for(int j=0;j<n2;j++)//j goes from 0 to n2-1
		{
			if(a1[i+j]!=a2[j])//breaks the inner loop if the substring of a1 starting at i does not match a2
				break;
			if(j==n2-1)//if every element is equal, j will reach this point
				return i;//return where the substring in a1 starts
		}
	return -1;
}
int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if(n1<0||n2<0)
		return -1;
	for(int i=0;i<n1;i++)
		for(int j=0;j<n2;j++)
			if(a1[i]==a2[j])//for each value in a1, checks if it is in a2. If it is, return the index of the value in a1
				return i;
	return -1;
}
int separate(string a[], int n, string separator)
{
	if(n<0)
		return -1;
	if(n==0)//empty array, then it is separated already and there are no elements >= separator so return n, which is 0
		return 0;
	int front = 0;
	int end = n-1;
	while(front<end)//if front >= end, then all elements < separator are at beginning
	{
		while(a[front]<separator && front<end)//moves front to the index of the first element >= separator
			front++;
		while(a[end]>=separator && front<end)//moves end to the index of the last element < separator
			end--;
		string toEnd=a[front];//swaps elements at front and end
		a[front]=a[end];
		a[end]=toEnd;
	}
	int i;
	for(i=0;i<n;i++)
		if(a[i]>=separator)//stops i at first element that is >= separator
			break;
	front = i;//set front to the index of first element >= separator
	end = n-1;
	while(front<end)//loop through again, moving elements equal to separator before elements >separator
	{
		while(a[front]<=separator && front<end)
			front++;
		while(a[end]>separator && front<end)
			end--;
		string toEnd=a[front];
		a[front]=a[end];
		a[end]=toEnd;
	}
	return i;
}
int main()
{
	/*string h[7] = { "jill", "hillary", "donald", "tim", "", "evan", "gary" };
	    assert(lookup(h, 7, "evan") == 5);
	    assert(lookup(h, 7, "donald") == 2);
	    assert(lookup(h, 2, "donald") == -1);
	    assert(positionOfMax(h, 7) == 3);

	    string g[4] = { "jill", "hillary", "gary", "mindy" };
	    assert(differ(h, 4, g, 4) == 2);
	    assert(appendToAll(g, 4, "?") == 4 && g[0] == "jill?" && g[3] == "mindy?");
	    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "gary?" && g[3] == "hillary?");

	    string e[4] = { "donald", "tim", "", "evan" };
	    assert(subsequence(h, 5, e, 4) == -1);

	    string d[5] = { "hillary", "hillary", "hillary", "tim", "tim" };
	    assert(countRuns(d, 4) ==2);
	
	    string f[3] = { "gary", "donald", "mike" };
	    assert(lookupAny(h, 7, f, 3) == 2);
	    assert(flip(f, 3) == 3 && f[0] == "mike" && f[2] == "gary");
	
	    assert(separate(h, 7, "gary") == 3);*/
		string a[3]={"a","b","c"};
		assert(appendToAll(a,2," ")==2 && a[1]=="b " && a[2]=="c");
		assert(appendToAll(a,-1,"aoeuaoeu")==-1);
		
		string b[3]={"a","b","c"};
		assert(lookup(b,3,"c")==2 && lookup(b,2,"c")==-1 && lookup(b,-1,"c")==-1);
		
		assert(positionOfMax(b,3)==2 && positionOfMax(b,0)==-1 && positionOfMax(b,-1)==-1);
		
		assert(rotateLeft(b,0,0)==-1 && rotateLeft(b,3,-1)==-1 && rotateLeft(b,-1,1)==-1 && rotateLeft(b,3,0)==0);
		assert(b[0]=="b" && b[1]=="c" && b[2]=="a");
		
		string c[10]={"a","a","a","a","c","a","b","b","b","c"};
		assert(countRuns(b,0)==0 && countRuns(b,-1)==-1 && countRuns(b,3)==3 && countRuns (c,10)==5);
		
		assert(flip(c,0)==0 && flip(c,-1)==-1 && flip(c,10)==10);
		assert(c[0]=="c" && c[1]=="b" && c[2]=="b" && c[3]=="b" && c[4]=="a" && c[5]=="c" && c[6]=="a" && c[7]=="a" && c[8]=="a" && c[9]=="a");
		
		string d[5]={"a","b","c","d","e"};
		string e[3]={"a","b","d"};
		assert(differ(d,-1,e,3)==-1 && differ(d,5,e,-1)==-1 && differ(d,5,e,3)==2 && differ(d,5,e,1)==1);
		
		string f[5]={"a","b","c","b","c"};
		string g[2]={"b","c"};
		assert(subsequence(f,-1,g,0)==-1 && subsequence(f,0,g,-1)==-1 && subsequence(f,3,g,0)==0);
		assert(subsequence(f,2,g,2)==-1 && subsequence(f,5,g,2)==1);
		
		assert(lookupAny(f,-1,g,0)==-1 && lookupAny(f,0,g,-1)==-1);
		assert(lookupAny(f,1,g,2)==-1 && lookupAny(f,5,g,2)==1);
		
		assert(separate(f,-1,"b")==-1 && separate (f,0,"b")==0);
		assert(separate(f,5,"b")==1);
		assert(f[0]=="a" && f[1]=="b"  && f[2]=="b" && f[3]=="c" && f[4]=="c");
		
	    cout << "All tests succeeded" << endl;
}