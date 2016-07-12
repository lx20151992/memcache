# Simple memcache implementation

A server that implements the Memcache Binary Protocol for the Get and Set methods.



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
