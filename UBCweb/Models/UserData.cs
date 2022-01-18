namespace UBCweb.Models
{
    public class UserData
    {
        public const int MAX_BATTERIES = 2;
        public const string USER_DATA_FOLDER = "userdata/";
        public static readonly object userDataLock = new();
        public int Id { get; set; }
        public string Username { get; set; }
        public string Password { get; set; }

        public static string GetUserDataFilePath(int id)
        {
            return USER_DATA_FOLDER + id.ToString() + ".xml";
        }
    }

    public class CanalNum
    {
        public int canalNum { get; set; }
    }

    public class ProfileNum
    {
        public int profileNum { get; set; }
    }
}