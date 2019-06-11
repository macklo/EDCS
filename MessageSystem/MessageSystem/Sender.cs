using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace MessageSystem
{
    public class Sender
    {
        MainWindow mainWindow;

        public Sender(MainWindow mainWindow)
        {
            this.mainWindow = mainWindow;
        }

        public void sendMessage(string message, string receiverIpAddress, bool testMessage = false)
        {
            byte[] bytes = new byte[1024];

            try
            {
                IPHostEntry ipHostInfo = Dns.GetHostByName(Dns.GetHostName());
                IPAddress ipAddress = ipHostInfo.AddressList[1];

                IPHostEntry tmp = Dns.GetHostByName(receiverIpAddress);
                IPAddress receiverIPAdress = tmp.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(receiverIPAdress, 11000);

                Socket sender = new Socket(ipAddress.AddressFamily,
                    SocketType.Stream, ProtocolType.Tcp);

                try
                {
                    sender.Connect(remoteEP);

                    Console.WriteLine("Socket connected to {0}",
                        sender.RemoteEndPoint.ToString());

                    byte[] msg = Encoding.ASCII.GetBytes(message);

                    int bytesSent = sender.Send(msg);

                    if (testMessage)
                    {
                        int bytesRec = sender.Receive(bytes);
                        Console.WriteLine("Echoed test = {0}",
                            Encoding.ASCII.GetString(bytes, 0, bytesRec));
                        mainWindow.recievedTestMessage(Encoding.ASCII.GetString(bytes, 0, bytesRec));
                    }
                    sender.Shutdown(SocketShutdown.Both);
                    sender.Close();
                }
                catch (ArgumentNullException ane)
                {
                    Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
                }
                catch (SocketException se)
                {
                    Console.WriteLine("SocketException : {0}", se.ToString());
                    mainWindow.addLocalMessage("Connection timed out", receiverIpAddress);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Unexpected exception : {0}", e.ToString());
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
