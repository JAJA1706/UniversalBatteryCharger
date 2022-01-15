using Microsoft.AspNetCore.Mvc;
using UBCweb.Models;

namespace UBCweb.Controllers
{

    [ApiController]
    [Route("[controller]")]
    public class LogoutController : ControllerBase
    {
        [HttpPost]
        public void Index(Token token)
        {
            if (System.IO.File.Exists(Credentials.FILE_PATH_LOGGED))
            {
                string text;
                lock (Credentials.loggedLock)
                {
                   text = System.IO.File.ReadAllText(Credentials.FILE_PATH_LOGGED);
                }
                int pos = text.IndexOf(';');
                while (pos != -1)
                {
                    int endLine = text.IndexOf('\n', pos);
                    if (text.Substring(pos+1, endLine - pos - 1).Equals(token.TokenVal))
                    {
                        int currentLine = text.LastIndexOf('\n', pos);
                        int charsToRemove;
                        if (currentLine == -1)
                        {
                            charsToRemove = endLine+1;
                        }
                        else
                        {
                            charsToRemove = endLine - currentLine;
                        }
                        text = text.Remove(currentLine+1, charsToRemove);
                        break;
                    }
                    pos = text.IndexOf(';', pos + 1);
                }
                lock (Credentials.loggedLock)
                {
                    System.IO.File.WriteAllText(Credentials.FILE_PATH_LOGGED, text);
                }
            }
        }
    }
}