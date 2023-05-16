#!/usr/bin/bash
valgrind --leak-check=full --suppressions=./misc/valgrind_suppressions.supp ./cmake/$1
