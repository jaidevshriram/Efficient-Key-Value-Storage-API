# Efficient-Key-Value-Storage-API

This is a project for the course Software Programming for Performance at the International Institute of Information Technology, Hyderabad. The project implements an in-memory key-value storage system. The encapsulated methods and data structure have been optimized to handle millions of requests while keeping memory and CPU usage to a minimum. 

Key (pun intended) to this project is the implementation of a compressed trie. With significant memory optimizations, the trie is able to perform several trnsactions per second while having memory usage that's much better than a regular trie.