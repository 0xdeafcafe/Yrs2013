using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ModeManager.Twitter
{
    public class Streamer
    {
        private String _cityA;
        private String _cityB;

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
            _cityA = cityA;
            _cityB = cityB;
        }
    }
}
