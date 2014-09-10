LDPMap Program (Version 2.0, maintained by Aaron Albin, Email: albin.9@osu.edu)

This is the latest version of LDPMap program, which is used for onGrid (Aaron Albin, Xiaonan Ji, Tara B. Borlawsky, Zhan Ye, Simon Lin, Philip R.O. Payne, Kun Huang, Yang Xiang, "Enabling efficient online studies on conceptual relationships between medical terms", JMIR Medical Informatics, In press). 

The original program is used for paper "Effectively processing medical term queries on the UMLS Metathesaurus by layered dynamic programming" by Kaiyu Ren, Albert M. Lai, Aveek Mukhopadhyay, Raghu Machiraju, Kun Huang, and Yang Xiang, BMC Medical Genomics, 2014, 7(Suppl 1):S11. DOI: 10.1186/1755-8794-7-S1-S11

Software License Agreement: You may use or modify this computer program for research purposes, provided that you properly cite our paper in publication. This computer program is provided on an as is basis and there is no guarantee on the program nor additional support offered. Neither the author(s) nor their institute(s) is liable under any circumstances. This program archive (including this license agreement) may be updated without further notice.

The following of this README file describes how to run the program, including the commands and parameters needed for appropriate executions.

This archive contains the program in source code form as well as the make file. To compile this program, you need to go to its directory and type make(in Linux). To run the program, you need to type bin/UMLSLDP followed by parameters needed.

Here is the instruction on how to run the program:

$ bin/UMLSLDP.exe

Usage:
        UMLSLDP [-h] [-c] [-t1 threshold1] [-t2 threshold2] filename

-h: Used when you need to print the help message.
-c: Used when you want to use the program in case-sensitive mode.
-t1 threshold1: Used when you need to customize the threshold T1 in paper. The default value is 0.85
-t2 threshold2: Used when you need to customize the threshold T2 in paper. The default value is 0.35

Filename: 

Name of the file containing data source.

Input:

1.	Term intended to be queried.

2.	Number of results having highest match scores that you want to output.cd 

Output:

Matching results sorted by scores in descent order.

Therefore, a typical command line would look like:

$ bin/UMLSLDP.exe -c ./MRCONSO.RRF

Reading...
----------------------------------UMLSLDP-------------------------------------
Query Name:
Gastro reflux
Top Match Number:
5
LDPMap_Query...
The most match:
C0558176|Gastric reflux  Score: 0.884615
====================================
Alternative matches:
====================================
C0017168|gastric reflux  Score: 0.807692
C0558176|gastric reflux  Score: 0.807692
C0017168|Gastresophageal reflux  Score: 0.785714
C0017168|Gastroesophageal reflux  Score: 0.772727
