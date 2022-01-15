using System;

namespace UBCweb.Models
{
    public class RegisterCredentials
    {
        public const string FILE_PATH = "credentials";
        public static readonly object credentialsLock = new();
        public string Username { get; set; }
        public string Password { get; set; }
        public string Password2 { get; set; }
    }
    public class Credentials
    {
        public const string FILE_PATH_LOGGED = "loggedUsers";
        public static readonly object loggedLock = new();
        public string Username { get; set; }
        public string Password { get; set; }
    }

    public class Token
    {
        public string TokenVal { get; set; }
    }
}