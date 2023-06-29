# Data structures and algorithms Project 3: Problem solving

## Project Description
We'd like to create an application for finding and recognizing sketches made quickly by hand. To this end, we have identified ten classes of interest and have a reference database containing 1000 sketches per class. From a source sketch (the query), the aim is to identify as quickly as possible in the database the k sketches "closest" to this query by a certain distance. Sketches found in this way can then be used to make a prediction about the class of the source sketch, for example, by assigning it the majority (i.e. most frequent) class among the k sketches found.

## How to Use the Project

### Compilation
Use *Makefile*.

### Run
```bash
BPQ.c DynamicTimeWarping.c NearestNeighbours.c Sketch.c easyppm.c --std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -g -o progs
```
