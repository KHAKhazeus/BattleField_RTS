
#include "rts_async_client.h"
GameMessageSet RTSClient::GameExchangeOnce(GameMessageSet message_set){
    // Data we are sending to the server.
    //HelloRequest request;
    //request.set_name(user);
    // Container for the data we expect from the server.
    GameMessageSet reply;
    
    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;
    
    // The producer-consumer queue we use to communicate asynchronously with the
    // gRPC runtime.
    CompletionQueue cq;
    
    // Storage for the status of the RPC upon completion.
    Status status;
    
    // stub_->PrepareAsyncSayHello() creates an RPC object, returning
    // an instance to store in "call" but does not actually start the RPC
    // Because we are using the asynchronous API, we need to hold on to
    // the "call" instance in order to get updates on the ongoing RPC.
    std::unique_ptr<ClientAsyncResponseReader<GameMessageSet> > rpc(
                                                                    stub_->PrepareAsyncOneExchange(&context, message_set, &cq));
    
    // StartCall initiates the RPC call
    rpc->StartCall();
    
    // Request that, upon completion of the RPC, "reply" be updated with the
    // server's response; "status" with the indication of whether the operation
    // was successful. Tag the request with the integer 1.
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    // Block until the next result is available in the completion queue "cq".
    // The return value of Next should always be checked. This return value
    // tells us whether there is any kind of event or the cq_ is shutting down.
    GPR_ASSERT(cq.Next(&got_tag, &ok));
    
    // Verify that the result from "cq" corresponds, by its tag, our previous
    // request.
    GPR_ASSERT(got_tag == (void*)1);
    // ... and that the request was completed successfully. Note that "ok"
    // corresponds solely to the request for updates introduced by Finish().
    GPR_ASSERT(ok);
    
    // Act upon the status of the actual RPC.
    if (status.ok()) {
        return reply;
    } else {
        reply.set_broken("broken");
        return reply;
    }
}

bool RTSClient::startRequest(GameMessageSet start_request){
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    //RTSClient greeter(grpc::CreateChannel(
    //"localhost:50051", grpc::InsecureChannelCredentials()));
    //std::string user("world");
    GameMessageSet reply = this->GameExchangeOnce(start_request);  // The actual RPC call!
    if(reply.broken()=="Start"){
        return true;
    }
    else{
        return false;
    }
}

GameMessageSet RTSClient::exchangeOnceBlock(GameMessageSet new_request){
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    //RTSClient greeter(grpc::CreateChannel(
    //"localhost:50051", grpc::InsecureChannelCredentials()));
    //std::string user("world");
    GameMessageSet reply = this->GameExchangeOnce(new_request);  // The actual RPC call!
    std::cout << "Message received: " << reply.broken() << std::endl;
    return reply;
}
