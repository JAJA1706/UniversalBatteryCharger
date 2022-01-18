using Microsoft.AspNetCore.Mvc;
using System;
using System.IO;
using System.Text;
using System.Text.Json;
using UBCweb.Models;

namespace UBCweb.Controllers
{

    [ApiController]
    [Route("[controller]")]
    public class RegisterController : ControllerBase
    {
        [HttpPost]
        public string Index(RegisterCredentials credentials)
        {

            string result = RegisterCredentials.CheckCredentials(credentials);
            if(result != "")
                return JsonSerializer.Serialize(result);

            string[] splits = { "0", "", "" };
            lock (RegisterCredentials.credentialsLock)
            {
                if (System.IO.File.Exists(RegisterCredentials.FILE_PATH))
                {
                    string[] lines = System.IO.File.ReadAllLines(RegisterCredentials.FILE_PATH);
                    for (int i = 0; i < lines.Length; ++i)
                    {
                        splits = lines[i].Split(';');
                        if (splits[1].Equals(credentials.Username))
                        {
                            return JsonSerializer.Serialize("Taki uzytkownik juz istnieje");
                        }
                    }
                }
            }

            int id = Int32.Parse(splits[0]);
            ++id;
            string newData = id.ToString() + ';' + credentials.Username + ';' + credentials.Password + '\n';
            lock (RegisterCredentials.credentialsLock)
            {
                FileStream fs = System.IO.File.OpenWrite(RegisterCredentials.FILE_PATH);
                fs.Seek(0, SeekOrigin.End);
                fs.Write(Encoding.UTF8.GetBytes(newData));
                fs.Close();
            }

            return JsonSerializer.Serialize("profil utworzony");
        }
    }
}