#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include <math.h>
#include <time.h>



using namespace std;


class Attribute{
	public:
	double countP;
	double countN;
	double probP;
	double probN;

	Attribute()
	{
		countP=0;
		countN=0;
		probP=0;
		probN=0;
	}

	Attribute operator= (Attribute & param)
	{
		Attribute x;
		x.countP=param.countP;
		x.countN=param.countN;
		x.probP=param.probP;
		x.probN=param.probN;
		return x;
	}

};

class Tuple
{
	public:
	int label;
	vector<int> feature;
	Tuple()
	{
		label=0;
	}
};

class Classifier
{	
	public:
	vector<Tuple  * > tupleSet;	
	vector< map<int,Attribute> > featureStats;
	unsigned countPositive;
	unsigned countNegative;
	
	Classifier()
	{
		countPositive=0;
		countNegative=0;
	}
	
	
	void UpdateFeatureStats()
	{

		featureStats.resize( tupleSet[0]->feature.size());
		
		for (unsigned i=0;i<tupleSet.size();i++)
		{
			for (unsigned j=0;j<tupleSet[i]->feature.size();j++)
			{

				//if not find
				if ( featureStats[j].find(tupleSet[i]->feature[j])==featureStats[j].end()) 
				{	
					Attribute x;
					featureStats[j][tupleSet[i]->feature[j]]=x;
				}
				
				if (tupleSet[i]->label==1)
				{
					featureStats[j][tupleSet[i]->feature[j]].countP++;
					countPositive++;
				}
				else if (tupleSet[i]->label==-1)
				{
					featureStats[j][tupleSet[i]->feature[j]].countN++;
					countNegative++;
				}
				else
				{
					cout<<"Wrong Label"<<endl;
				}
				
			}
		}
		
		
		
		for (unsigned i=0;i<featureStats.size();i++)
		{
			map<int,Attribute>::iterator it;
			for (it=featureStats[i].begin(); it!=featureStats[i].end();it++)
	  		{
	  			it->second.probN=it->second.countN/(countNegative);
	  			it->second.probP=it->second.countP/(countPositive);
	  		}
		}		
	}
	
	int Judge(Tuple * test)
	{
		double posteriorP=(double) countPositive/ (double)(countPositive+countNegative);
		double posteriorN=1-posteriorP;
	
		for (unsigned i=0;i<test->feature.size();i++)
		{
//			Attribute temp=featureStats[i][tuple->feature[i]];
		
			if (featureStats[i][test->feature[i]].probP==0)
			{
				featureStats[i][test->feature[i]].probP=1/((double) (countPositive+countNegative));
	//			cout<<"correted P"<<endl;
			}
		
			if (featureStats[i][test->feature[i]].probN==0)
			{
				featureStats[i][test->feature[i]].probN=1/((double) (countPositive+countNegative));
	//			cout<<"correted N"<<endl;
			}
		
		
		
			posteriorP=posteriorP*featureStats[i][test->feature[i]].probP;
			posteriorN=posteriorN*featureStats[i][test->feature[i]].probN;
		}
		
		if (posteriorN==0 || posteriorP==0)
		{
			cout<<"posterior error"<<endl;
		}
		
		if (posteriorN>posteriorP)
		{
			return -1;
		}
		else if (posteriorN<posteriorP)
		{
			return 1;
		}
		else
		{
			cout<<"same probablitly, return 0"<<endl;
			return 0;
		}
		
		
	}
};




void RawtoTuple(string const & rawTuple, Tuple * const tuple)
{

	string num;
	vector<int> tempdata;

	for (unsigned i=0; i<rawTuple.size();i++)
	  {
	  		if (rawTuple[i]!='\t')
	  		{
	  			num.push_back(rawTuple[i]);
	  		}
	  		else
	  		{
	  			tempdata.push_back( atoi(num.c_str()) );
	  			num.clear();
	  		}
	  				
	  				
	  }
	  			
	  			if (!rawTuple.empty())
	  			{
	  			tempdata.push_back( atoi(num.c_str()) );
	  			num.clear(); 			
				}
	
	if (tempdata.size()==0)
	{
		cout<<"no temp data"<<endl;
	}
	else
	{
		tuple->label=tempdata[0];
	
		tuple->feature.resize(tempdata.size() -1);
	
		for (unsigned i=1;i<tempdata.size();i++)
		{
			tuple->feature[i-1]=tempdata[i];
		}
	}
}






int main(int argc, char *argv[])
{
 
  vector<int> tuple;
  

  


  

//  string num;

  Classifier D; //original trained set, consists the original training data
  ifstream myfile(argv[1]);
  if (!myfile.is_open())
  
  {
  	cout<<"Fail to open file"<<endl;
  }
  
  
  	//read the file and parse into attributes
  	while(myfile.good())
  	{
		string rawTuple;
		
  		getline(myfile,rawTuple); //get line into raw tuple 
  	
  		Tuple * tupleptr = new Tuple;
  		
  		RawtoTuple(rawTuple, tupleptr); 		
  		
  		if (tupleptr->feature.size()==0)
  		{
  			delete tupleptr;
  			break;
  		}
  		D.tupleSet.push_back(tupleptr);
  		
  	}
  	
  	myfile.close();
  	
  D.UpdateFeatureStats();
  
  //----------finished processing Traning Data
  
  
  Classifier testData;
  
  
  myfile.open(argv[2]);
  if (!myfile.is_open())
  
  {
  	cout<<"Fail to open file"<<endl;
  }
  
  
  	//read the file and parse into attributes
  	while(myfile.good())
  	{
		string rawTuple;
		
  		getline(myfile,rawTuple); //get line into raw tuple 
  	
  		Tuple * tupleptr = new Tuple;
  		
  		RawtoTuple(rawTuple, tupleptr); 		
  		
  		if (tupleptr->feature.size()==0)
  		{
  			delete tupleptr;
  			break;
  		}
  		testData.tupleSet.push_back(tupleptr);
  	}
  	
  	myfile.close();
  	
  	unsigned tp=0;
  	unsigned tn=0;
  	unsigned fp=0;
  	unsigned fn=0;
  	
  	for (unsigned i=0;i<testData.tupleSet.size();i++)
  	{
  		int judgement=D.Judge(testData.tupleSet[i]);
  		
  		if (judgement==1 && testData.tupleSet[i]->label==1)
  		{
  			tp++;
  		}
  		else if (judgement==1 && testData.tupleSet[i]->label==-1)
  		{
  			fp++;
  		}
  		else if (judgement==-1 && testData.tupleSet[i]->label==1)
  		{
  			fn++;
  		}
  		else if (judgement==-1 && testData.tupleSet[i]->label==-1)
  		{
  			tn++;
  		}
  		else
  		{
  			cout<<"label judgement mismatch"<<endl;
  		}
  	}
  	
  	  printf("Naive Bayes:\ntrue positive: %d\nfalse negative: %d\nfalse positive: %d\ntrue negative: %d\n", tp, fn, fp, tn);
  	  
  	  
	//---------
	//END OF NAIVE BAYES
	//--------
 


	for (unsigned i=0;i< D.tupleSet.size();i++)
	{
		delete D.tupleSet[i];
	}
	for (unsigned i=0;i< testData.tupleSet.size();i++)
	{
		delete testData.tupleSet[i];
	}


  return 0;

  }
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
