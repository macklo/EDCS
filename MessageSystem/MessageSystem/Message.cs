using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;

namespace MessageSystem
{
    [DataContract]
    class Message
    {
        [DataMember]
        string message;

        [DataMember]
        string receiverAddress;

        [DataMember]
        string senderAddress;

        [DataMember]
        bool isAlert;

        public Message(string message, string receiverAddress, string senderAddress, bool isAlert)
        {
            this.message = message;
            this.receiverAddress = receiverAddress;
            this.senderAddress = senderAddress;
            this.isAlert = isAlert;
        }

        public string getJsonString()
        {
            MemoryStream ms = new MemoryStream();

            DataContractJsonSerializer ds = new DataContractJsonSerializer(typeof(Message));
            ds.WriteObject(ms, this);
            string str = Encoding.UTF8.GetString(ms.ToArray());
            ms.Close();
            return str;
        }
    }
}
