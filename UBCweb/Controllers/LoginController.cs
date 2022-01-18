using Microsoft.AspNetCore.Mvc;
using System;
using System.Text;
using System.Text.Json;
using System.IO;
using UBCweb.Models;
using System.Security.Cryptography;

namespace UBCweb.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class LoginController : ControllerBase
    {
        [HttpPost]
        public string Index(Credentials credentials)
        {
            string[] splits = { "0", "", "" };
            lock (RegisterCredentials.credentialsLock)
            {
                if (System.IO.File.Exists(RegisterCredentials.FILE_PATH))
                {
                    string[] lines = System.IO.File.ReadAllLines(RegisterCredentials.FILE_PATH);
                    for (int i = 0; i < lines.Length; ++i)
                    {
                        splits = lines[i].Split(';');
                        if (splits[1].Equals(credentials.Username) && splits[2].Equals(credentials.Password))
                        {
                            RNGCryptoServiceProvider tokenGenerator = new RNGCryptoServiceProvider();
                            Byte[] bytes = new byte[16];
                            tokenGenerator.GetBytes(bytes);
                            UInt32 tokenValue = BitConverter.ToUInt32(bytes);

                            int userID = Convert.ToInt32(splits[0]);
                            Token token = new Token { UserID = userID, TokenVal = tokenValue.ToString() };
                            Credentials.AddUserToLogged(credentials.Username, tokenValue);
                            return JsonSerializer.Serialize(token);
                        }
                    }
                }
            }

            return JsonSerializer.Serialize("no");
        }

        [HttpPost]
        [Route("checkToken")]
        public string checkToken(Token token)
        {
            
            string[] splits = { "0", ""};
            lock (Credentials.loggedLock)
            {
                if (System.IO.File.Exists(Credentials.FILE_PATH_LOGGED))
                {
                    string[] lines = System.IO.File.ReadAllLines(Credentials.FILE_PATH_LOGGED);
                    for (int i = 0; i < lines.Length; ++i)
                    {
                        splits = lines[i].Split(';');
                        if (splits[1].Equals(token.TokenVal))
                        {
                            return JsonSerializer.Serialize("good");
                        }
                    }
                }
            }
            return JsonSerializer.Serialize("bad");
        }
    }
}