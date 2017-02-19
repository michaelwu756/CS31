#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<cctype>
//#include<iostream>
//#include<cassert>

using namespace std;

const int MAX_WORD_LENGTH = 20;
int normalizeRules(char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int distance[], int nRules);
int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1], const char word2[][MAX_WORD_LENGTH+1], const int distance[], int nRules, const char document[]);

struct Rule
{
	public:
		Rule(const char w1[],const char w2[],int d)
		{
			strcpy(word1,w1);
			strcpy(word2,w2);
			distance=d;
			next=nullptr;
			cstrToLower(word1);//make words lowercase
			cstrToLower(word2);
			char tempWord[MAX_WORD_LENGTH+1];
			if(strcmp(word1,word2)>0)//put the words in alphabetical order
			{
				strcpy(tempWord,word1);
				strcpy(word1,word2);
				strcpy(word2,tempWord);
			}//does not matter what order w1 or w2 are in, will always result in same data stored so it's easy to check for duplicates
		}
		char word1[MAX_WORD_LENGTH+1];
		char word2[MAX_WORD_LENGTH+1];
		int distance;
		Rule* next;
	
	private:
		void cstrToLower(char str[])
		{
			for(int i=0; str[i]!='\0';i++)
			{
				str[i]=tolower(str[i]);
			}
		}
};
class RuleList
{
	public:
		RuleList()
		{
			head=nullptr;
		}
		RuleList(const char word1[][MAX_WORD_LENGTH+1],const char word2[][MAX_WORD_LENGTH+1], const int distance[], int nRules)
		{
			head = nullptr;
			for(int i=0;i<nRules;i++)//go through every rule
			{
				if(validWord(word1[i]) && validWord(word2[i]) && distance[i]>0)//must have valid words and positive distance
					add(word1[i],word2[i],distance[i]);//add valid rules
			}
		}
		~RuleList()//destructor to prevent memory leaks
		{
			Rule* h=head;
			Rule* prev=nullptr;
			while(h!=nullptr)
			{
				prev = h;
				h=h->next;
				delete prev;
			}
		}
		void add(Rule* r)
		{
			Rule* h=head;
			if(h==nullptr)//if list is empty, set head node
				head=r;
			else
			{
				while(h->next!=nullptr)//otherwise go to end of list
					h=h->next;
				h->next=r;//link the end of list to point to the added node
			}
		}
		void add(const char w1[],const char w2[],int d)
		{
			Rule* r = new Rule(w1,w2,d);
			add(r);
		}
		void remove(Rule* r)
		{
			if(r==nullptr)
				return;
			Rule* h=head;
			Rule* prev=nullptr;
			if(r==head)//if the head node is deleted, set the new head node to be the second node
			{
				head=r->next;
				delete h;
			}
			else
			{
				while(h!=nullptr && h!=r)//search for the node r
				{
					prev = h;
					h=h->next;
				}
				if(h==r)//if the node is found
				{
					prev->next=h->next;//link the previous node to the node after r
					delete h;//delete r
				}
			}
		}
		int length()//counts nodes to get length
		{
			int length = 0;
			Rule* h = head;
			while(h!=nullptr)
			{
				h=h->next;
				length++;
			}
			return length;
		}
		void outputArrays(char word1[][MAX_WORD_LENGTH+1],char word2[][MAX_WORD_LENGTH+1], int distance[])
		{
			int index = 0;
			Rule* h = head;
			while(h!=nullptr)//for each rule, copy into the array of cstrings and array of distances
			{
				strcpy(word1[index],h->word1);
				strcpy(word2[index],h->word2);
				distance[index]=h->distance;
				h=h->next;
				index++;
			}
		}
		void removeDuplicates()
		{
			Rule* h=head;
			while(h!=nullptr)//traverse list in outer loop with h
			{
				Rule* i = head;
				while(i!=nullptr)//traverse list with inner loop with i
				{
					if(i!=h && strcmp(i->word1,h->word1)==0 && strcmp(i->word2,h->word2)==0 && i->distance<=h->distance)
					{//if rules are duplicates, remove the rule i if distance in i is less than or equal to distance in h
						Rule* prev = i;
						i = i->next;
						remove(prev);
					}
					else
						i=i->next;
				}
				h=h->next;//because h will visit each node that is not removed in the inner loop, no duplicates will remain and only the one with the highest distance will remain
			}
		}
		int satisfaction(const char document[])
		{
			int score=0;
			char lowerCaseDoc[201];
			char docWords[100][201];//never more than 100 words in a document, a word is never more than 200 char long
			for(int i = 0;i<100;i++)
			{
				docWords[i][0]='\0';
			}
			removeNonAlphaAndConvertToLowerCase(lowerCaseDoc,document);
			separateWords(docWords,lowerCaseDoc);//create an array of every word
			Rule* h = head;
			while(h!=nullptr)//go through every rule
			{
				bool ruleSatisfied=false;
				for(int i=0;i<100;i++)//check every word in order
				{
					if(docWords[i][0]=='\0')//if a word is empty, then ther are no more words in the array
						break;
					if(strcmp(docWords[i],h->word1)==0)//if the word at i matches the first word
					{
						int begin=i-h->distance;
						int end=i+h->distance+1;
						if(begin<0)
							begin=0;
						if(end>100)
							end=100;
						for(int j=begin;j<end;j++)//check within the rule's distance before and after the first word
						{
							if(strcmp(docWords[j],h->word2)==0 && i!=j)//if the second word is found and distance is not 0
							{
								score++;//add one to satisfaction
								ruleSatisfied=true;//stop looking for more matches
								break;
							}
						}
					}
					if(ruleSatisfied)//if the rule is satisfied, stop looking and move to next rule
						break;
				}
				h=h->next;
			}
			return score;
		}
		
	private:
		RuleList(const RuleList& other);//prevent copy constructor from being used
		RuleList& operator= (const RuleList& other);//prevent assignment operator from being used
		Rule* head;
		
		bool validWord(const char str[])//returns false for empty words and words that are not all alphabetical
		{
			if(str[0]=='\0')
				return false;
			for(int i=0; str[i]!='\0';i++)
			{
				if(!isalpha(str[i]))
					return false;
			}
			return true;
		}
		void separateWords(char result[][201], const char document[])//separates words by spaces
		{
			char word[201];
			int i = 0;//character index in document
			int j = 0;//index of the word in result
			int k = 0;//character index in the current word
			while(document[i]!='\0')//go through every character in document
			{
				if(document[i]==' ')//if character is a space, store the previous word
				{
					word[k]='\0';//make the character after the end of the previous word the terminating character
					if(word[0]!='\0')//do not store empty words
					{
						strcpy(result[j],word);//copy the word as the next word in the array
						j++;//make j the index of the next empty spot in the array
					}
					k=0;//make the index 0 in order to overwrite the previous word and start a new one
				}
				else//character is not a space,
				{
					word[k]=document[i];//record it in character array
					k++;//move to the next character
				}
				i++;//move to next character in document
			}
			word[k]='\0';
			if(word[0]!='\0')
				strcpy(result[j],word);//store the last word if it is not empty
		}
		void removeNonAlphaAndConvertToLowerCase(char result[], const char document[])
		{
			int i = 0;//index of character in document
			int j = 0;//index of character in result
			while(document[i]!='\0')
			{
				if(isalpha(document[i]) || document[i]==' ')//only record alphabetical characters and spaces
				{
					result[j]=tolower(document[i]);//copy character as lowercase
					j++;//increment the characters in result
				}
				i++;//increment the character in document
			}
			result[j]='\0';
		}
};

int normalizeRules(char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int distance[], int nRules)
{
	RuleList list(word1,word2,distance,nRules);
	list.removeDuplicates();
	list.outputArrays(word1,word2,distance);
	return list.length();
}
				   
int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH+1], const char word2[][MAX_WORD_LENGTH+1], const int distance[], int nRules, const char document[])
{
	RuleList list(word1,word2,distance,nRules);
	return list.satisfaction(document);
}

/*
void output(const char word1[][MAX_WORD_LENGTH+1], const char word2[][MAX_WORD_LENGTH+1], const int distance[], int nRules)
{
	for(int i=0;i<nRules;i++)
		cout<<word1[i]<<" "<<word2[i]<<" "<<distance[i]<<endl;
}
void test(int n)
{
	switch(n)
	{
		case 0:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           500
			};
			int n=normalizeRules(word1,word2,distance,-1);
			assert(n==0);
			output(word1,word2,distance,nRules);
		}
		break;
		case 1:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           500
			};
			n=normalizeRules(word1,word2,distance,0);
			assert(n==0);
			output(word1,word2,distance,n);
		}
		break;
		case 2:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           500
			};
			n=normalizeRules(word1,word2,distance,nRules);
			assert(n==3);
			output(word1,word2,distance,n);
		}
		break;
		case 3:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot!",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           500
			};
			n=normalizeRules(word1,word2,distance,nRules);
			assert(n==3);
			output(word1,word2,distance,n);
		}
		break;
		case 4:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           0,          -1,           500
			};
			n=normalizeRules(word1,word2,distance,nRules);
			assert(n==2);
			output(word1,word2,distance,n);
		}
		break;
		case 5:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "scientist", "mad", "scientist"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "mad",    "scientist",      "mad"
			};
			int distance[nRules] = {
				2,           4,          500,           500
			};
			n=normalizeRules(word1,word2,distance,nRules);
			assert(n==1);
			assert(distance[0]==500);
			output(word1,word2,distance,n);
		}
		break;
		case 6:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"MAd",       "sciEntist", "maD", "SCIENTIST"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "mad",    "scientist",      "MAD"
			};
			int distance[nRules] = {
				2,           4,          500,           500
			};
			n=normalizeRules(word1,word2,distance,nRules);
			assert(n==1);
			assert(distance[0]==500);
			output(word1,word2,distance,n);
		}
		break;
		case 7:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"MAd",       "sci Entist", "maD", "SCIENT-IST"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scient3ist", "mad",    "",      "MAD"
			};
			int distance[nRules] = {
				2,           4,          500,           500
			};
			n=normalizeRules(word1,word2,distance,nRules);
			assert(n==0);
			output(word1,word2,distance,n);
		}
		break;
		case 8:
		{
			const int nRules = 1;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"mad"
			};
			int distance[nRules] = {
				1
			};
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad mad")==1);
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad a")==0);
			output(word1,word2,distance,nRules);
		}
		break;
		case 9:
		{
			const int nRules = 1;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist"
			};
			int distance[nRules] = {
				1
			};
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad scientist")==1);
			assert(calculateSatisfaction(word1,word2,distance,nRules,"scientist mad")==1);
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad a scientist")==0);
			assert(calculateSatisfaction(word1,word2,distance,nRules,"scientist a mad")==0);
			output(word1,word2,distance,nRules);
		}
		break;
		case 10:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           13
			};
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad have scientist")==2);
			output(word1,word2,distance,nRules);
		}
		break;
		case 11:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           13
			};
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad mad scientist mad")==1);
			output(word1,word2,distance,nRules);
		}
		break;
		case 12:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           13
			};
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad MAD      scieNtist        mad")==1);
			output(word1,word2,distance,nRules);
		}
		break;
		case 13:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           13
			};
			assert(calculateSatisfaction(word1,word2,distance,nRules,"mad MA32423423D      scieNtist   h^^&^ave     ma44d")==2);
			output(word1,word2,distance,nRules);
		}
		break;
		case 14:
		{
			const int nRules = 4;
			char word1[nRules][MAX_WORD_LENGTH+1] = {
				"mad",       "deranged", "nefarious", "have"
			};
			char word2[nRules][MAX_WORD_LENGTH+1] = {
				"scientist", "robot",    "plot",      "mad"
			};
			int distance[nRules] = {
				2,           4,          1,           13
			};
			assert(calculateSatisfaction(word1,word2,distance,-1,"mad scientist")==0);
			assert(calculateSatisfaction(word1,word2,distance,0,"mad scientist")==0);
			output(word1,word2,distance,nRules);
		}
		break;
	}
	cout<<endl;
}
*/

int main()
{
	/*
	for(int i=0;i<15;i++)
	{
		cout<<i<<endl;
		test(i);
	}
	cout << "All tests succeeded" << endl;
	*/
}