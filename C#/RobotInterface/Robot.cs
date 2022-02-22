using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotInterface
{
    public class Robot
    {
        public string receivedText = "";
        public float distanceTelemetreDroit; 
        public float distanceTelemetreGauche;
        public float distanceTelemetreCentre;
        public float distanceTelemetreFullDroit;
        public float distanceTelemetreFullGauche;



        public Queue<byte> byteListRcived = new Queue<byte>();
    }
}
