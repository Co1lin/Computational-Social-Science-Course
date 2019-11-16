# -*- coding: utf-8 -*-
import json

statistics = {}

#input
print "Please input five different accusations:\n"
for i in range(1, 2):
    mystr = raw_input("  input No." + str(i) + " : ")
    #mystr.encode('utf-8')
    mystr = unicode(mystr, 'utf-8')
    statistics[mystr] =\
        {
            "life": 0,
            "death": 0
        }
#process
print "\nInput ended. Start processing...\n"
index = 0
with open("train.json", 'r') as f:
    for line in f:
        index += 1
        dict_line = json.loads(line)
        dict_meta = dict_line["meta"]
        info = dict_meta["term_of_imprisonment"]
        death = info["death_penalty"]
        life = info["life_imprisonment"]
        last = info["imprisonment"]
        for accu in dict_meta["accusation"]:
            if statistics.has_key(accu):
                if death:
                    statistics[accu]["death"] += 1
                elif life:
                    statistics[accu]["life"] += 1
                else:
                    #statistics[accu][last] =\
                    #    statistics[accu].get(last, default=0) + 1
                    if statistics[accu].has_key(last):
                        statistics[accu][last] += 1
                    else:
                        statistics[accu][last] = 1
                #endif
            #endif
        #end for
        if index % 100 == 0:
            print "  Processed about " + str(index) + " lines."
        #if index == 10000:
        #    break
    #end for
#end with
#output
print "\nProcessing ended. Start Outputing...\n"
fo = open("output.txt", "w")
fo.write("----OUTPUT----\n\n")
#fo.write(str(statistics))
for accu, info in statistics.items():
    fo.write(accu)
    fo.write("\n")
    fo.write("刑期 " + "数量")
    fo.write("\n")
    for key, val in info.items():
        fo.write(str(key))
        fo.write(" : ")
        fo.write(str(val))
        fo.write("\n")
    #end for
    fo.write("\n")
#end for
fo.write("\n----OUTPUT ENDED----\n")
