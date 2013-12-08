One concern for testing big n:
When n > 100k, the pre-allocated local boxArray and actualArray will consume too much stack space and cause some wired seg fault error. By changing boxArray and actualArray to global variable will solve the stack overflow problem. More details refer to code line 12-15 in project.c To test larger n, please be sure to change BIG_NUMBER first to make sure we have enough space to hold boxArray and actualArray

Most of the commented out codes could be used for debugging



Overall Design

1. heap.h (more strictly speaking a maxHeap)
A heap structure I implemented to efficiently keep track k minimal points

heapItem - a pair to keep track both index and distance(to a given point) of a point

heapUp() - a function to adjust heap structure when a new element was added
heapDown() - a function to adjust heap structure when a element was removed from heap
insertHeap() - a function to insert a element to the heap, heapUp/heapDown will be called accordingly(determine by the size and capacity) to adjust the heap structure
popHeap() - pop the root out of the heap, then adjust the structure



2. seek algorithm

quadSplit - split all boxes that have more than k points 
getRadius - get the porper radius for a given point
intersect - determine if the leaf box intersects with the circle (point and radius)
find_k_smallest - for a given set of points, find the k points with smallest distance
isInBox - determine is a box is in a box or not