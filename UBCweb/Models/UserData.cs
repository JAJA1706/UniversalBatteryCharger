using System.Xml;
using System.IO;

namespace UBCweb.Models
{
    public class UserData
    {
        public const int MAX_BATTERIES = 2;
        public const string USER_DATA_FOLDER = "userdata/";
        public static readonly object userDataLock = new(); //ich powinno byc tyle co plikow
        public int Id { get; set; }
        public string Username { get; set; }
        public string Password { get; set; }

        public static string GetUserDataFilePath(int id)
        {
            return USER_DATA_FOLDER + id.ToString() + ".xml";
        }

        public static Battery GetBatteryDataFromNode(XmlNode batteryNode)
        {
            Battery battery = new();
            XmlNode mode = batteryNode.FirstChild;
            battery.BatteryMode = mode.InnerText;
            XmlNode capacity = mode.NextSibling;
            battery.Capacity = capacity.InnerText;
            XmlNode minVoltage = capacity.NextSibling;
            battery.MinVoltage = minVoltage.InnerText;

            return battery;
        }

        public static Profile GetProfileDataFromNode(XmlNode profileNode)
        {
            Profile profile = new();
            XmlNode method = profileNode.FirstChild;
            profile.method = method.InnerText;
            XmlNode maxVoltage = method.NextSibling;
            profile.maxVoltage = maxVoltage.InnerText;
            XmlNode maxCurrent = maxVoltage.NextSibling;
            profile.maxCurrent = maxCurrent.InnerText;
            XmlNode maxTemperature = maxCurrent.NextSibling;
            profile.maxTemperature = maxTemperature.InnerText;
            XmlNode maxTime = maxTemperature.NextSibling;
            profile.maxTime = maxTime.InnerText;
            XmlNode desiredVoltage = maxTime.NextSibling;
            profile.desiredVoltage = desiredVoltage.InnerText;
            XmlNode desiredCurrent = desiredVoltage.NextSibling;
            profile.desiredCurrent = desiredCurrent.InnerText;
            XmlNode voltageDeltaInverval = desiredCurrent.NextSibling;
            profile.voltageDeltaInverval = voltageDeltaInverval.InnerText;
            XmlNode minVoltageDelta = voltageDeltaInverval.NextSibling;
            profile.minVoltageDelta = minVoltageDelta.InnerText;
            XmlNode temperatureDeltaInterval = minVoltageDelta.NextSibling;
            profile.temperatureDeltaInterval = temperatureDeltaInterval.InnerText;
            XmlNode maxTemperatureDelta = temperatureDeltaInterval.NextSibling;
            profile.maxTemperatureDelta = maxTemperatureDelta.InnerText;
            XmlNode endingVoltageDrop = maxTemperatureDelta.NextSibling;
            profile.endingVoltageDrop = endingVoltageDrop.InnerText;
            XmlNode lookForEndingVoltageDrop = endingVoltageDrop.NextSibling;
            profile.lookForEndingVoltageDrop = lookForEndingVoltageDrop.InnerText;

            return profile;
        }

        public static void SaveXML(XmlDocument xmlDoc, string filePath)
        {
            lock (userDataLock)
            {
                FileStream fs = System.IO.File.Open(filePath, FileMode.Truncate);
                xmlDoc.Save(fs);
                fs.Close();
            }
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