# Simple memcache implementation

A server that implements the Memcache Binary Protocol for the Get and Set methods.

Thread pool and mutex is used to handle concurrency issues with multiple simultaneous writers and readers.

Use client.py to test the server.

####Limitations
The cache is implemented use a naive hashset. It does not take care of expiry time. For future work, a smart caching algorithm can be implemented to make the caching more effcient.
Memory management can be improved, e.g. use a memory pool.
