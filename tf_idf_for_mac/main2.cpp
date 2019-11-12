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
#include <ctime>
#include <map>
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
    friend bool operator < (const top_ele& x, const top_ele& y)
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

int top_num = 6;
map<string, word> mydata;
int poems = 0;
int tot_words = 0;
int tot_diff_words = 0;
multiset<top_ele> top_list_tf;
multiset<top_ele> top_list_idf;
multiset<top_ele> top_list_tf_idf;

short s2num(const string& s)
{
    short ret = 0;
    short ten = 1;
    for (int i = s.length() - 1; i >= 0; i--)
    {
        ret += (s[i] - '0') * ten;
        ten *= 10;
    }
    return ret;
}
void del_the_same(const map<string, word>::iterator& it, const int& list_type);
void pre_output();
void read_data();
void calculate();
void res_output();

int main()
{
    clock_t start_time, end_time;
    start_time = clock();
    pre_output();
    read_data();
    calculate();
    res_output();
    end_time = clock();
    double tot_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("--------\n\n");
    printf("  Run time: %f s\n\n", tot_time);
    printf("----PROGRAM COMPLETED----\n");
    fclose(stdout);
    return 0;
}

void pre_output()
{
    printf("----INPUT----\n\n");
    printf("  Notice: the word list should be placed at the same directory with this program.\n");
    printf("  Please input the size of rank list\n");
    printf("  (enter to use default value 6;\n   longer time will be used if the number is larger):\n");
    printf("  the size of rank list = ");
    string input_num;
    getline(cin, input_num);
    if (input_num == "")
    {
        printf("  entered; use default value 6\n");
    }
    else
    {
        if (input_num.length() <= 4 || input_num.length() == 5 && input_num <= "32767")
        {
            top_num = s2num(input_num);
            printf("  (read \"%hd\")\n", top_num);
        }
        else
        {
            printf("  Invalid input (May be too large). Use default value.\n");
        }
    }
    printf("\n\n");
}

void del_the_same(const map<string, word>::iterator& it, const int& list_type)
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

void read_data()
{
    ifstream in("data.in");
    string test;
    printf("----READ DATA----\n\n");
    while (getline(in, test))
    {
        poems++;
        //int word_count_in_this_poem = 0;
        int before = 0; //last space + 1
        map<string, bool> words_in_this_poem;
        for (int i = 1; i <= test.length(); i++)
        {
            if (test[i] == ' ' || test[i] == '\0')
            {
                //word_count_in_this_poem++;
                string thisword = test.substr(before, i - before);
                //check the word
                if (thisword[0] == '?' || '0' <= thisword[0] && thisword[0] <= '9')
                    continue;
                tot_words++;
                map<string, word>::iterator it;
                it = mydata.find(thisword);
                if (it != mydata.end()) //found; word exist already
                {
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
                    //it = mydata.find(thisword);
                    words_in_this_poem[thisword] = true;
                }
                before = i + 1;
            }//end if;
        }//end for
        if (poems % 1000 == 0)
        {
            printf("  read about %d poems, %d words, %d different words\n", poems, tot_words, tot_diff_words);
        }
    }//end while
    in.close();
    printf("\n\n");
}

void calculate()
{
    printf("----CALCULATE----\n\n");
    map<string, word>::iterator it;
    int cc = 0;
    for (it = mydata.begin(); it != mydata.end(); it++)
    {
        cc++;
        for (int j = 1; j <= 3; j++)
            del_the_same(it, j);
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
        if (top_list_tf.size() >= top_num + 1)
            top_list_tf.erase(last);
        last = top_list_idf.end();
        last--;
        if (top_list_idf.size() >= top_num + 1)
            top_list_idf.erase(last);
        last = top_list_tf_idf.end();
        last--;
        if (top_list_tf_idf.size() >= top_num + 1)
            top_list_tf_idf.erase(last);

        if (cc % 1000 == 0)
            printf("  processed %d words\n", cc);
    }//end for
    printf("\n  the result will be saved in \"result.out\"\n");
    printf("\n\n");
}

void res_output()
{
    printf("--------\n");
    printf("  The program will finish running soon.\n\n");
    freopen("result.out", "w", stdout);
    printf("----OUTPUT----\n\n");
    printf("  Info: %d words, %d different words, %d poems\n\n", tot_words, tot_diff_words, poems);
    printf("  Rank List:\n\n");
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
    //fclose(stdout);
}
