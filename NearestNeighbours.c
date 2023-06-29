#include <stdlib.h>

#include "NearestNeighbours.h"
#include "BoundedPriorityQueue.h"
#include "DynamicTimeWarping.h"
#include "math.h"


SketchDistance* nearestNeighbours(const Dataset* dataset, Sketch query, size_t k) 
{

  // check inputs arguments
  if (!dataset || !dataset->sketches || 
      !query.strokeStarts || !query.points) 
  {
    // error in arguments
    return NULL;
  }

  if (k == 0 || k > dataset->size)
  {
    // error in arguments
    return NULL;
  }


  // priority queue of size k (we want to the k nearest sketch) 
  BoundedPriorityQueue *prioQ = bpqCreate(k); 

  double dyn_tim_wrap;
  double maxDistance = 0.0;

  // in order to be able to create the SketchDistance pointer that we have to return at the end
  double distanceMemory[dataset->size];

  for(size_t num_sketch = 0; num_sketch < dataset->size; num_sketch++){

    if (num_sketch < k){
      dyn_tim_wrap = dtw(query, dataset->sketches[num_sketch], INFINITY);

      // insert dyn_tim_wrap as its priority (= distance return by dtw())                                            
      // and its n° as value
      bool error = bpqInsert(prioQ, dyn_tim_wrap, num_sketch);
      if (!error)
      {
        // the key/value is not added because queue is full
        return NULL;
      }

      if (dyn_tim_wrap > maxDistance)
        maxDistance = dyn_tim_wrap;

      // update memory
      distanceMemory[num_sketch] = dyn_tim_wrap;
    }
    else{
      dyn_tim_wrap = dtw(query, dataset->sketches[num_sketch], maxDistance);

      if (dyn_tim_wrap < maxDistance){
        
        // insert dyn_tim_wrap at the current maxDistance
        bpqReplaceMaximum(prioQ, dyn_tim_wrap, num_sketch);

        // re-establish a maxDistance
        maxDistance =  bpqMaximumKey(prioQ);

        // update memory
        distanceMemory[num_sketch] = dyn_tim_wrap;
      }
    }
  }
  
  
  // num of all best skecthes
  size_t *best_sketch = bpqGetItems(prioQ);

  // contains the k nearest sketch + distance
  SketchDistance *nearest_neighbours = malloc(k * sizeof(SketchDistance));

  for(size_t i = 0; i < k; i++){

    // we pick up the distance associated to the value stored in best_sketch
    nearest_neighbours[i].distance = distanceMemory[best_sketch[i]];

    // we pick up the index of the sketch associated to the value store in best_sketch
    nearest_neighbours[i].sketch = &dataset->sketches[best_sketch[i]];
  }

  // free all
  free(best_sketch);
  bpqFree(prioQ);

  return nearest_neighbours;
}
