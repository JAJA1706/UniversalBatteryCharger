using System;

namespace UBCweb.Models
{
    public class RegisterCredentials
    {
        public const string FILE_PATH = "credentials";
        public string Username { get; set; }
        public string Password { get; set; }
        public string Password2 { get; set; }
    }
    public class Credentials
    {
        public string Username { get; set; }
        public string Password { get; set; }
    }
}