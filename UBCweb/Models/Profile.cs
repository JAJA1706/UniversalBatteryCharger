namespace UBCweb.Models
{
    public class Profile
    {
        public string method { get; set; }
        public string maxVoltage { get; set; }
        public string maxCurrent { get; set; }
        public string maxTemperature { get; set; }
        public string maxTime { get; set; }
        public string desiredVoltage { get; set; }
        public string desiredCurrent { get; set; }
        public string voltageDeltaInverval { get; set; }
        public string minVoltageDelta { get; set; }
        public string temperatureDeltaInterval { get; set; }
        public string maxTemperatureDelta { get; set; }
        public string endingVoltageDrop { get; set; }
        public string lookForEndingVoltageDrop { get; set; }
    }
}