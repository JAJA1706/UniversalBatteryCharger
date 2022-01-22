using System.Collections.Generic;

namespace UBCweb.Models
{
    //public class MicroData
    //{
    //    public MicroData() { canalData = new(); }
    //    public List<CanalData> canalData { get; set; }
    //}

    public class BatteryData
    {
        public Battery battery { get; set; }

        public List<Profile> chargingProfiles { get; set; }

        public bool isDataCorrect()
        {
            if(battery.MinVoltage == "0")
            {
                return false;
            }
            return true;
        }
    }

}