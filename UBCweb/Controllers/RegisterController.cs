using Microsoft.AspNetCore.Mvc;
using System.Text.Encodings.Web;
using System;
using System.IO;
using System.Text;
using System.Text.Json;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
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
            if ( string.IsNullOrEmpty(credentials.Username) )
                return JsonSerializer.Serialize("Nazwa uzytkownika nie moze byc pusta");
            if ( string.IsNullOrEmpty(credentials.Password) || string.IsNullOrEmpty(credentials.Password2))
                return JsonSerializer.Serialize("Haslo nie moze byc puste");
            if( CheckForUnallowedChars(credentials.Username) )
                return JsonSerializer.Serialize("Nazwa uzytkownika moze zawierac tylko liczby i litery");
            if (CheckForUnallowedChars(credentials.Password) || CheckForUnallowedChars(credentials.Password2))
                return JsonSerializer.Serialize("Haslo moze zawierac tylko liczby i litery");
            if (credentials.Password != credentials.Password2)
                return JsonSerializer.Serialize("hasla nie sa sobie rowne");

            string[] splits = { "0", "", "" };
            if (System.IO.File.Exists(RegisterCredentials.FILE_PATH)){
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

            int id = Int32.Parse(splits[0]);
            ++id;
            string newData = id.ToString() + ';' + credentials.Username + ';' + credentials.Password + '\n';
            FileStream fs = System.IO.File.OpenWrite(RegisterCredentials.FILE_PATH);
            fs.Seek(0, SeekOrigin.End);
            fs.Write(Encoding.UTF8.GetBytes(newData));
            fs.Close();

            return JsonSerializer.Serialize("profil utworzony");
        }

        private bool CheckForUnallowedChars( string str )
        {
            foreach(char x in str )
            {
                if (x < 48 || (x > 57 && x < 65) || (x > 90 && x < 97) || x > 122)
                    return true;
            }

            return false;
        }
    }
}