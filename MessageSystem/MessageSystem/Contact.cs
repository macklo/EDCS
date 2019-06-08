using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;

namespace MessageSystem
{
    [DataContract]
    class Contact
    {
        [DataMember]
        public string ipAddress;

        [DataMember]
        public string name;

        public string messages;

        public Contact (string name, string ipAddress)
        {
            this.ipAddress = ipAddress;
            this.name = name;
            this.messages = "";
        }
    }
}
