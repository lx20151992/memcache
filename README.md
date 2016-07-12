# Simple memcache implementation

A server that implements the Memcache Binary Protocol for the Get and Set methods.

Thread pool and mutex is used to handle concurrency issues with multiple simultaneous writers and readers.

####Limitations
The cache is implemented use a naive hashset. It does not take care of expiry time.

####Performance tradeoffs

####Future work
* A smart caching algorithm can be implemented to make the caching more effcient.
* Memory pool to avoid potential memory fragmentation.

####Server
1. Command line

  Build:
  ```
  cd Memcache
  make
  ```
  Run:
  ```
  ./memcache
  ```

2. Xcode

  Xcode project files are included.

####Client
Prerequisite

Install [python-binary-memcached](https://github.com/jaysonsantos/python-binary-memcached)
```
pip install python-binary-memcached
```
Run:
```
python client.py
```
