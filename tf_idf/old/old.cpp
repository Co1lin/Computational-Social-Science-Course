//
//  main.cpp
//  t1
//
//  Created by Colin on 2019/11/11.
//  Copyright © 2019 Colin. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <queue>
#include <set>
#include <fstream>
using namespace std;

struct word
{
	int tot, poem;
	double tf, idf, tf_idf;
};
struct top_ele
{
	map<string, word>::iterator iter;
	short list_type;
	friend bool operator < (const top_ele &x, const top_ele &y)
	{
		if (x.list_type == 1)
			return x.iter->second.tf > y.iter->second.tf;
		
		else if (x.list_type == 2)
			return x.iter->second.idf > y.iter->second.idf;
		else
			return x.iter->second.tf_idf > y.iter->second.tf_idf;
	}
	bool operator == (const top_ele& y)
	{
		return iter == y.iter;
	}
	void output() const
	{
		printf("%s", (*iter).first.c_str());
		if (list_type == 1)
			printf(" (%f), ", (*iter).second.tf);
		else if (list_type == 2)
			printf(" (%f), ", (*iter).second.idf);
		else
			printf(" (%f), ", (*iter).second.tf_idf);
	}
};

const int top_num = 6;
map<string, word> mydata;
int poems = 0;
int tot_words = 0;
int tot_diff_words = 0;
multiset<top_ele> top_list_tf;
multiset<top_ele> top_list_idf;
multiset<top_ele> top_list_tf_idf;

void mycount()
{
	ifstream in("data.in");
	string test;
	while (getline(in, test))
	{
		poems++;
		for (int i = 1; i <= test.length(); i++)
		{
			if (test[i] == ' ' || test[i] == '\0')
			{
				tot_words++;
			}//end if;
		}//end for
		//tot_words += word_count_in_this_poem;
	}//end while
	in.close();
}

void del_the_same(const map<string, word>::iterator &it, const int &list_type)
{
	set<top_ele>::iterator it_out;
	if (list_type == 1)
	{
		for (it_out = top_list_tf.begin(); it_out != top_list_tf.end(); it_out++)
			if (it_out->iter == it)
			{
				top_list_tf.erase(it_out);
				break;
			}
	}
	else if (list_type == 2)
	{
		for (it_out = top_list_idf.begin(); it_out != top_list_idf.end(); it_out++)
			if (it_out->iter == it)
			{
				top_list_idf.erase(it_out);
				break;
			}
	}
	else
	{
		for (it_out = top_list_tf_idf.begin(); it_out != top_list_tf_idf.end(); it_out++)
			if (it_out->iter == it)
			{
				top_list_tf_idf.erase(it_out);
				break;
			}
	}
}

int main()
{
	mycount();
	ifstream in("data.in");
	string test;
	while (getline(in, test))
	{
		//poems++;
		//int word_count_in_this_poem = 0;
		int before = 0; //last space + 1
		map<string, bool> words_in_this_poem;
		for (int i = 1; i <= test.length(); i++)
		{
			if (test[i] == ' ' || test[i] == '\0')
			{
				//word_count_in_this_poem++;
				//tot_words++;
				string thisword = test.substr(before, i - before);
				if (thisword == "无")
					printf("FUCK WU!\n");
				map<string, word>::iterator it;
				it = mydata.find(thisword);
				if (it != mydata.end()) //found; word exist already
				{
					//keep the top_list
					for (int j = 1; j <= 3; j++)
						del_the_same(it, j);
					//count
					(*it).second.tot++;
					if (!words_in_this_poem[thisword])
						(*it).second.poem++;
					words_in_this_poem[thisword] = true;
				}
				else    //can't find; a new word
				{
					tot_diff_words++;
					mydata[thisword] = { 1, 1 };
					it = mydata.find(thisword);
					words_in_this_poem[thisword] = true;
				}
				before = i + 1;
				//calculate(update) the statistics
				(*it).second.tf = (*it).second.tot / (double)tot_words;
				(*it).second.idf = log10((double)poems / ((*it).second.poem + 1));
				(*it).second.tf_idf = (*it).second.tf * (*it).second.idf;
				//keep the top_list
				top_list_tf.insert({ it, 1 });
				top_list_idf.insert({ it, 2 });
				top_list_tf_idf.insert({ it, 3 });
				multiset<top_ele>::iterator last;
				last = top_list_tf.end();
				last--;
				if (top_list_tf.size() >= top_num)
					top_list_tf.erase(last);
				last = top_list_idf.end();
				last--;
				if (top_list_idf.size() >= top_num)
					top_list_idf.erase(last);
				last = top_list_tf_idf.end();
				last--;
				if (top_list_tf_idf.size() >= top_num)
					top_list_tf_idf.erase(last);

			}//end if;
		}//end for
		//tot_words += word_count_in_this_poem;
	}//end while

	//output
	freopen("out.out", "w", stdout);
	printf("----OUTPUT----\n\n");
	printf("  Info: %d words, %d different words, %d poems\n\n", tot_words, tot_diff_words, poems);
	printf("  Top List:\n\n");
	//top_ele out;
	printf("    TF    : ");
	set<top_ele>::iterator it_out;
	for (it_out = top_list_tf.begin(); it_out != top_list_tf.end(); it_out++)
		(*it_out).output();
	printf("\n");
	printf("    IDF   : ");
	for (it_out = top_list_idf.begin(); it_out != top_list_idf.end(); it_out++)
		(*it_out).output();
	printf("\n");
	printf("    TF_IDF: ");
	for (it_out = top_list_tf_idf.begin(); it_out != top_list_tf_idf.end(); it_out++)
		(*it_out).output();
	printf("\n\n");

	in.close();
	fclose(stdout);

	return 0;
}