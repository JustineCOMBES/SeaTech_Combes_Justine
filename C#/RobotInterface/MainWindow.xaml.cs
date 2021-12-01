using ExtendedSerialPort;
using System;
using System.Collections.Generic;
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

namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        serialPort1 = new ReliableSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
        serialPort1.Open();

        public MainWindow()
        {
            InitializeComponent();
        }

        void SendMessage()
        {
            // envoi des messages
            string message = TextBoxEmission.Text;
            if (TextBoxReception.Text == "")
                TextBoxReception.Text = TextBoxReception.Text  + "Reçu : " + message;
            else
                TextBoxReception.Text = TextBoxReception.Text + "\nReçu : " + message;

            TextBoxEmission.Text = "";
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();

            // gestion des couleurs
            if (buttonEnvoyer.Background == Brushes.RoyalBlue)
                buttonEnvoyer.Background = Brushes.Beige;
            else
                buttonEnvoyer.Background = Brushes.RoyalBlue;
        }



        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                SendMessage();

        }
    }
}
