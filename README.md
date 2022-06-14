## NOTE: If you are a student at WSU, please follow to the WSU Academic Integrity Policy in regards to this repository. The policy can be found [here](https://communitystandards.wsu.edu/policies-and-reporting/academic-integrity-policy/) <br> If you atttend any university other than WSU, please refer to your universities academic integrity policy.

# CptS-471

This is a repository for the Cpt_S 471 Computational Genomics class at WSU for the Spring 2022 semester. 

Professor: Ananth Kalyanaraman

## Assignments

### Programming Project 1 (PP1) - Sequence Alignments

Given two DNA sequences, find their optimal global / local alignments using the Needleman-Wunsch and Smith-Waterman algorithms respectively.

Input parameters: 

`<executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>`
<br><br>
___
<br>

### Programming Project 2 (PP2) - Suffix Tree Construction

Given a sequence, contruct its suffix tree and output the BWT to a file.

Input parameters: 

`<executable name> <input file containing the alphabet of the sequence> <sequence file>`

#### Clarifications

The alphabet input file should contain at least all letters in the sequence file. The alphabet should be in lexiocgraphical order with spaces in between each character. The size of the alphabet is hard-coded into the tree.hpp file. Please change ALPHA_LEN to be the size of the input alphabet.

The sequence file should contain a garbage line on line 1, this is to allow for naming and other info. The program will start reading from line 2
</br></br>
___
</br>

### Programming Project 3 (PP3) - Suffix Array Construction
Given a group of sequence files, construct a Generalized Suffix Tree of all the sequences and find the shortest
unique substring of each sequence.

Then, for each pair of sequences, construct a GST and a similarity matrix. This will find the longest common substring and similarity score between the two sequences. The similarity score will be calculated using the affine gap penalty.

There are multiple sample sequences and alphabets included in the repository. However, custom sequences and alphabets can be used. Please ensure that the custom sequence file follows the same format as the examples and that the custom alphabet is in lexiocgraphical order with spaces in between each character.
