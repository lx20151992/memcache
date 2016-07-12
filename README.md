# Simple memcache implementation

A server that implements the Memcache Binary Protocol for the Get and Set methods.

Thread pool and mutex is used to handle concurrency issues with multiple simultaneous writers and readers.

###Limitations
* The cache is implemented use a simple STL hashset and assume all keys & values are string type. The data structure of the cache can be better designed to store and retrive different data types more efficient.
* This server implementation does not take care of expiry time. 

###Performance tradeoffs
* Implemented Cache as a singleton, and used mutex to prevent potential concurrency issues. However this makes the write to cache part become bottle neck, writer threads may need to wait for writting to the cache. Possible optimization, 
   - divide cache into a number of smaller parts, so a writer thread only lock a small part of the cache
   - and / or have a number of copies of the cache, each writer thread write simultaneously. Server merges conflicts(if any) afterwards.

###Future work
* Design and implement a data structure for Least Recently Used (LRU) cache.
* Use memory pool to avoid potential memory fragmentation.
* Make the memcache server distributed.

==
###Server
1. Command line

  Build:
  ```
  $ cd Memcache
  $ make
  ```
  Run:
  ```
  $ ./memcache
  ```

2. Xcode

  Xcode project files are included.

==
###Client
Prerequisite

Install [python-binary-memcached](https://github.com/jaysonsantos/python-binary-memcached)
```
$ pip install python-binary-memcached
```
Run:
```
$ python client.py
```
