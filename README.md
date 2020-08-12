# Synapster

Synapster is a peer-to-peer file sharing software that works on *nix
systems. The app is a simplified version of the once popular file sharing app
Napster. The app works with its own application protocol on top of TCP. *This is a work in progress*

> Although I use the term *app*, it is just a command line utility which could be used further to build a GUI app.

The project structure is as follows

```
----|
    |--server(metadata server)
    |
    |--processing
    |
    |--peer
```

**Server**

This folder contains files that are needed to build metadata server. 

The purpose of metadata server is to maintain the information about active files that can be downloaded from active peers. It is to be noted that it only stores information regarding files that can be served from active users. If a user goes offline, the information regarding that particular user will be deleted.

As of now, the server maintains a unordered map that comes with C++ STL for maintaining the metadata, but in the next iteration of development(if it happens), this will be changed to SQLite. 

The server needs to be given a port value as command line argument. This port is the one on which server listens to for peers.

**Processing**

This folder contains files that are needed for processing the data received from TCP layer i.e., all the files that concern with application protocol.

The folder contains encoding functions, framing functions and handler functions. The encoding of a message depends on the kind of request and response that is being sent. If the data involves only numerics, binary encoding is done. Else, text encoding is used.

The framing function uses 32-bit length attribute and doesn't have any start and end delimiters. Although, the codec functions check for MAGIC word to avoid processing information that doesn't belong to the protocol.

The handler functions are the ones used by both client and server to handle requests and responses. These can be restructured and segregated into server and client code.

**Peer**

This folder contains the code required to build a peer in the network. 

When starting the peer, it needs to be given information about the sharable folder that can be shared with other peers, the port on which peer listens to other peers and the IP address and port of metadata server.

The sharable folder should contain a file with <keyword,filename> key-value pairs. This information is read by client code and sent to metadata server in a ```join``` request.




> The project is still in progress. Encoding and Framing functions are done. Handlers, Server and Peer code is yet to be implemented.
