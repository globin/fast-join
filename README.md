# Fast Join

[![Build Status](https://travis-ci.org/globin/fast-join.svg?branch=master)](https://travis-ci.org/globin/fast-join)

This is a WIP implementation of the paper
**Fast-Join: An Efficient Method for Fuzzy Token - Matching based String Similarity Join**
(http://dbgroup.cs.tsinghua.edu.cn/dd/list/6.pdf)

To run the tests call after making sure to have **libgtest** installed

```
./build.sh  # Will ask for password to install libsimmetrics
./build/tests/test
```


## Installing libgtest on Debian-based systems

```
sudo apt-get install libgtest-dev
"cd /usr/src/gtest && sudo cmake . && sudo cmake --build . && sudo mv libg* /usr/local/lib/ ; cd -"
```
