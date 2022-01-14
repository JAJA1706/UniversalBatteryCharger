using Microsoft.AspNetCore.Mvc;
using System.Text.Encodings.Web;
using System;
using System.Text.Json;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using UBCweb.Models;

namespace UBCweb.Controllers
{
    public class Token
    {
        public string token { get; set; }
    }

    [ApiController]
    [Route("[controller]")]
    public class LoginController : ControllerBase
    {
        [HttpPost]
        public string Index(Credentials credentials)
        {
            string[] splits = { "0", "", "" };
            if (System.IO.File.Exists(RegisterCredentials.FILE_PATH))
            {
                string[] lines = System.IO.File.ReadAllLines(RegisterCredentials.FILE_PATH);
                for (int i = 0; i < lines.Length; ++i)
                {
                    splits = lines[i].Split(';');
                    if (splits[1].Equals(credentials.Username) && splits[2].Equals(credentials.Password))
                    {
                        Token token = new Token { token = splits[0] };
                        return JsonSerializer.Serialize(token);
                    }
                }
            }

            return JsonSerializer.Serialize("no");
        }
    }
}