using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization.Json;

namespace MessageSystem
{
    public class Listener
    {
        public string data = "";
        Socket listener;
        MainWindow mainWindow;
        public string ipAddress;

        public void Start(MainWindow mainWindow) {
            this.mainWindow = mainWindow;
            IPHostEntry ipHostInfo = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddress = ipHostInfo.AddressList[3];
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);
            Console.WriteLine(ipAddress);
            //this.ipAddress = ipAddress.ToString();
            this.ipAddress = "79.184.167.160";

            listener = new Socket(ipAddress.AddressFamily,
                SocketType.Stream, ProtocolType.Tcp);

            try {
                listener.Bind(localEndPoint);
                listener.Listen(10);

            }
            catch (Exception e) {
                Console.WriteLine(e.ToString());
            }
            Console.WriteLine("Server started");
        }

        public void Listen()
        {
            byte[] bytes = new Byte[1024];
            while (true)
            {
                Console.WriteLine("Waiting for a connection...");
                Socket handler = listener.Accept();
                data = "";
                int bytesRec = handler.Receive(bytes);
                data += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                /*
                while (bytesRec != 0)
                {
                    
                    if (data.IndexOf("<EOF>") > -1)
                    {
                        break;
                    }
                }
            */
                data = data.Substring(0, data.IndexOf("<EOF>"));

                MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(data));
                DataContractJsonSerializer ds = new DataContractJsonSerializer(typeof(Message));

                Message msgObject = ds.ReadObject(ms) as Message;
                if (msgObject.areYouAlive)
                {
                    Message returnMessage = new Message("", msgObject.senderAddress, ipAddress, false, true);
                    handler.Send(Encoding.ASCII.GetBytes(returnMessage.getJsonString()));
                }

                Console.WriteLine("Text received : {0}", data);
                mainWindow.addMessage(msgObject);

                byte[] msg = Encoding.UTF8.GetBytes(data);

                //handler.Send(msg);
                handler.Shutdown(SocketShutdown.Both);
                handler.Close();
            }
        }
    }
}
