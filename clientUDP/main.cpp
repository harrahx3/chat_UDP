#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>
#include <list>

using namespace std;
using namespace sf;

void send (UdpSocket& recipient)
{
    sf::Packet data;
    string message;
    cout<<"What's your message?"<<endl;
    cin.ignore();
    getline(cin, message);
    data<<message;
    recipient.send(data, "127.0.0.1", 54000);
    cout<<"message sent !"<<endl;
}

sf::Socket::Status receiveWithTimeout(sf::UdpSocket& socket, sf::Packet& packet, sf::Time timeout)
{
    IpAddress sender;
    short unsigned port;
    sf::SocketSelector selector;
    selector.add(socket);
    if (selector.wait(timeout))
        return    socket.receive(packet, sender , port);
    else
        return sf::Socket::NotReady;
}

int main()
{
    unsigned short port;
    sf::UdpSocket socket;
    string str, sender;
    sf::Packet data;

// lie la socket à un port
    socket.bind(Socket::AnyPort);

    do
    {
        cout<<"entrer 'actualise' pour verifier les messages ou autre chose pour en envoyer"<<endl;
        cin>>str;
        if (str != "actualise")
        {
            send(socket);    ///envoyer un message
        }
        while (receiveWithTimeout(socket, data, seconds(0.1f)) == Socket::Done)    ///si on a recu un message
        {
            data>>str;
            ///data>>sender;   ///recuperer les donnees : expediteur puis message
            cout<<"Vous avez recu un message : servor says '"<<str<<"'"<<endl;
        }
    }
    while (true);   ///an infinite loop

    return 0;
}


/**int main()
{
    string str;
    unsigned short port;
    IpAddress sender;
    UdpSocket socket;
    Packet packet;
    socket.bind(Socket::AnyPort);
    do
    {
        cout<<"entrer 'actualise' pour verifier les messages ou autre chose pour en envoyer"<<endl;
        cin>>str;
        if (str != "actualise")
        {
            cout<<"message?"<<endl;
            cin>>str;
            packet<<str;
            socket.send(packet, "127.0.0.1", 54000);
        }

        socket.receive(packet, sender, port);

        packet>>str;
        cout<<"Vous avez recu un message : '"<<sender.toString()<<"' at port '"<<port<<"' says '"<<str<<"'"<<endl;
    }
    while (true);   ///an infinite loop

    return 0;
}**/
