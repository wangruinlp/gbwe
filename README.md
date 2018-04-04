## Tools for "Graph based Bilingual Word Embedding"

Parts of the codes for bilingual co-occurrence graph constructions are from the following paper:

Rui Wang, Hai Zhao, Sabine Ploux, Bao-Liang Lu, and Masao Utiyama. 2016. "A Bilingual Graph-based Semantic Model for Statistical Machine Translation". IJCAI-2016.


Rui Wang, Hai Zhao*, Sabine Ploux*, Bao-Liang Lu, Masao Utiyama, and Eiichro Sumita, "Graph-based Bilingual Word Embedding for Statistical Machine Translation". ACM Transactions on Asian and Low-Resource Language Information Processing (TALLIP), accepted, 2018
 
Parts of the codes for clique extraction and dimension reduction are from the following two papers:

Sabine Ploux and Hyungsuk Ji. 2003. "A model for matching semantic maps between languages (French/English, English/French)". Computational Linguistics.

Hyungsuk Ji, Sabine Ploux and Eric Wehrli. 2003. "Lexical knowledge representation with contexonyms". MT summit.


Corpus preparation:

For monolingual embedding, the corpus is just the raw monolingual corpus. Such as:

"le th¨¨me du bonheur devient ¨¤ la mode parmi les chercheurs ."

For bilingual embedding, the corpus is a combination of a foreign sentence and a target sentence. Such as:

"le th¨¨me du bonheur devient ¨¤ la mode parmi les chercheurs . there is a huge wave of interest in happiness , among researchers ."

There are some perl files for PMI based graph constructions.

statistic.pl: This is for counting the co-occurrences of words in a bilingual corpus.

co_occur_filter.pl: This is for filtering the co-occurrence graph using PMI information.


## INSTALL:
##gcc version 4.4.7

cd ./src

make

This will create the following executable files:

sym: This is for final Contexonym graph construction. 

In particular, vs_dicos.F-mono is a toy monolingual Contexonym graph and vs_dicos.F is a toy bilingual Contexonym graph.

index: This is for the index of the final Contexonym graph.

In particular, index.arb.F and index.arb.F-mono are the index files of vs_dicos.F and vs_dicos.F-mono, respectively.

Please rename the Contexonym graph created by "sym" into "vs_dicos.F" and the index file created by "index" into "index.arb.F".

clique: This is for clique extraction. 

Usage: ./clique work_e F F work_e

This will create several files, typically:

work_e.syn: all of the Contexonym words.

work_e.doc: the document information of clique extraction.

work_e.cli: The cliques.

ca: The Correspondence Analysis (CA) dimension reduction method. 

Usage: ./ca work_e

This will create mainly two files:

work_e.cpc: The vectors for cliques

work_e.cps: The vectors for words

Typically, the 6-dimension vectors of work_e.cpc and work_e.cps are corresponding to the clique/word order in work_e.cli and work_e.syn.

