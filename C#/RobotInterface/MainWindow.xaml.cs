using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;



namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        Robot robot;
        DispatcherTimer timerAffichage;

        public MainWindow()
        {
            InitializeComponent();
            robot = new Robot();
            serialPort1 = new ReliableSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0,0,0,0,100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();
        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            //throw new NotImplementedException();
            //for(int i =0;i< robot.byteListRcived.Count) { 
            // byte message2 = robot.byteListRcived.Peek();
            //TextBoxReception.Text.WriteLine(message2.ToString);
            //}

            while (robot.byteListRcived.Count() > 0)
            {
                var c = robot.byteListRcived.Dequeue();
                TextBoxReception.Text = TextBoxReception.Text + " 0x" + c.ToString("X2");
            }
        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            for(int i =0; i < e.Data.Length; i++) {
                robot.byteListRcived.Enqueue(e.Data[i]);
            }
            
        }

        void SendMessage()
        {
            // envoi des messages
            string message = TextBoxEmission.Text;

            serialPort1.WriteLine(message);

            //if (TextBoxReception.Text == "")
            //{
            //    TextBoxReception.Text = TextBoxReception.Text + "Reçu : " + message;
            //}
            
            //else
            //{
            //    TextBoxReception.Text = TextBoxReception.Text + "\nReçu : " + message;
            //}
            TextBoxEmission.Text = "";
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();

            // gestion des couleurs
           // if (buttonEnvoyer.Background == Brushes.RoyalBlue)
             //   buttonEnvoyer.Background = Brushes.Beige;
            //else
              //  buttonEnvoyer.Background = Brushes.RoyalBlue;
        }

        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            TextBoxReception.Clear();

        
        }

        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] bytelist = new byte[20];
            for (int i = 0; i<20; i++)
            {
                bytelist[i] = (byte)(2 * i);

                if (TextBoxReception.Text == "")
                {
                    TextBoxReception.Text = TextBoxReception.Text + "Reçu : " + bytelist[i];
                }

                else
                {
                    TextBoxReception.Text = TextBoxReception.Text + "\nReçu : " + bytelist[i];
                }
                TextBoxEmission.Text = "";
            }
            serialPort1.Write(bytelist,0,bytelist.Length);

            
        }
    

        private void TextBoxReception_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
                TextBoxReception.Clear();

        }

        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                SendMessage();

        }
    }
}
