class ServerBase{
    public:
        ServerBase();

    ServerBase::ServerBase(int port, char hostname){
    struct sockaddr_in this.socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = inet_addr(hostname); 

    }

    void()
}