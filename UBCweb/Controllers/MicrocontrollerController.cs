using Microsoft.AspNetCore.Mvc;
using System;
using System.Text;
using System.Text.Json;
using System.IO;
using System.Xml;
using UBCweb.Models;
using System.Collections.Generic;

namespace UBCweb.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class MicrocontrollerController : ControllerBase
    {

        [HttpGet]
        [Route("getBatteryData/{userID}/{canalNum}/{batteryNum}")]
        public string GetBatteryData(int userID, int canalNum, int batteryNum)
        {
            string filePath = UserData.GetUserDataFilePath(userID);
            if (!System.IO.File.Exists(filePath))
            {
                return JsonSerializer.Serialize("no");
            }

            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            BatteryData data = new();

            string xPath = String.Format("charger[{0}]/battery[{1}]", canalNum + 1, batteryNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode batteryNode = root.SelectSingleNode(xPath);
            if (batteryNode is null)
            {
                return JsonSerializer.Serialize("no");
            }
            XmlNode isNewData = root.SelectSingleNode(xPath + "/isNewData");
            if(isNewData.InnerText == "0")
            {
                return JsonSerializer.Serialize("no");
            }

            Battery battery = UserData.GetBatteryDataFromNode(batteryNode);
            List<Profile> chargingProfilesList = new();

            XmlNode chargingProfiles = batteryNode.SelectSingleNode("chargingProfiles");
            foreach (XmlNode profileNode in chargingProfiles.ChildNodes)
            {
                Profile profile = UserData.GetProfileDataFromNode(profileNode);
                chargingProfilesList.Add(profile);
            }

            data.battery = battery;
            data.chargingProfiles = chargingProfilesList;
            if (data.isDataCorrect())
            {
                isNewData.InnerText = "0";
                UserData.SaveXML(xmlDoc, filePath);
                return JsonSerializer.Serialize(data);
            }
            else
            {
                return JsonSerializer.Serialize("no");
            }
        }

        [HttpPost]
        [Route("sendChargingData/{userID}/{canalNum}/{batteryNum}")]
        public string SaveChargingData(int userID, int canalNum, int batteryNum, BatteryMonitorData data)
        {
            string filePath = UserData.GetUserDataFilePath(userID);
            if (!System.IO.File.Exists(filePath))
            {
                return JsonSerializer.Serialize("no");
            }

            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]/completion", canalNum + 1, batteryNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode completion = root.SelectSingleNode(xPath);
            if( completion is null )
            {
                return JsonSerializer.Serialize("no");
            }
            completion.InnerText = data.Completion;
            XmlNode cellVoltage = completion.NextSibling;
            cellVoltage.InnerText = data.CellVoltage;
            XmlNode currentFlowing = cellVoltage.NextSibling;
            currentFlowing.InnerText = data.CurrentFlowing;

            lock (UserData.userDataLock)
            {
                FileStream fs = System.IO.File.Open(filePath, FileMode.Truncate);
                xmlDoc.Save(fs);
                fs.Close();
            }

            ////do testowwwwwwwwwww
            //string testData = data.Completion + ';' + data.CellVoltage + ';' + data.CurrentFlowing + '\n';
            //FileStream pog = System.IO.File.OpenWrite("testy.csv");
            //pog.Seek(0, SeekOrigin.End);
            //pog.Write(Encoding.UTF8.GetBytes(testData));
            //pog.Close();

            return JsonSerializer.Serialize("ok");
        }
    }
}