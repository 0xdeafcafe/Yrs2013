using System.IO;
using System.Net;
using ModeManager.Twitter;

namespace ModeManager.Helpers
{
    public static class Server
    {
        public static void SubmitStats(Session session)
        {


            //// POST it to the server
            //var request = (HttpWebRequest)WebRequest.Create("http://jewllama.arborvs.co.uk/api/submit.php");
            //request.Method = "POST";
            //request.ContentType = "application/octet-stream";
            //request.ContentLength = json.Length;
            //Stream dataStream = null;
            //try
            //{
            //    dataStream = request.GetRequestStream();
            //    dataStream.Write(json.GetBuffer(), 0, (int)json.Length);
            //}
            //catch (WebException)
            //{
            //    return;
            //}
            //finally
            //{
            //    if (dataStream != null)
            //        dataStream.Close();
            //    json.Close();
            //}
        }
    }
}
