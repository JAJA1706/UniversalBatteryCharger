using Microsoft.AspNetCore.Mvc;
using System;
using System.Text;
using System.Text.Json;
using System.IO; 
using System.Collections.Generic;
using UBCweb.Models;

namespace UBCweb.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class UserDataController : ControllerBase
    {
        [HttpGet]
        [Route("GetCanals/{id}")]
        public string getCanalData(int id)
        {
            //read xml;
            List<Canal> canalList = new List<Canal>();
            for(int i=0; i<4; ++i)
            {
                Canal canal = new Canal();
                canal.Name = "kanal " + i.ToString();
                canalList.Add(canal);
            }
            string what = JsonSerializer.Serialize(canalList);
            return what;
        }
    }
}