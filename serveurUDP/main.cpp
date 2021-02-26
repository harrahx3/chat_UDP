/**#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>
#include <list>

using namespace std;
using namespace sf;
int main()
{
    Packet packet;
    unsigned short port;
    string str;
    IpAddress adress;
    UdpSocket socket;
    socket.bind(54000);
    SocketSelector selector ;

    while(true)
    {
        socket.receive(packet, adress, port);
        packet>>str;
        cout<<"recieved data from '"<<adress<<"' at port '"<<port<<"' : '"<<str<<"'"<<endl;
        socket.send(packet, adress, port);
    }

    return 0;
}**/


#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <map>

using namespace std;
using namespace sf;

Packet receive(UdpSocket& socket, multimap <IpAddress, unsigned short>& clients)
{
    sf::Packet data;
    IpAddress sender;
    unsigned short port;
    string str;
    socket.receive(data, sender, port);
    data>>str;
    cout<<"recieved data from '"<<sender<<"' at port '"<<port<<"' : '"<<str<<"'"<<endl;
    ///clients[sender] = port;  ->use it with 'map' if all ipAdress are different
    clients.insert(make_pair((IpAddress)sender, (unsigned short)port));     ///->use it wit 'multimap' if it's possible there are several the same IpAdress(like interne)
    return data;                                                            ///  but send the message several to the same client !
}

void send (UdpSocket& socket, Packet& data, multimap<IpAddress,unsigned short>& clients)
{
    for(multimap<IpAddress, unsigned short>::iterator it = clients.begin(); it!=clients.end(); ++it)
        socket.send(data, it->first, it->second);

    cout<<"sent data"<<endl;
}

int main()
{
    multimap <IpAddress, unsigned short> clients;    ///store pointers on the clients' sockets
    sf::SocketSelector selector;
    sf::Packet data;    /// recieved data
    UdpSocket socket;
    socket.bind(54000);
    selector.add(socket);

    do
    {
        if (selector.wait())///controler si on a recu un message bloque tant qu'aucune des sockets n'est prete
        {
            data = receive(socket, clients);///receive the message
            send(socket, data, clients);///and send it to all clients
        }
    }
    while(true);    ///infinite loop
    system("PAUSE");

    return 0;
}
