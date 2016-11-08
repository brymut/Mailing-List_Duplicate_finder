# Duplicate finder for Mailing Lists implemented in C


# 1 Requirement
A mailing list is a collection of people’s names and addresses, used for bulk mailings. Mailing lists from
different sources are often merged. A merged mailing list might contain duplicate entries, with only trivial
differences, such as alternative forms of names, missing parts of addresses, etc. Duplicate entries result in
excessive postage costs, and irritation for recipients of multiple copies of a single mailing. A program is
required to report potential duplicate entries in a given mailing list.

#2 Specification

The program’s function is to report potential duplicate entries in a given mailing list. Two entries are to be
treated as potential duplicates if they have identical surnames, post-codes, and house numbers; these
comparisons are made in a case-insensitive way.
A mailing list file is a text file in which each entry occupies three successive lines:


- full name
- address
- post-code

The full name consists of an individual’s surname, followed by a comma (‘,’), followed by an optional title,
followed by one or more forenames. The address line consists of a house number, street name, and city; the
city is separated from the rest of the address by a comma (‘,’). This file will be read from the standard input.
The program’s output is a report on the standard output showing pairs of potentially duplicate entries. Both
entries of a pair should be written out in full, exactly as they appear in the mailing list file. For example:


```
Potential duplicate
===================
Meldrew, Margaret
1 Happenstance Place, Epping, London
N11 3SR
==========
Meldrew, Victor
1 Happenstance Place, London
N11 3SR
Potential duplicate
===================
Sventek, Joe
University of Glasgow, Glasgow
G12 8QQ
==========
Sventek, Prof. Joseph
University of Glasgow, Glasgow
G12 8QQ
3 Design

```
