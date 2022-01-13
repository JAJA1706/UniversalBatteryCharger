using Microsoft.AspNetCore.Mvc;
using System.Text.Encodings.Web;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UBCweb.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class BruhController : ControllerBase
    {
        [HttpGet]
        public string Index()
        {
            return "bals";
        }
        [HttpGet("gigabals/")]
        public string GigaBals()
        {
            return "Gigabals";
        }
    }
}