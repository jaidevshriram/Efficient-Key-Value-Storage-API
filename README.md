# Efficient-Key-Value-Storage-API

The main goal with this project was building a database system - with insert, delete, search, and update operations - for key, value pairs. The database was judged on peak memory usage and number of operations per second. Hence, we chose to build a compressed trie. The encapsulated methods and data structure have been optimized to handle millions of requests while keeping memory and CPU usage to a minimum.

The trie automatically adapts to the data inserted/deleted to/from the database. The resulting data strucutre saves a considerable amount of space while being parallelizable at the same time. Further, to save time in allocating memory, we make use of free lists to service more requests sooner.

More details can be found in the report pdf. 

Built in a team of four for the course 'Software Programming for Performance' at IIIT-H.
