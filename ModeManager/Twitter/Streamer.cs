using System;
using System.Collections.Generic;
using System.Linq;
using kfouwels.lib.SentimentAnalysis;
using TweetSharp;

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

        public String CityA
        {
            get { return _cityA; }
        }
        
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
        /// <param name="query"></param>
        /// <param name="count"></param>
        /// <param name="resultType"></param>
        /// <returns></returns>
        public IEnumerable<TwitterStatus> GetRecentTweetsFromQuery(String query, int count = 20, TwitterSearchResultType resultType = TwitterSearchResultType.Recent)
        {
            return _twitterService.Search(new SearchOptions { Resulttype = resultType, Count = count, Q = query }).Statuses;
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
        public Tuple<decimal, decimal> CalculateShittyCity()
        {
            // Get Tweets
            var cityAWords = ConvertTwitterStatusesToStringArray(GetRecentTweetsFromQuery(CityA));
            var cityBWords = ConvertTwitterStatusesToStringArray(GetRecentTweetsFromQuery(CityB));

            // Load Sentiment Analyser Dictionaries
            var inverters = Loaders.LoadDictionaryFromTxt(@"SengimentAnalysisData/inverters1.txt");
            var intensifiers = Loaders.LoadDictionaryFromTxt(@"SengimentAnalysisData/intensifiers1.txt");
            var wordList = Loaders.LoadDictionaryFromTxt(@"SengimentAnalysisData/wordList1.txt");

            // Initalize the Analyser
            var sentimentAnalyser = new SentimentAnalyser(wordList, inverters, intensifiers, false);

            // Analyse tweets from city A & B
            var cityAWordAnalysis = sentimentAnalyser.Analyse(cityAWords);
            var cityBWordAnalysis = sentimentAnalyser.Analyse(cityBWords);

            // Returns a Turple of data
            return new Tuple<decimal, decimal>(cityAWordAnalysis, cityBWordAnalysis);
        }
    }
}
