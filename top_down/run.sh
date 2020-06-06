#!/bin/bash

i=[0, 1, 2]
j=[2, 4, 6, 8]

for (( i = 0; i < 3; i++ )); do
  for (( j = 0; j < 10; j += 2 )); do
      ../cmake-build-debug/top_down/top_down ../test/peg_examples/Java1.5.txt ../test/java/Arrays.java i j
  done
done