using System.Diagnostics;
using System.Windows.Forms;
using ModeManager.Twitter;

namespace ModeManager
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            // Test Code
            var twitterStreamer = new Streamer("London", "Birmingham");
            var timer = new Timer { Interval = 5000 };
            timer.Tick += (sender, args) =>
            {
                var response = twitterStreamer.CalculateShittyCity();
                Debug.WriteLine("{0}: {1}", new object[] { twitterStreamer.CityA, response.Item1 });
                Debug.WriteLine("{0}: {1}", new object[] { twitterStreamer.CityB, response.Item2 });
                Debug.WriteLine("");
            };
            timer.Start();
        }
    }
}
