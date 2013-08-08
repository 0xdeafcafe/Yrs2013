using System;
using System.Collections.Generic;
using System.Linq;
using kfouwels.lib.SentimentAnalysis;
using TweetSharp;
using System.IO;

namespace ModeManager.Twitter
{
    // TODO: Documentation

    /// <summary>
    /// 
    /// </summary>
    public class Streamer
    {
        private const string ConsumerKey = "WUdHlHvMdivzsoQ3h1qA";
        private const string ConsumerSecret = "7vtk2Ar7fzB36QOshlw2auZ4tjJdvksXafhLNdtO8g";
        private const string AccessToken = "221180324-9H7HCiCkKFun9fBRtUIMzhScKQsKSOd6JCsyHi4T";
        private const string AccessTokenSecret = "I97L9N5NV2Pir9IFPCtRJufTj4OzRxIpyXB8pHixVk";

        private Dictionary<string, sbyte> _wordList;
        private Dictionary<string, sbyte> _intensifiers;
        private Dictionary<string, sbyte> _inverters;

        private readonly String _cityA;
        private readonly String _cityB;

        private TwitterService _twitterService;
        private SentimentAnalyser _sentimentAnalyser;

        /// <summary>
        /// 
        /// </summary>
        public String CityA
        {
            get { return _cityA; }
        }
        
        /// <summary>
        /// 
        /// </summary>
        public String CityB
        {
            get { return _cityB; }
        }
        
        /// <summary>
        /// Create new instance of the twitter streaming wrapper. 
        /// </summary>
        /// <param name="cityA">Name of City A</param>
        /// <param name="cityB">Name of City B</param>
        public Streamer(String cityA, String cityB)
        {
            // Save Cities
            _cityA = cityA;
            _cityB = cityB;

            // Initalize the Twitter Service
            InitalizeTwitterService();

            // Authorize the Twitter Service
            AuthorizeTwitterService();

            // Initalize the sentiment analysier
            InitalizeSentimentAnalysis();
        }

        /// <summary>
        /// 
        /// </summary>
        private void InitalizeTwitterService()
        {
            _twitterService = new TwitterService(ConsumerKey, ConsumerSecret);
        }

        /// <summary>
        /// 
        /// </summary>
        private void AuthorizeTwitterService()
        {
            _twitterService.AuthenticateWith(AccessToken, AccessTokenSecret);
        }

        /// <summary>
        /// 
        /// </summary>
        private void InitalizeSentimentAnalysis()
        {
            // Load Sentiment Analyser Dictionaries
            _inverters = Loaders.LoadDictionaryFromTxt(@"SengimentAnalysisData/inverters1.txt");
            _intensifiers = Loaders.LoadDictionaryFromTxt(@"SengimentAnalysisData/intensifiers1.txt");
            _wordList = Loaders.LoadDictionaryFromTxt(@"SengimentAnalysisData/wordList1.txt");

            // Initalize Sentiment Analysis
            _sentimentAnalyser = new SentimentAnalyser(_wordList, _inverters, _intensifiers, false);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="query"></param>
        /// <param name="count"></param>
        /// <param name="resultType"></param>
        /// <returns></returns>
        public IEnumerable<TwitterStatus> GetRecentTweetsFromQuery(String query, int count = 20, TwitterSearchResultType resultType = TwitterSearchResultType.Recent, string language = "eu")
        {
            return _twitterService.Search(new SearchOptions { Resulttype = resultType, Count = count, Q = query, Lang = language }).Statuses;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="statuses"></param>
        /// <returns></returns>
        private static string[] ConvertTwitterStatusesToStringArray(IEnumerable<TwitterStatus> statuses)
        {
            var final = new List<String>();

            foreach (var words in from status in statuses where !status.Text.StartsWith("RT") select status.Text.Split(' '))
            {
                final.AddRange(words.ToList());
            }

            return final.ToArray();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public Tuple<int, int> CalculateShittyCity()
        {
            // Get Tweets
            var cityAWords = ConvertTwitterStatusesToStringArray(GetRecentTweetsFromQuery(CityA));
            var cityBWords = ConvertTwitterStatusesToStringArray(GetRecentTweetsFromQuery(CityB));

            // Analyse tweets from city A & B
            var cityAWordAnalysis = _sentimentAnalyser.Analyse(cityAWords);
            var cityBWordAnalysis = _sentimentAnalyser.Analyse(cityBWords);

            // Returns a Turple of data
            return new Tuple<int, int>((int)cityAWordAnalysis, (int)cityBWordAnalysis);
        }
    }

    public class Session
    {
        public Session()
        {
            DataPoints = new List<DataPoint>();
        }

        public string CityA { get; set; }
        public string CityB { get; set; }

        public IList<DataPoint> DataPoints { get; set; }
    }
    public class DataPoint
    {
        public int CityARating { get; set; }
        public int CityBRating { get; set; }
    }
}
