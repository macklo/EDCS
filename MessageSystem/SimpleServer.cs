using System;

class SimpleServer
{
    Server server;
    ClientInfo client;
    void Start()
    {
        server = new Server(2345, new ClientEvent(ClientConnect));
    }

    bool ClientConnect(Server serv, ClientInfo new_client)
    {
        new_client.Delimiter = '\n';
        new_client.OnRead += new ConnectionRead(ReadData);
        return true; // allow this connection
    }

    void ReadData(ClientInfo ci, String text)
    {
        Console.WriteLine("Received from " + ci.ID + ": " + text);
        if (text[0] == '!')
            server.Broadcast(Encoding.UTF8.GetBytes(text));
        else ci.Send(text);
    }
}
