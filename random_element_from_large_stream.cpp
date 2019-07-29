/*
Given a stream of elements too large to store in memory, pick a random element from the stream with uniform probability.
*/

/*
In order to get a random element from the stream, we need to see the whole stream.
The solution is the following: for each element that we see in the stream, we generate a random number between 0 and 1. If the number generated is the largest (or smallest)
that we have generated so far, we save the current element in the stream as the one that will be returned. We only need two variables: the largest number generated so far,
and the element corresponding to when the largest number was generated. We return the element that we have saved in our variable.
*/
