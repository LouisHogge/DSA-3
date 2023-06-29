#include <math.h>
#include <stdlib.h>

#include "Sketch.h"
#include "DynamicTimeWarping.h"


/** ------------------------------------------------------------------------ *
 * Calculate the minimum between 3 values 
 *
 * Parameters:
 * -value1      First value 
 * -value2      Second value
 * -value3      Third value
 * 
 * Return
 * -minimum     minimum
 * ------------------------------------------------------------------------- */
static double min(double value1, double value2, double value3);

/** ------------------------------------------------------------------------ *
 * Calculate the absolute mean distance between 2 points 
 *
 * Parameters:
 * -p1      First point 
 * -p2      Second point
 * 
 * Return
 * -dist    absolute mean distance
 * ------------------------------------------------------------------------- */
static double abs_mean_dist(Point p1, Point p2);


double dtw(Sketch sketch1, Sketch sketch2, double maxDistance) 
{
  if(sketch1.size == 0 || sketch2.size == 0)
    return INFINITY;

  // creation and initialization of dtw_matrix 
  double dtw_matrix[sketch1.size+1][sketch2.size+1];


  // initialisation of the dtw_matrix
  dtw_matrix[0][0] = 0;
  // condition allow us to use a single for loop depending on the size of the bigger sketch
  if (sketch1.size > sketch2.size)
  {
    for (size_t i = 1; i < sketch1.size+1; i++)
    {
      dtw_matrix[i][0] = INFINITY;

      if (i < sketch2.size+1)
        dtw_matrix[0][i] = INFINITY;
    }
  }
  else
  {
    for (size_t i = 1; i < sketch2.size+1; i++)
    {
      dtw_matrix[0][i] = INFINITY;

      if (i < sketch1.size+1)
        dtw_matrix[i][0] = INFINITY;
    }
  }

  // calculation of the dtw_matrix 
  for (size_t i = 1; i < sketch1.size+1; i++)
  {
    bool cut_off = true;

    for (size_t j = 1; j < sketch2.size+1; j++)
    {
      dtw_matrix[i][j] = abs_mean_dist(sketch1.points[i-1], sketch2.points[j-1])
                         + 
                         min(dtw_matrix[i-1][j-1], // match
                             dtw_matrix[i-1][j],   // insertion
                             dtw_matrix[i][j-1]);  // deletion

      // cut-off for optimality
      if (cut_off)
      {
        // if one complete line of the matrix is greater or equal than maxDistance
        // cut_off is set to true to return INFINITY
        if (dtw_matrix[i][j] <= maxDistance)
          cut_off = false;
      }
    }

    if (cut_off)
      return INFINITY;
  } 

  // score of this pair of sketch
  return dtw_matrix[sketch1.size][sketch2.size];
}

static double min(double value1, double value2, double value3)
{
  double minimum = value1;
  if (value2 < minimum)
    minimum = value2;

  if (value3 < minimum)
    minimum = value3;

  return minimum;
}

static double abs_mean_dist(Point p1, Point p2)
{
  return ((fabs(((double)(p1.x - p2.x))) + fabs(((double)(p1.y - p2.y)))) / 2.0);
}
