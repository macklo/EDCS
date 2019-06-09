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
    public class Message
    {
        [DataMember]
        public string message { get; set; }

        [DataMember]
        public string receiverAddress { get; set; }

        [DataMember]
        public string senderAddress { get; set; }

        [DataMember]
        public bool isAlert { get; set; }

        [DataMember]
        public bool areYouAlive;

        public Message(string message, string receiverAddress, string senderAddress, bool isAlert, bool areYouAlive = false)
        {
            this.message = message;
            this.receiverAddress = receiverAddress;
            this.senderAddress = senderAddress;
            this.isAlert = isAlert;
            this.areYouAlive = areYouAlive;
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
