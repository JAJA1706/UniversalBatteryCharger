using System;
using System.IO;
using System.Text;

namespace UBCweb.Models
{
    public class RegisterCredentials
    {
        public const string FILE_PATH = "credentials";
        public static readonly object credentialsLock = new();
        public string Username { get; set; }
        public string Password { get; set; }
        public string Password2 { get; set; }

        public static string CheckCredentials(RegisterCredentials credentials )
        {
            if (string.IsNullOrEmpty(credentials.Username))
                return "Nazwa uzytkownika nie moze byc pusta";
            if (string.IsNullOrEmpty(credentials.Password) || string.IsNullOrEmpty(credentials.Password2))
                return "Haslo nie moze byc puste";
            if (CheckForUnallowedChars(credentials.Username))
                return "Nazwa uzytkownika moze zawierac tylko liczby i litery";
            if (CheckForUnallowedChars(credentials.Password) || CheckForUnallowedChars(credentials.Password2))
                return "Haslo moze zawierac tylko liczby i litery";
            if (credentials.Password != credentials.Password2)
                return "hasla nie sa sobie rowne";

            return "";
        }
        
        public static bool CheckForUnallowedChars(string str)
        {
            foreach (char x in str)
            {
                if (x < 48 || (x > 57 && x < 65) || (x > 90 && x < 97) || x > 122)
                    return true;
            }

            return false;
        }
    }
    public class Credentials
    {
        public const string FILE_PATH_LOGGED = "loggedUsers";
        public static readonly object loggedLock = new();
        public string Username { get; set; }
        public string Password { get; set; }

        public static void AddUserToLogged(string username, UInt32 tokenValue)
        {
            string newData = username + ';' + tokenValue.ToString() + '\n';
            lock (loggedLock)
            {
                FileStream fs = System.IO.File.OpenWrite(FILE_PATH_LOGGED);
                fs.Seek(0, SeekOrigin.End);
                fs.Write(Encoding.UTF8.GetBytes(newData));
                fs.Close();
            }
        }
    }

    public class Token
    {
        public int UserID { get; set; }
        public string TokenVal { get; set; }
    }
}