#B+ Tree:

M = Number of buffers available
B = Size of memory buffer

##Usage
 
g++ b+tree.cpp -std=c++11
./a.out inut_filename M B


##Explanation:

For the insertion part of B+ tree, we iterate down to the leaf,add the node.
If it exceeds the degree then we split the node into two, and copy its middle element to its parent.Similarly if parent exceeds the degree the split is again called and this happens recursively. An important point is to let the middle element stay in the node which we are splitting, if it is a leaf , and not otherwise. 

For the queries of finding count of values, our nodes have a varible called count. We reach to the node with that value, using the B+tree ordering and retrieve its count from that variable, 
returning 0 is that value is not found. 

For range queries, every B+tree node maintains the count of total nodes in its subtree. 
This helps in speeding up since for B+ tree nodes having all values in the query range, 
we can just retrieve their answers directly.And we only need to finally find the count in maximum two leaves, instead of iterating for all the leaves falling in our range. 

Refer to code for implementation details. 

#Linear Hashing:

M = Number of buffers available
B = Size of memory buffer

##Usage:

g++ linear_hashing.cpp -std=c++11
./a.out input_filename M B

##Explanation:

Buckets are created with room to increase more nodes using overflow. The HashTable on finding the hash finds the appropriate bucket number, checks in the bucket if value is present. If node is not found, it adds the new value into the bucket. After adding the load is checked and if load exceeds a threshold, a new slot is added, if modulo is less than that it is doubled to yield a new hsh function. The next bucket to be split is split. Increase the nextslottoSplit counter.If all entries of previous hash function has been visited , i.e slot size if half of current modulo, then reinitialize  the next slot to split. 

Refer to code for implementation details. 
