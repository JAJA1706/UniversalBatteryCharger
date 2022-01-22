using Microsoft.AspNetCore.Mvc;
using System.Xml;
using System.Text;
using System.Text.Json;
using System.IO; 
using System.Collections.Generic;
using UBCweb.Models;
using System;

namespace UBCweb.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class UserDataController : ControllerBase
    {
        [HttpGet]
        [Route("GetCanals/{id}")]
        public string GetCanalData(int id)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            lock (UserData.userDataLock)
            {
                if (!System.IO.File.Exists(filePath))
                {
                    FileStream file = System.IO.File.OpenWrite(filePath);
                    string encoding = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<User></User>";
                    file.Write(Encoding.UTF8.GetBytes(encoding));
                    file.Close();
                    return "[]";
                }
            }


            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            List<CanalNum> canalList = new List<CanalNum>();
            int i = 0;
            foreach (XmlNode node in xmlDoc.GetElementsByTagName("charger"))
            {
                CanalNum canal = new CanalNum();
                canal.canalNum = i;
                canalList.Add(canal);
                ++i;
            }

            return JsonSerializer.Serialize(canalList);
        }

        [HttpPut]
        [Route("AddCanal/{id}")]
        public string AddNewCanal(int id)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            if (xmlDoc.GetElementsByTagName("charger").Count >= 4)
            {
                return JsonSerializer.Serialize("maksymalna ilosc kanalow dla wersji non premium osiagnieta");
            }

            XmlNode root = xmlDoc.DocumentElement;
            XmlElement newCanal = xmlDoc.CreateElement("charger");
            for (int i = 0; i < UserData.MAX_BATTERIES; ++i)
            {
                XmlElement newBattery = xmlDoc.CreateElement("battery");
                newCanal.AppendChild(newBattery);
                XmlElement mode = xmlDoc.CreateElement("mode");
                XmlElement capacity = xmlDoc.CreateElement("capacity");
                XmlElement minVoltage = xmlDoc.CreateElement("minVoltage");
                XmlElement chargingProfiles = xmlDoc.CreateElement("chargingProfiles");

                XmlElement completion = xmlDoc.CreateElement("completion");
                completion.InnerText = "0";
                XmlElement cellVoltage = xmlDoc.CreateElement("cellVoltage");
                cellVoltage.InnerText = "0";
                XmlElement currentFlowing = xmlDoc.CreateElement("currentFlowing");
                currentFlowing.InnerText = "0";
                XmlElement isNewData = xmlDoc.CreateElement("isNewData");
                isNewData.InnerText = "0";
                newBattery.AppendChild(mode);
                newBattery.AppendChild(capacity);
                newBattery.AppendChild(minVoltage);
                newBattery.AppendChild(chargingProfiles);
                newBattery.AppendChild(completion);
                newBattery.AppendChild(cellVoltage);
                newBattery.AppendChild(currentFlowing);
                newBattery.AppendChild(isNewData);
            }

            root.AppendChild(newCanal);

            UserData.SaveXML(xmlDoc, filePath);

            return JsonSerializer.Serialize("ok");
        }

        [HttpPost]
        [Route("RemoveCanal/{id}")]
        public string RemoveCanal(int id, CanalNum canal)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]", canal.canalNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode canalNode = root.SelectSingleNode(xPath);
            root.RemoveChild(canalNode);

            UserData.SaveXML(xmlDoc, filePath);

            return JsonSerializer.Serialize("ok");
        }

        [HttpPost]
        [Route("SaveBattery/{id}/{canal}/{batteryNum}")]
        public string SaveBatteryData(int id, int canal, int batteryNum, Battery battery)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]", canal + 1, batteryNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode batteryNode = root.SelectSingleNode(xPath);
            XmlNode mode = batteryNode.FirstChild;
            mode.InnerText = battery.BatteryMode;
            XmlNode capacity = mode.NextSibling;
            capacity.InnerText = battery.Capacity;
            XmlNode minVoltage = capacity.NextSibling;
            minVoltage.InnerText = battery.MinVoltage;
            XmlNode isNewData = root.SelectSingleNode(xPath+"/isNewData");
            isNewData.InnerText = "1";

            UserData.SaveXML(xmlDoc, filePath);

            return JsonSerializer.Serialize("ok");
        }

        [HttpGet]
        [Route("GetBattery/{id}/{canal}/{batteryNum}")]
        public string GetBatteryData(int id, int canal, int batteryNum)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]", canal + 1, batteryNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode batteryNode = root.SelectSingleNode(xPath);
            Battery battery = UserData.GetBatteryDataFromNode(batteryNode);

            return JsonSerializer.Serialize(battery);
        }

        [HttpGet]
        [Route("GetProfilesList/{id}/{canal}/{batteryNum}")]
        public string GetProfilesList(int id, int canal, int batteryNum)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]/chargingProfiles", canal + 1, batteryNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode chargingProfiles = root.SelectSingleNode(xPath);

            List<ProfileNum> profileList = new List<ProfileNum>();
            int i = 0;
            foreach (XmlNode node in chargingProfiles.ChildNodes)
            {
                ProfileNum profile = new ProfileNum();
                profile.profileNum = i;
                profileList.Add(profile);
                ++i;
            }

            return JsonSerializer.Serialize(profileList);
        }

        [HttpPut]
        [Route("AddProfile/{id}/{canal}/{batteryNum}")]
        public string AddNewProfile(int id, int canal, int batteryNum)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]/chargingProfiles", canal + 1, batteryNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode chargingProfiles = root.SelectSingleNode(xPath);

            if (chargingProfiles.ChildNodes.Count >= 4)
            {
                return JsonSerializer.Serialize("maksymalna ilosc profili osiagnieta");
            }

            XmlElement newProfile = xmlDoc.CreateElement("profile");
            XmlElement method = xmlDoc.CreateElement("method");
            XmlElement maxVoltage = xmlDoc.CreateElement("maxVoltage");
            XmlElement maxCurrent = xmlDoc.CreateElement("maxCurrent");
            XmlElement maxTemperature = xmlDoc.CreateElement("maxTemperature");
            XmlElement maxTime = xmlDoc.CreateElement("maxTime");
            XmlElement desiredVoltage = xmlDoc.CreateElement("desiredVoltage");
            XmlElement desiredCurrent = xmlDoc.CreateElement("desiredCurrent");
            XmlElement voltageDeltaInverval = xmlDoc.CreateElement("voltageDeltaIntverval");
            XmlElement minVoltageDelta = xmlDoc.CreateElement("minVoltageDelta");
            XmlElement temperatureDeltaInterval = xmlDoc.CreateElement("temperatureDeltaInterval");
            XmlElement maxTemperatureDelta = xmlDoc.CreateElement("maxTemperatureDelta");
            XmlElement endingVoltageDrop = xmlDoc.CreateElement("endingVoltageDrop");
            XmlElement lookForEndingVoltageDrop = xmlDoc.CreateElement("lookForEndingVoltageDrop");
            newProfile.AppendChild(method);
            newProfile.AppendChild(maxVoltage);
            newProfile.AppendChild(maxCurrent);
            newProfile.AppendChild(maxTemperature);
            newProfile.AppendChild(maxTime);
            newProfile.AppendChild(desiredVoltage);
            newProfile.AppendChild(desiredCurrent);
            newProfile.AppendChild(voltageDeltaInverval);
            newProfile.AppendChild(minVoltageDelta);
            newProfile.AppendChild(temperatureDeltaInterval);
            newProfile.AppendChild(maxTemperatureDelta);
            newProfile.AppendChild(endingVoltageDrop);
            newProfile.AppendChild(lookForEndingVoltageDrop);

            chargingProfiles.AppendChild(newProfile);

            UserData.SaveXML(xmlDoc, filePath);

            return JsonSerializer.Serialize("ok");
        }

        [HttpPost]
        [Route("RemoveProfile/{id}/{canal}/{batteryNum}/{profileNum}")]
        public string RemoveProfile(int id, int canal, int batteryNum, int profileNum)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]/chargingProfiles/profile[{2}]", canal + 1, batteryNum + 1, profileNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode profile = root.SelectSingleNode(xPath);
            XmlNode chargingProfiles = profile.ParentNode;
            chargingProfiles.RemoveChild(profile);

            UserData.SaveXML(xmlDoc, filePath);

            return JsonSerializer.Serialize("ok");
        }

        [HttpPost]
        [Route("SaveProfile/{id}/{canal}/{batteryNum}/{profileNum}")]
        public string SaveProfileData(int id, int canal, int batteryNum, int profileNum, Profile profile)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]/chargingProfiles/profile[{2}]", canal + 1, batteryNum + 1, profileNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode profileNode = root.SelectSingleNode(xPath);
            XmlNode method = profileNode.FirstChild;
            method.InnerText = profile.method;
            XmlNode maxVoltage = method.NextSibling;
            maxVoltage.InnerText = profile.maxVoltage;
            XmlNode maxCurrent = maxVoltage.NextSibling;
            maxCurrent.InnerText = profile.maxCurrent;
            XmlNode maxTemperature = maxCurrent.NextSibling;
            maxTemperature.InnerText = profile.maxTemperature;
            XmlNode maxTime = maxTemperature.NextSibling;
            maxTime.InnerText = profile.maxTime;
            XmlNode desiredVoltage = maxTime.NextSibling;
            desiredVoltage.InnerText = profile.desiredVoltage;
            XmlNode desiredCurrent = desiredVoltage.NextSibling;
            desiredCurrent.InnerText = profile.desiredCurrent;
            XmlNode voltageDeltaInverval = desiredCurrent.NextSibling;
            voltageDeltaInverval.InnerText = profile.voltageDeltaInverval;
            XmlNode minVoltageDelta = voltageDeltaInverval.NextSibling;
            minVoltageDelta.InnerText = profile.minVoltageDelta;
            XmlNode temperatureDeltaInterval = minVoltageDelta.NextSibling;
            temperatureDeltaInterval.InnerText = profile.temperatureDeltaInterval;
            XmlNode maxTemperatureDelta = temperatureDeltaInterval.NextSibling;
            maxTemperatureDelta.InnerText = profile.maxTemperatureDelta;
            XmlNode endingVoltageDrop = maxTemperatureDelta.NextSibling;
            endingVoltageDrop.InnerText = profile.endingVoltageDrop;
            XmlNode lookForEndingVoltageDrop = endingVoltageDrop.NextSibling;
            lookForEndingVoltageDrop.InnerText = profile.lookForEndingVoltageDrop;

            UserData.SaveXML(xmlDoc, filePath);

            return JsonSerializer.Serialize("ok");
        }

        [HttpGet]
        [Route("GetProfile/{id}/{canal}/{batteryNum}/{profileNum}")]
        public string GetProfileData(int id, int canal, int batteryNum, int profileNum)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]/chargingProfiles/profile[{2}]", canal + 1, batteryNum + 1, profileNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode profileNode = root.SelectSingleNode(xPath);
            Profile profile = UserData.GetProfileDataFromNode(profileNode);

            return JsonSerializer.Serialize(profile);
        }

        [HttpGet]
        [Route("GetMonitorData/{id}/{canal}/{batteryNum}")]
        public string GetBatteryMonitorData(int id, int canal, int batteryNum)
        {
            string filePath = UserData.GetUserDataFilePath(id);
            XmlDocument xmlDoc = new();
            lock (UserData.userDataLock)
            {
                xmlDoc.Load(filePath);
            }

            string xPath = String.Format("charger[{0}]/battery[{1}]/completion", canal + 1, batteryNum + 1);
            XmlElement root = xmlDoc.DocumentElement;
            XmlNode completion = root.SelectSingleNode(xPath);
            BatteryMonitorData data = new();
            data.Completion = completion.InnerText;
            XmlNode cellVoltage = completion.NextSibling;
            data.CellVoltage = cellVoltage.InnerText;
            XmlNode currentFlowing = cellVoltage.NextSibling;
            data.CurrentFlowing = currentFlowing.InnerText;

            return JsonSerializer.Serialize(data);
        }
    }
}