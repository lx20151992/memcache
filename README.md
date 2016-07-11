# simple memcache implementation

A server that implements the Memcache Binary Protocol for the Get and Set methods.

Thread pool and mutex is used to handle concurrency issues with multiple simultaneous writers and readers.

Use client.py to test the server.
