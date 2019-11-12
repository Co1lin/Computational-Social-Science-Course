//
//  main.cpp
//  t1
//
//  Created by Colin on 2019/11/11~13.
//  Copyright © 2019 Colin. All rights reserved.
//
//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <fstream>
using namespace std;

struct word
{
    int tot, poem;
    double tf, idf, tf_idf;
    vector<int> poem_index;
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
    void output(ofstream &f) const
    {
        f << (*iter).first;
        if (list_type == 1)
            f << " (" << (*iter).second.tf << "), ";
        else if (list_type == 2)
            f << " (" << (*iter).second.idf << "), ";
        else
            f << " (" << (*iter).second.tf_idf << "), ";
    }
};
struct rank_for_each_poem
{
    string origin;
    multiset<top_ele> rank[3];

    void insert(const map<string, word>::iterator &it)
    {
        for (short i = 0; i <= 2; i++)
        {
            rank[i].insert({it, (short)(i + 1)});
        }
    }

    void update()
    {
        for (short i = 0; i <= 2; i++)
        {
            stack<top_ele> tmp;
            multiset<top_ele>::iterator it_out;
            for (it_out = rank[i].begin(); it_out != rank[i].end(); it_out++)
            {
                tmp.push(*it_out);
                rank[i].erase(it_out);
            }
            while (!tmp.empty())
            {
                top_ele ele = tmp.top();
                rank[i].insert(ele);
                tmp.pop();
            }
        }
    }

    void output(ofstream &f)
    {
        update();
        f << "      | TF Rank    : ";
        multiset<top_ele>::iterator it_out;
        for (it_out = rank[0].begin(); it_out != rank[0].end(); it_out++)
            (*it_out).output(f);
        f << endl;
        f << "      | IDF Rank   : ";
        for (it_out = rank[1].begin(); it_out != rank[1].end(); it_out++)
            (*it_out).output(f);
        f << endl;
        f << "      | TF_IDF Rank: ";
        for (it_out = rank[1].begin(); it_out != rank[1].end(); it_out++)
            (*it_out).output(f);
        f << endl;
    }
};

int top_num = 6;
map<string, word> mydata;
vector< rank_for_each_poem > poem_list;
int poems = 0;
int tot_words = 0;
int tot_diff_words = 0;
multiset<top_ele> top_list_tf;
multiset<top_ele> top_list_idf;
multiset<top_ele> top_list_tf_idf;

short s2num(const string &s)
{
    short ret = 0;
    short ten = 1;
    for (int i = s.length() - 1; i >= 0; i--)
    {
        ret += (s[i] - '0')* ten;
        ten *= 10;
    }
    return ret;
}
void del_the_same(const map<string, word>::iterator &it, const int &list_type);
void pre_output();
void read_data();
void calculate();
void res_output(ofstream &f);

int main()
{
    clock_t start_time, end_time;
    start_time = clock();
    pre_output();
    read_data();
    calculate();
    ofstream f;
    f.open("../result.txt");
    res_output(f);
    end_time = clock();
    double tot_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("--------\n\n");
    f << "--------\n\n";
    printf("  Run time: %f s\n\n", tot_time);
    f << "  Run time: " << tot_time << " s\n\n";
    printf("----PROGRAM COMPLETED----\n");
    f << "----END----\n";

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

void read_data()
{
    ifstream in("../data.in");
    string test;
    printf("----READ DATA----\n\n");
    poem_list.push_back({});
    while (getline(in, test))
    {
        poem_list.push_back({});
        poems++;
        poem_list[poems].origin = test;
        //int word_count_in_this_poem = 0;
        int before = 0; //last space + 1
        map<string, bool> words_in_this_poem;
        for (int i = 1; i <= test.length(); i++)
        {
            if (test[i] == ' ' || test[i] == '\0')
            {
                //word_count_in_this_poem++;
                string thisword = test.substr(before, i - before);
                before = i + 1;
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
                    {
                        (*it).second.poem++;
                        poem_list[poems].insert(it);
                    }
                    words_in_this_poem[thisword] = true;
                }
                else    //can't find; a new word
                {
                    tot_diff_words++;
                    mydata[thisword] = {1, 1};
                    it = mydata.find(thisword);
                    poem_list[poems].insert(it);
                }
                it->second.poem_index.push_back(poems);
            }//end if
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
    printf("\n  The result will be saved in \"result.out\"\n\n");
    printf("\n\n");
}

void res_output(ofstream &f)
{
    printf("--------\n");
    printf("  Outputing the result into \"result.txt\"...\n\n");
    f << "----OUTPUT----\n\n";
    f << "  Info: " << tot_words << " words, " << tot_diff_words << " different words, " << poems << " poems\n\n";
    f << "  Total Rank List:\n\n";
    //top_ele output
    f << "    TF    : ";
    multiset<top_ele>::iterator it_out;
    for (it_out = top_list_tf.begin(); it_out != top_list_tf.end(); it_out++)
        (*it_out).output(f);
    f << endl;
    f << "    IDF   : ";
    for (it_out = top_list_idf.begin(); it_out != top_list_idf.end(); it_out++)
        (*it_out).output(f);
    f << endl;
    f << "    TF_IDF: ";
    for (it_out = top_list_tf_idf.begin(); it_out != top_list_tf_idf.end(); it_out++)
        (*it_out).output(f);
    f << endl << endl;

    //analylize some poems
    f << "  Rank List for some poems:\n";
    printf("----ADDITIONAL INPUT----\n\n");
    printf("  Please input which poems you want to see its words rank list:\n");
    printf("  (input a series of number, END with -1!):\n    ");

    vector<int> poem_index;
    int tmp_index = 0;
    while (cin >> tmp_index)
    {
        if (tmp_index == -1)
            break;
        poem_index.push_back(tmp_index);
    }
    for (short i = 0; i <= poem_index.size() - 1; i++)
    {
        f << "\n    For poem " << poem_index[i] << " : \n";
        printf("    Outputing data for poem %d...\n", poem_index[i]);
        poem_list[poem_index[i]].output(f);
        f << "    Original Sentence: " << poem_list[poem_index[i]].origin << endl;
    }
    printf("\n  Finished!\n");
}
