# Conclave Server

> Conclave: a private meeting

The ability to create and join rooms and send and receive unreliable datagrams to members of the rooms that you are in.

This server is created for testing purposes, do not use in production!

## Usage

### Initialize

```c
int clvServerInit(ClvServer* self, struct ImprintAllocator* memory);
```

### Feed

For each packet received from a `sockaddr_in`, call `clvServerFeed()`:

```c
int clvServerFeed(ClvServer* self, const ClvAddress* address, const uint8_t* data,
                  size_t len, ClvResponse* response);
```
