using System.Diagnostics;
using System.Windows.Forms;
using ModeManager.Twitter;
using System.Collections.Generic;
using System.IO;

namespace ModeManager
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            // Test Code
            var twitterStreamer = new Streamer("London", "New York");

            //var data = new List<string>();
            //for (int i = 0; i < 3; i++)
            //{
            //    var tweets = twitterStreamer.GetRecentTweetsFromQuery("royal wedding", 100);
            //    foreach (var tweet in tweets)
            //        data.Add(tweet.Text);
            //}
            //File.WriteAllLines("data.txt", data.ToArray());

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
