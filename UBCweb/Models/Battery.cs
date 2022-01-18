namespace UBCweb.Models
{
    public class Battery
    {
        public string BatteryMode { get; set; }
        public string Capacity { get; set; }
        public string MinVoltage { get; set; }
    }

    public class BatteryMonitorData
    {
        public string Completion { get; set; }
        public string CellVoltage { get; set; }
        public string CurrentFlowing { get; set; }
    }
}