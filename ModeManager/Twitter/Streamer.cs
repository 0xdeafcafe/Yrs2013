﻿using System;
using System.Collections.Generic;
using TweetSharp;

namespace ModeManager.Twitter
{
    public class Streamer
    {
        private const string ConsumerKey = "WUdHlHvMdivzsoQ3h1qA";
        private const string ConsumerSecret = "7vtk2Ar7fzB36QOshlw2auZ4tjJdvksXafhLNdtO8g";
        private const string AccessToken = "221180324-9H7HCiCkKFun9fBRtUIMzhScKQsKSOd6JCsyHi4T";
        private const string AccessTokenSecret = "I97L9N5NV2Pir9IFPCtRJufTj4OzRxIpyXB8pHixVk";

        private String _cityA;
        private String _cityB;
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
        private IEnumerable<TwitterStatus> GetRecentTweetsFromQuery(string query, int count = 50, TwitterSearchResultType resultType = TwitterSearchResultType.Recent)
        {
            return _twitterService.Search(new SearchOptions {Resulttype = resultType, Count = count, Q = query}).Statuses;
        }

    }
}
