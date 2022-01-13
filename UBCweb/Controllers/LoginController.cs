using Microsoft.AspNetCore.Mvc;
using System.Text.Encodings.Web;
using System;
using System.Text.Json;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using UBCweb.Models;

namespace UBCweb.Controllers
{
    public class Token
    {
        public string token { get; set; }
    }
    public class Credentials
    {
        public string Username { get; set; }
        public string Password { get; set; }
    }

    [ApiController]
    [Route("[controller]")]
    public class LoginController : Controller
    {
        [HttpPost]
        public string Index(Credentials credentials)
        {
            if(credentials.Username.Equals("dick"))
            {
                Token token = new Token { token = "test123" };
                return JsonSerializer.Serialize(token);
            }
            return "no";
        }

        //public List<UserModel> PutValue()
        //{
        //    var users = new List<UserModel>
        //    {
        //        new UserModel{ Id=1, Username="bals", Password="dick"}
        //    };

        //    return users;
        //}

        //[HttpPost]
        //public IActionResult Verify(UserModel usr)
        //{
        //    var u = PutValue();
        //    var ue = u.Where(u => u.Username.Equals(usr.Username));
        //    var up = ue.Where(p => p.Password.Equals(usr.Password));
        //}
    }
}